#include "acubesat.hpp"


Eigen::Matrix<float, MATRIX_SIZE, MATRIX_SIZE> Acubesat::skew(const Eigen::Vector3f& vector) {
    Eigen::Matrix<float, MATRIX_SIZE, MATRIX_SIZE> skewMatrix;
    skewMatrix(0, 0) = 0;
    skewMatrix(0, 1) = -vector(2);
    skewMatrix(0, 2) = vector(1);
    skewMatrix(1, 0) = vector(2);
    skewMatrix(1, 1) = 0;
    skewMatrix(1, 2) = -vector(0);
    skewMatrix(2, 0) = -vector(1);
    skewMatrix(2, 1) = vector(0);
    skewMatrix(2, 2) = 0;

    return skewMatrix;
}

Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> Acubesat::stateTransitionJacobian(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements) {
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> transitionMatrix; // NOLINT
    transitionMatrix(Eigen::seq(0, 2), Eigen::seq(0, 2)) = skew(gyroMeasurements - state(Eigen::seq(4, 6)));
    transitionMatrix(Eigen::seq(0, 2), Eigen::seq(3, 5)) = -Eigen::Matrix<float, 3, 3>::Identity();
    transitionMatrix(Eigen::seq(3, 5), Eigen::seq(0, 5)) = Eigen::Matrix<float, 3, 6>::Zero();
    
    return transitionMatrix;
}

Eigen::Quaternionf Acubesat::quaternionExponent(const Eigen::Vector3f& angularVelocity) {
    const float theta = angularVelocity.norm();

    Eigen::Quaternionf quaternionExponent;
    quaternionExponent.w() = static_cast<float>(cos(theta / 2.0)); // NOLINT
    quaternionExponent.vec() = sin(theta / 2.0) * angularVelocity.normalized(); // NOLINT

    return quaternionExponent;
}

Eigen::Quaternionf Acubesat::quaternionProduct(const Eigen::Quaternionf& quaternion1, const Eigen::Quaternionf& quaternion2) {
    Eigen::Quaternionf quaternionProduct;

    quaternionProduct.w() = quaternion1.w() * quaternion2.w() - quaternion1.vec().dot(quaternion2.vec());
    quaternionProduct.vec() = quaternion1.w() * quaternion2.vec() + quaternion2.w() * quaternion1.vec() + quaternion1.vec().cross(quaternion2.vec());

    return quaternionProduct;
}

GlobalStateVector Acubesat::stateTransitionFunction(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements, const float timestep) {
    const Eigen::Vector3f bias = state(Eigen::seq(4, 6));
    const Eigen::Vector3f angularVelocity = gyroMeasurements - bias;

    Eigen::Quaternionf currentQuaternion;
    currentQuaternion.w() = state(0);
    currentQuaternion.vec() = state(Eigen::seq(1, 3));

    Eigen::Quaternionf nextQuaternion = quaternionProduct(currentQuaternion, quaternionExponent(angularVelocity * timestep));
    
    GlobalStateVector nextState;
    nextState(0) = nextQuaternion.w();
    nextState(Eigen::seq(1, 3)) = nextQuaternion.vec();
    nextState(Eigen::seq(4, 6)) = bias;

    return nextState;
}

Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> Acubesat::matrixExponential(const Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>& A, int terms) {
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> result = Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>::Identity();
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> term = Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>::Identity();

    for (int k = 1; k < terms; ++k) {
        term = term * A / float(k);
        result += term;
    }
    return result;
}

void Acubesat::setupForOnlyGyro() {
    // Start with no rotation (identity quaternion)
    globalState(0) = 1.0f; // q_w
    globalState(1) = 0.0f; // q_x
    globalState(2) = 0.0f; // q_y
    globalState(3) = 0.0f; // q_z

    // Start with no gyro bias
    globalState(4) = 0.0f;
    globalState(5) = 0.0f;
    globalState(6) = 0.0f;

    // Initialize covariance and noise matrices
    P = Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>::Identity();
    Q = 0.001f * Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>::Identity();
}

void Acubesat::predict(const Eigen::Vector3f &gyroMeasurements, const float timestep) {
    globalState = stateTransitionFunction(globalState, gyroMeasurements, timestep);
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> F_k = stateTransitionJacobian(globalState, gyroMeasurements);
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> phi = matrixExponential(F_k * timestep);
    P = phi * P * phi.transpose() + Q;
}

void Acubesat::predict(const float gyroscopeX, const float gyroscopeY, const float gyroscopeZ, const float timestep) {
    Eigen::Vector3f gyroscopes;
    gyroscopes[0] = gyroscopeX;
    gyroscopes[1] = gyroscopeY;
    gyroscopes[2] = gyroscopeZ;
    predict(gyroscopes, timestep);
}

void Acubesat::updateEulerAngles() {
    Eigen::Quaternionf q(globalState(0), globalState(1), globalState(2), globalState(3)); // Extract quaternion from the global state
    q.normalize();  // Ensure the quaternion is normalized to avoid numerical drift
    Eigen::Vector3f eulerAngles = q.toRotationMatrix().eulerAngles(2, 1, 0);  // Convert quaternion to rotation matrix, then to Euler angles (ZYX order = yaw, pitch, roll)
    eulerAngles *= 180.0f / M_PI;  // Convert radians to degrees for easier interpretation

    pitch = eulerAngles[0];
    roll = eulerAngles[1];
    yaw = eulerAngles[2];
}