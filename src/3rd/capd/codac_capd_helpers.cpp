#include "codac_capd_helpers.h"

namespace codac {
Interval CapdHelpers::c2i_i(const capd::interval &x) {
  return Interval(x.leftBound(), x.rightBound());
}

IntervalVector CapdHelpers::c2i_v(const capd::IVector &x) {
  IntervalVector y(x.dimension());
  for (uint i = 0; i < x.dimension(); ++i) y[i] = c2i_i(x[i]);
  return y;
}

IntervalMatrix CapdHelpers::c2i_m(const capd::IMatrix &x) {
  IntervalMatrix M(x.numberOfRows(), x.numberOfColumns());
  for (uint i = 0; i < x.numberOfRows(); ++i) {
    for (uint j = 0; j < x.numberOfColumns(); ++j) {
      M[i][j] = c2i_i(x[i][j]);
    }
  }
  return M;
}

capd::Interval CapdHelpers::i2c_i(const Interval &x) {
  return capd::Interval(x.lb(), x.ub());
}

capd::IVector CapdHelpers::i2c_v(const IntervalVector &x) {
  capd::IVector y(x.size());
  for (int i = 0; i < x.size(); ++i) y[i] = i2c_i(x[i]);
  return y;
}

capd::IVector CapdHelpers::i2c_v_mid(const IntervalVector &x) {
  capd::IVector y(x.size());
  for (int i = 0; i < x.size(); ++i) y[i] = i2c_i(x[i]).mid();
  return y;
}

capd::IMatrix CapdHelpers::i2c_m(const IntervalMatrix &x) {
  capd::IMatrix M(x.nb_rows(), x.nb_cols());
  for (int i = 0; i < x.nb_rows(); ++i) {
    for (int j = 0; j < x.nb_cols(); ++j) {
      M[i][j] = i2c_i(x[i][j]);
    }
  }
  return M;
}

capd::IMatrix CapdHelpers::i2c_m_mid(const IntervalMatrix &x) {
  capd::IMatrix M(x.nb_rows(), x.nb_cols());
  for (int i = 0; i < x.nb_rows(); ++i) {
    for (int j = 0; j < x.nb_cols(); ++j) {
      M[i][j] = i2c_i(x[i][j]).mid();
    }
  }
  return M;
}

}