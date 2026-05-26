#include "QUEST.hpp"

#include "../../utils/math/matrix/matrix.hpp"

#include <cmath>
#include <limits>




/**
 * Creates a new normalized Matrix from a 3x1 vector.
 * Does not alter the input.
 */
Matrix<3, 1> normalizeVector(const Matrix<3, 1>& inputVector) {
    const float magnitude = std::sqrt(std::pow(inputVector.at(0, 0), 2) + 
                                     std::pow(inputVector.at(1, 0), 2) + 
                                     std::pow(inputVector.at(2, 0), 2));
    
    if (magnitude < std::numeric_limits<float>::epsilon()) {
        return inputVector; // Return as is if too small to normalize
    }
    
    return inputVector.multiply(1.0f / magnitude);
}

/**
 * Computes the Outer Product (3x1 * 1x3 = 3x3).
 * Does not alter inputs.
 */
Matrix<3, 3> computeOuterProduct(const Matrix<3, 1>& bodyVector, const Matrix<3, 1>& referenceVector) {
    Matrix<3, 3> resultMatrix;
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t column = 0; column < 3; ++column) {
            resultMatrix.at(row, column) = bodyVector.at(row, 0) * referenceVector.at(column, 0);
        }
    }
    return resultMatrix;
}

/**
 * QUEST Algorithm
 * Calculates orientation between two sets of vectors.
 */
Quaternion QUEST(const Vector3d accelerometerBody, const Vector3d gravityReference, 
                                     const Vector3d magnetometerBody, const Vector3d magneticNorthReference, 
                                     const float accelerometerWeight, const float magnetometerWeight) {
    
    // Convert inputs to Matrix form
    Matrix<3, 1> accelBodyMat; 
    accelBodyMat.at(0,0) = accelerometerBody.x; accelBodyMat.at(1,0) = accelerometerBody.y; accelBodyMat.at(2,0) = accelerometerBody.z;
    
    Matrix<3, 1> gravityRefMat; 
    gravityRefMat.at(0,0) = gravityReference.x; gravityRefMat.at(1,0) = gravityReference.y; gravityRefMat.at(2,0) = gravityReference.z;
    
    Matrix<3, 1> magBodyMat; 
    magBodyMat.at(0,0) = magnetometerBody.x; magBodyMat.at(1,0) = magnetometerBody.y; magBodyMat.at(2,0) = magnetometerBody.z;
    
    Matrix<3, 1> magNorthRefMat; 
    magNorthRefMat.at(0,0) = magneticNorthReference.x; magNorthRefMat.at(1,0) = magneticNorthReference.y; magNorthRefMat.at(2,0) = magneticNorthReference.z;

    // Normalize observations
    const Matrix<3, 1> unitAccelBody = normalizeVector(accelBodyMat);
    const Matrix<3, 1> unitGravityRef = normalizeVector(gravityRefMat);
    const Matrix<3, 1> unitMagBody   = normalizeVector(magBodyMat);
    const Matrix<3, 1> unitMagRef    = normalizeVector(magNorthRefMat);

    // Build the Attitude Profile Matrix B
    const Matrix<3, 3> profileMatrixB = computeOuterProduct(unitAccelBody, unitGravityRef).multiply(accelerometerWeight) + computeOuterProduct(unitMagBody, unitMagRef).multiply(magnetometerWeight);

    // Scalar constants for the Davenport M matrix
    const float matrixTrace = profileMatrixB.at(0, 0) + profileMatrixB.at(1, 1) + profileMatrixB.at(2, 2);
    const float optimalEigenvalue = accelerometerWeight + magnetometerWeight;
    const float alphaFactor = optimalEigenvalue + matrixTrace;
    
    const float zVector[3] = {
        profileMatrixB.at(1, 2) - profileMatrixB.at(2, 1),
        profileMatrixB.at(2, 0) - profileMatrixB.at(0, 2),
        profileMatrixB.at(0, 1) - profileMatrixB.at(1, 0)
    };

    // Construct the Davenport M matrix
    Matrix<3, 3> davenportM;
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            const float sumSymmetric = profileMatrixB.at(i, j) + profileMatrixB.at(j, i);
            davenportM.at(i, j) = (i == j) ? (alphaFactor - sumSymmetric) : -sumSymmetric;
        }
    }

    // Solve for the vector part of the quaternion using Cramer's Rule
    const float davenportDeterminant = 
        davenportM.at(0, 0) * (davenportM.at(1, 1) * davenportM.at(2, 2) - davenportM.at(1, 2) * davenportM.at(2, 1)) -
        davenportM.at(0, 1) * (davenportM.at(1, 0) * davenportM.at(2, 2) - davenportM.at(1, 2) * davenportM.at(2, 0)) +
        davenportM.at(0, 2) * (davenportM.at(1, 0) * davenportM.at(2, 1) - davenportM.at(1, 1) * davenportM.at(2, 0));

    if (std::abs(davenportDeterminant) < 1e-6f) {
        return Quaternion{1.0f, 0.0f, 0.0f, 0.0f}; 
    }

    // Cramer's rule to solve the system [davenportM][q_vector] = [zVector]
    const float qx = ((davenportM.at(1,1)*davenportM.at(2,2) - davenportM.at(1,2)*davenportM.at(2,1)) * zVector[0] + (davenportM.at(0,2)*davenportM.at(2,1) - davenportM.at(0,1)*davenportM.at(2,2)) * zVector[1] + (davenportM.at(0,1)*davenportM.at(1,2) - davenportM.at(0,2)*davenportM.at(1,1)) * zVector[2]) / davenportDeterminant;
    const float qy = ((davenportM.at(1,2)*davenportM.at(2,0) - davenportM.at(1,0)*davenportM.at(2,2)) * zVector[0] + (davenportM.at(0,0)*davenportM.at(2,2) - davenportM.at(0,2)*davenportM.at(2,0)) * zVector[1] + (davenportM.at(0,2)*davenportM.at(1,0) - davenportM.at(0,0)*davenportM.at(1,2)) * zVector[2]) / davenportDeterminant;
    const float qz = ((davenportM.at(1,0)*davenportM.at(2,1) - davenportM.at(1,1)*davenportM.at(2,0)) * zVector[0] + (davenportM.at(0,1)*davenportM.at(2,0) - davenportM.at(0,0)*davenportM.at(2,1)) * zVector[1] + (davenportM.at(0,0)*davenportM.at(1,1) - davenportM.at(0,1)*davenportM.at(1,0)) * zVector[2]) / davenportDeterminant;
    const float qw = 1.0f;

    const float normalizationFactor = std::sqrt(qw*qw + qx*qx + qy*qy + qz*qz);
    return Quaternion{qw / normalizationFactor, qx / normalizationFactor, qy / normalizationFactor, qz / normalizationFactor};
}