#ifndef CODAC_LIB_CODAC_EIGEN_H
#define CODAC_LIB_CODAC_EIGEN_H

#include <ibex_Vector.h>
#include <ibex_Matrix.h>
#include <Eigen/Dense>


namespace codac {
class EigenHelpers {
public:
  static Eigen::MatrixXd i2e(const ibex::Matrix &x);

  static Eigen::MatrixXd i2e(const ibex::Vector &x);

  static ibex::Matrix e2i(const Eigen::MatrixXd &x);
};
}

#endif //CODAC_LIB_CODAC_EIGEN_H
