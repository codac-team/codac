/** 
 *  codac2_SampledTraj_operations.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SampledTraj_operations.h"
#include "codac2_math.h"

using namespace std;

namespace codac2
{
  SampledTraj<double> sqr(const SampledTraj<double>& x1) {
    return pow(x1,2);
  }

  SampledTraj<double> sqrt(const SampledTraj<double>& x1)
    macro_unary_traj(std::sqrt)

  inline double operator_pow(double x1, int x2) { return std::pow(x1,x2); }
  inline double operator_pow(double x1, double x2) { return std::pow(x1,x2); }
  inline double operator_root(double x1, double x2) { return root(x1,x2); }

  SampledTraj<double> pow(const SampledTraj<double>& x1, int x2)
    macro_binary_traj_real(operator_pow)

  SampledTraj<double> pow(const SampledTraj<double>& x1, double x2)
    macro_binary_traj_real(operator_pow)

  SampledTraj<double> root(const SampledTraj<double>& x1, int x2)
    macro_binary_traj_real(operator_root)

  SampledTraj<double> exp(const SampledTraj<double>& x1)
    macro_unary_traj(std::exp)

  SampledTraj<double> log(const SampledTraj<double>& x1)
    macro_unary_traj(std::log)

  SampledTraj<double> cos(const SampledTraj<double>& x1)
    macro_unary_traj(std::cos)

  SampledTraj<double> sin(const SampledTraj<double>& x1)
    macro_unary_traj(std::sin)

  SampledTraj<double> tan(const SampledTraj<double>& x1)
    macro_unary_traj(std::tan)

  SampledTraj<double> acos(const SampledTraj<double>& x1)
    macro_unary_traj(std::acos)

  SampledTraj<double> asin(const SampledTraj<double>& x1)
    macro_unary_traj(std::asin)

  SampledTraj<double> atan(const SampledTraj<double>& x1)
    macro_unary_traj(std::atan)

  SampledTraj<double> atan2(const SampledTraj<double>& x1, const SampledTraj<double>& x2)
    macro_binary_traj_traj(std::atan2)

  SampledTraj<double> atan2(const SampledTraj<double>& x1, double x2)
    macro_binary_traj_real(std::atan2)

  SampledTraj<double> atan2(double x1, const SampledTraj<double>& x2)
    macro_binary_real_traj(std::atan2)

  SampledTraj<double> cosh(const SampledTraj<double>& x1)
    macro_unary_traj(std::cosh)

  SampledTraj<double> sinh(const SampledTraj<double>& x1)
    macro_unary_traj(std::sinh)

  SampledTraj<double> tanh(const SampledTraj<double>& x1)
    macro_unary_traj(std::tanh)

  SampledTraj<double> acosh(const SampledTraj<double>& x1)
    macro_unary_traj(std::acosh)

  SampledTraj<double> asinh(const SampledTraj<double>& x1)
    macro_unary_traj(std::asinh)

  SampledTraj<double> atanh(const SampledTraj<double>& x1)
    macro_unary_traj(std::atanh)

  SampledTraj<double> abs(const SampledTraj<double>& x1)
    macro_unary_traj(std::abs)

  SampledTraj<double> min(const SampledTraj<double>& x1, const SampledTraj<double>& x2)
    macro_binary_traj_traj(std::min)

  SampledTraj<double> min(const SampledTraj<double>& x1, double x2)
    macro_binary_traj_real(std::min)

  SampledTraj<double> min(double x1, const SampledTraj<double>& x2)
    macro_binary_real_traj(std::min)

  SampledTraj<double> max(const SampledTraj<double>& x1, const SampledTraj<double>& x2)
    macro_binary_traj_traj(std::max)

  SampledTraj<double> max(const SampledTraj<double>& x1, double x2)
    macro_binary_traj_real(std::max)

  SampledTraj<double> max(double x1, const SampledTraj<double>& x2)
    macro_binary_real_traj(std::max)

  SampledTraj<double> sign(const SampledTraj<double>& x1)
    macro_unary_traj(codac2::sign)

  SampledTraj<double> integer(const SampledTraj<double>& x1)
    macro_unary_traj(codac2::integer)

  SampledTraj<double> floor(const SampledTraj<double>& x1)
    macro_unary_traj(std::floor)

  SampledTraj<double> ceil(const SampledTraj<double>& x1)
    macro_unary_traj(std::ceil)
}