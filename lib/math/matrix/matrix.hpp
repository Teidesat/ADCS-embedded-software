#pragma once

#include <array>

namespace math {

template <std::size_t Rows, std::size_t Columns>
class Matrix {
    private:
        std::array<double, Rows * Columns> flattenedMatrix;

    public:
        Matrix();
        double& at(const std::size_t row, const std::size_t column);
        const double& at(const std::size_t row, const std::size_t column) const;
        Matrix<Rows, Columns> multiply(const double scalar) const;
        Matrix<Rows, Columns> operator*(const double scalar) const;
        Matrix<Rows, Columns> add(const Matrix<Rows, Columns>& matrix) const;
        Matrix<Rows, Columns> operator+(const Matrix<Rows, Columns>& matrix) const;
};


template <std::size_t Rows, std::size_t Columns>
Matrix<Rows, Columns>::Matrix() {flattenedMatrix.fill(0);}

template <std::size_t Rows, std::size_t Columns>
double& Matrix<Rows, Columns>::at(const std::size_t row, const std::size_t column) {return flattenedMatrix[column * Rows + row];}

template <std::size_t Rows, std::size_t Columns>
const double& Matrix<Rows, Columns>::at(const std::size_t row, const std::size_t column) const {return flattenedMatrix[column * Rows + row];}

template <std::size_t Rows, std::size_t Columns>
Matrix<Rows, Columns> Matrix<Rows, Columns>::multiply(const double scalar) const {
    Matrix<Rows, Columns> newMatrix;
    for(std::size_t i = 0; i < Columns; ++i) {
        for(std::size_t j = 0; j < Rows; ++j) {newMatrix.at(j, i) = at(j, i) * scalar;}
    }
    return newMatrix;
}

template <std::size_t Rows, std::size_t Columns>
Matrix<Rows, Columns> Matrix<Rows, Columns>::operator*(const double scalar) const {return multiply(scalar);}

template <std::size_t Rows, std::size_t Columns>
Matrix<Rows, Columns> Matrix<Rows, Columns>::add(const Matrix<Rows, Columns>& matrix) const {
    Matrix<Rows, Columns> newMatrix;
    for(std::size_t i = 0; i < Columns; ++i) {
        for(std::size_t j = 0; j < Rows; ++j) {newMatrix.at(j, i) = at(j, i) + matrix.at(j, i);}
    }
    return newMatrix;
}

template <std::size_t Rows, std::size_t Columns>
Matrix<Rows, Columns> Matrix<Rows, Columns>::operator+(const Matrix<Rows, Columns>& matrix) const {return add(matrix);}

}