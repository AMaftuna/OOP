#pragma once

#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix_;

 public:
  Matrix() : matrix_(N, std::vector<T>(M, 0)) {}

  Matrix(std::vector<std::vector<T>>& other) { matrix_ = other; }

  Matrix(T elem) : matrix_(N, std::vector<T>(M, elem)) {}

  Matrix& operator+=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix_[i][j] += other(i, j);
      }
    }
    return *this;
  }

  Matrix operator+(const Matrix& other) {
    Matrix<N, M, T> sum = *this;
    sum += other;
    return sum;
  }

  Matrix& operator-=(const Matrix& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix_[i][j] -= other(i, j);
      }
    }
    return *this;
  }

  Matrix operator-(const Matrix& other) {
    Matrix<N, M, T> sub = *this;
    sub -= other;
    return sub;
  }

  Matrix operator*(const T kValue) {
    Matrix<N, M, T> mul = *this;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        mul(i, j) *= kValue;
      }
    }
    return mul;
  }

  template <size_t MM>
  friend Matrix<N, MM, T> operator*(const Matrix<N, M, T>& other,
                                    const Matrix<M, MM, T>& other1) {
    Matrix<N, MM, T> mul;
    for (size_t i = 0; i < N; ++i) {
      for (size_t ii = 0; ii < MM; ++ii) {
        T sum = 0;
        for (size_t j = 0; j < M; ++j) {
          sum += other(i, j) * other1(j, ii);
        }
        mul(i, ii) = sum;
      }
    }
    return mul;
  }

  Matrix<M, N, T> Transposed() {
    Matrix<M, N, T> trans;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        trans(i, j) = matrix_[j][i];
      }
    }
    return trans;
  }

  T Trace() {
    T sum = 0;
    for (size_t i = 0; i < N; ++i) {
      sum += matrix_[i][i];
    }
    return sum;
  }

  T operator()(const size_t kIndex1, const size_t kIndex2) const {
    return matrix_[kIndex1][kIndex2];
  }

  T& operator()(const size_t kIndex1, const size_t kIndex2) {
    return matrix_[kIndex1][kIndex2];
  }

  bool operator==(const Matrix& other) const { return matrix_ == other; }
};
