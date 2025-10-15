#pragma once

#include  <ArduinoEigenDense.h>

/*
const int GLOBAL_STATE_SIZE = 7;
const int LOCAL_STATE_SIZE = 6;
const int MATRIX_SIZE = 3;        
const float TIMESTEP = 0.1;

typedef Eigen::Vector<float, GLOBAL_STATE_SIZE> GlobalStateVector;


class Acubesat {
    private:


        // Prediction step matrix. Represents the increasing unsertainty of the model as time passes
        Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> P;
        // Noise covariance matrix. Describes unpredictable errors not taken into account in the model
        Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> Q;
        // Current satellite orientation in a Earth cetered frame of reference
        //  elements [0...3] quaternion representing the actual orientation of the satellite
        //  elements [4...6] Bias or error of each of the gyroscopes axis
        GlobalStateVector globalState;



    public:
        Eigen::Matrix<float, MATRIX_SIZE, MATRIX_SIZE> skew(const Eigen::Vector3f& vector);
        Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> stateTransitionJacobian(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements) const;
        Eigen::Quaternionf quaternionExponent(const Eigen::Vector3f& angularVelocity);
        Eigen::Quaternionf quaternionProduct(const Eigen::Quaternionf &quaternion1, const Eigen::Quaternionf &quaternion2);
        GlobalStateVector stateTransitionFunction(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements);
        void predict(const Vector3f &gyroMeasurements, const float timestep);
};





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

Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> Acubesat::stateTransitionJacobian(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements) const {
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

Eigen::Quaternionf Acubesat::quaternionProduct(const Eigen::Quaternionf &quaternion1, const Eigen::Quaternionf &quaternion2) {
    Eigen::Quaternionf quaternionProduct;

    quaternionProduct.w() = quaternion1.w() * quaternion2.w() - quaternion1.vec().dot(quaternion2.vec());
    quaternionProduct.vec() = quaternion1.w() * quaternion2.vec() + quaternion2.w() * quaternion1.vec() + quaternion1.vec().cross(quaternion2.vec());

    return quaternionProduct;
}

GlobalStateVector Acubesat::stateTransitionFunction(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements) const {
    const Eigen::Vector3f bias = state(Eigen::seq(4, 6));
    const Eigen::Vector3f angularVelocity = gyroMeasurements - bias;

    Eigen::Quaternionf currentQuaternion;
    currentQuaternion.w() = state(0);
    currentQuaternion.vec() = state(Eigen::seq(1, 3));

    Eigen::Quaternionf nextQuaternion = quaternionProduct(currentQuaternion, quaternionExponent(angularVelocity * Timestep));
    
    GlobalStateVector nextState;
    nextState(0) = nextQuaternion.w();
    nextState(Eigen::seq(1, 3)) = nextQuaternion.vec();
    nextState(Eigen::seq(4, 6)) = bias;

    return nextState;
}











void Acubesat::predict(const Vector3f &gyroMeasurements, const float timestep) {
    Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> F_k = stateTransitionJacobian(globalState, gyroMeasurements);
    globalState = stateTransitionFunction(globalState, gyroMeasurements);
    auto Phi = (F_k * timestep).exp();
    P = Phi * P * Phi.transpose() + Q;
}

*/