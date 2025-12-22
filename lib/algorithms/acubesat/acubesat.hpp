#pragma once

#include  <ArduinoEigenDense.h>

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

        Eigen::Matrix<float, MATRIX_SIZE, MATRIX_SIZE> skew(const Eigen::Vector3f& vector);
        Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> stateTransitionJacobian(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements);
        Eigen::Quaternionf quaternionExponent(const Eigen::Vector3f& angularVelocity);
        Eigen::Quaternionf quaternionProduct(const Eigen::Quaternionf &quaternion1, const Eigen::Quaternionf &quaternion2);
        GlobalStateVector stateTransitionFunction(const GlobalStateVector& state, const Eigen::Vector3f& gyroMeasurements, const float timestep);
        Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> matrixExponential(const Eigen::Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE>& A, int terms = 10);
        void predict(const Eigen::Vector3f &gyroMeasurements, const float timestep);
        void updateEulerAngles();

    public:
        float pitch;
        float roll;
        float yaw;

        void setupForOnlyGyro();
        void predict(const float gyroscopeX, const float gyroscopeY, const float gyroscopeZ, const float timestep);
};