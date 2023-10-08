#include "codac_Eigen.h"


namespace codac {
Eigen::MatrixXd EigenHelpers::i2e(const Matrix &x) {
  Eigen::MatrixXd m(x.nb_rows(), x.nb_cols());
  for (int i = 0; i < x.nb_rows(); ++i) {
    for (int j = 0; j < x.nb_cols(); ++j) {
      m(i, j) = x[i][j];
    }
  }
  return m;
}

Eigen::MatrixXd EigenHelpers::i2e(const Vector &x) {
  Eigen::MatrixXd m(x.size(), 1);
  for (int i = 0; i < x.size(); ++i) {
    m(i, 0) = x[i];
  }
  return m;
}

Matrix EigenHelpers::e2i(const Eigen::MatrixXd &x) {
  Matrix m(x.rows(), x.cols());
  for (unsigned int i = 0; i < x.rows(); ++i) {
    for (unsigned int j = 0; j < x.cols(); ++j) {
      m[i][j] = x(i, j);
    }
  }
  return m;
}
}