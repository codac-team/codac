#ifndef TUBEX_LIB_TUBEX_EIGEN_H
#define TUBEX_LIB_TUBEX_EIGEN_H

#include <ibex_Vector.h>
#include <ibex_Matrix.h>
#include <Eigen/Dense>


namespace tubex {
class EigenHelpers {
public:
  static Eigen::MatrixXd i2e(const ibex::Matrix &x);

  static Eigen::MatrixXd i2e(const ibex::Vector &x);

  static ibex::Matrix e2i(const Eigen::MatrixXd &x);
};
}

#endif //TUBEX_LIB_TUBEX_EIGEN_H
