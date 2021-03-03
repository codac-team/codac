#ifndef CODAC_LIB_CODAC_EIGEN_H
#define CODAC_LIB_CODAC_EIGEN_H

#include <codac_Vector.h>
#include <codac_Matrix.h>
#include <Eigen/Dense>


namespace codac {
class EigenHelpers {
public:
  static Eigen::MatrixXd i2e(const Matrix &x);

  static Eigen::MatrixXd i2e(const Vector &x);

  static Matrix e2i(const Eigen::MatrixXd &x);
};
}

#endif //CODAC_LIB_CODAC_EIGEN_H
