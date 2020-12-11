#include "tubex_Eigen.h"


namespace tubex {
Eigen::MatrixXd EigenHelpers::i2e(const ibex::Matrix &x) {
  Eigen::MatrixXd m(x.nb_rows(), x.nb_cols());
  for (int i = 0; i < x.nb_rows(); ++i) {
    for (int j = 0; j < x.nb_cols(); ++j) {
      m(i, j) = x[i][j];
    }
  }
  return m;
}

Eigen::MatrixXd EigenHelpers::i2e(const ibex::Vector &x) {
  Eigen::MatrixXd m(x.size(), 1);
  for (int i = 0; i < x.size(); ++i) {
    m(i, 1) = x[i];
  }
  return m;
}

ibex::Matrix EigenHelpers::e2i(const Eigen::MatrixXd &x) {
  ibex::Matrix m(x.rows(), x.cols());
  for (uint i = 0; i < x.rows(); ++i) {
    for (uint j = 0; j < x.cols(); ++j) {
      m[i][j] = x(i, j);
    }
  }
  return m;
}
}