#include "QUEST.hpp"

#include "../../lib/math/quaternion/quaternion.hpp"
#include "../../lib/math/matrix/matrix.hpp"
#include "../../lib/math/constants.hpp"

#include <cmath>


// Creates a new normalized Matrix from a 3x1 vector Does not alter the input.
math::Matrix<3, 1> normalizeVector(const math::Matrix<3, 1>& inputVector) {
    const double magnitude = std::sqrt(std::pow(inputVector.at(0, 0), 2) + std::pow(inputVector.at(1, 0), 2) + std::pow(inputVector.at(2, 0), 2));
    
    // change function so there is only one return?
    if (magnitude < math::Constants::doubleComparisonThreshold) {return inputVector;} // Return as is if too small to normalize
    return inputVector.multiply(1.0f / magnitude);
}

// Computes the Outer Product (3x1 * 1x3 = 3x3). Does not alter inputs
math::Matrix<3, 3> computeOuterProduct(const math::Matrix<3, 1>& bodyVector, const math::Matrix<3, 1>& referenceVector) {
    math::Matrix<3, 3> resultMatrix;
    for(std::size_t row = 0; row < 3; ++row) {
        for(std::size_t column = 0; column < 3; ++column) {
            resultMatrix.at(row, column) = bodyVector.at(row, 0) * referenceVector.at(column, 0);
        }
    }
    return resultMatrix;
}

// Calculates orientation between two sets of vectors.
math::Quaternion QUEST(const math::Vector3d accelerometerBody, 
                       const math::Vector3d& gravityReference, 
                       const math::Vector3d& magnetometerBody, 
                       const math::Vector3d& magneticNorthReference, 
                       const double accelerometerWeight, 
                       const double magnetometerWeight) {
    
    // Convert inputs to Matrix form
    math::Matrix<3, 1> accelBodyMat; 
    accelBodyMat.at(0,0) = accelerometerBody.x; accelBodyMat.at(1,0) = accelerometerBody.y; accelBodyMat.at(2,0) = accelerometerBody.z;
    
    math::Matrix<3, 1> gravityRefMat; 
    gravityRefMat.at(0,0) = gravityReference.x; gravityRefMat.at(1,0) = gravityReference.y; gravityRefMat.at(2,0) = gravityReference.z;
    
    math::Matrix<3, 1> magBodyMat; 
    magBodyMat.at(0,0) = magnetometerBody.x; magBodyMat.at(1,0) = magnetometerBody.y; magBodyMat.at(2,0) = magnetometerBody.z;
    
    math::Matrix<3, 1> magNorthRefMat; 
    magNorthRefMat.at(0,0) = magneticNorthReference.x; magNorthRefMat.at(1,0) = magneticNorthReference.y; magNorthRefMat.at(2,0) = magneticNorthReference.z;

    // Normalize observations
    const math::Matrix<3, 1> unitAccelBody = normalizeVector(accelBodyMat);
    const math::Matrix<3, 1> unitGravityRef = normalizeVector(gravityRefMat);
    const math::Matrix<3, 1> unitMagBody   = normalizeVector(magBodyMat);
    const math::Matrix<3, 1> unitMagRef    = normalizeVector(magNorthRefMat);

    // Build the Attitude Profile Matrix B
    const math::Matrix<3, 3> profileMatrixB = computeOuterProduct(unitAccelBody, unitGravityRef).multiply(accelerometerWeight) + computeOuterProduct(unitMagBody, unitMagRef).multiply(magnetometerWeight);

    // Scalar constants for the Davenport M matrix
    const double matrixTrace = profileMatrixB.at(0, 0) + profileMatrixB.at(1, 1) + profileMatrixB.at(2, 2);
    const double optimalEigenvalue = accelerometerWeight + magnetometerWeight;
    const double alphaFactor = optimalEigenvalue + matrixTrace;
    
    const double zVector[3] = {
        profileMatrixB.at(1, 2) - profileMatrixB.at(2, 1),
        profileMatrixB.at(2, 0) - profileMatrixB.at(0, 2),
        profileMatrixB.at(0, 1) - profileMatrixB.at(1, 0)
    };

    // Construct the Davenport M matrix
    math::Matrix<3, 3> davenportM;
    for(std::size_t i = 0; i < 3; ++i) {
        for(std::size_t j = 0; j < 3; ++j) {
            const double sumSymmetric = profileMatrixB.at(i, j) + profileMatrixB.at(j, i);
            davenportM.at(i, j) = (i == j) ? (alphaFactor - sumSymmetric) : -sumSymmetric;
        }
    }

    // Solve for the vector part of the quaternion using Cramer's Rule
    const double davenportDeterminant = 
        davenportM.at(0, 0) * (davenportM.at(1, 1) * davenportM.at(2, 2) - davenportM.at(1, 2) * davenportM.at(2, 1)) -
        davenportM.at(0, 1) * (davenportM.at(1, 0) * davenportM.at(2, 2) - davenportM.at(1, 2) * davenportM.at(2, 0)) +
        davenportM.at(0, 2) * (davenportM.at(1, 0) * davenportM.at(2, 1) - davenportM.at(1, 1) * davenportM.at(2, 0));

    // change structure to have only one return?
    double qw = 1.0;
    double qx = 0.0;
    double qy = 0.0;
    double qz = 0.0;

    if (std::abs(davenportDeterminant) < 1e-6f) { // change to math::Constants::doubleComparisonThreshold
        return math::Quaternion(qw, qx, qy, qz);
    }

    // Cramer's rule to solve the system [davenportM][q_vector] = [zVector]
    qx = ((davenportM.at(1,1)*davenportM.at(2,2) - davenportM.at(1,2)*davenportM.at(2,1)) * zVector[0] + (davenportM.at(0,2)*davenportM.at(2,1) - davenportM.at(0,1)*davenportM.at(2,2)) * zVector[1] + (davenportM.at(0,1)*davenportM.at(1,2) - davenportM.at(0,2)*davenportM.at(1,1)) * zVector[2]) / davenportDeterminant;
    qy = ((davenportM.at(1,2)*davenportM.at(2,0) - davenportM.at(1,0)*davenportM.at(2,2)) * zVector[0] + (davenportM.at(0,0)*davenportM.at(2,2) - davenportM.at(0,2)*davenportM.at(2,0)) * zVector[1] + (davenportM.at(0,2)*davenportM.at(1,0) - davenportM.at(0,0)*davenportM.at(1,2)) * zVector[2]) / davenportDeterminant;
    qz = ((davenportM.at(1,0)*davenportM.at(2,1) - davenportM.at(1,1)*davenportM.at(2,0)) * zVector[0] + (davenportM.at(0,1)*davenportM.at(2,0) - davenportM.at(0,0)*davenportM.at(2,1)) * zVector[1] + (davenportM.at(0,0)*davenportM.at(1,1) - davenportM.at(0,1)*davenportM.at(1,0)) * zVector[2]) / davenportDeterminant;
    qw = 1.0f;

    const double normalizationFactor = std::sqrt(qw * qw + qx * qx + qy * qy + qz * qz);
    return math::Quaternion(qw / normalizationFactor, qx / normalizationFactor, qy / normalizationFactor, qz / normalizationFactor);
}