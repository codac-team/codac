/** 
 *  \file codac2_SampledTraj_operations.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_SampledTraj.h"
#include "codac2_math.h"

namespace codac2
{
  #define macro_unary_traj(f) \
  { \
    auto y = x1; \
    for(auto it = y.begin() ; it != y.end() ; it++) \
      it->second = f(it->second); \
    return y; \
  }; \

  #define macro_binary_traj_traj(f) \
  { \
    assert_release(x1.nb_samples() == x2.nb_samples()); \
    auto y = x2; \
    auto it_y = y.begin(); \
    auto it_x1 = x1.begin(); \
    while(it_y != y.end()) \
    { \
      assert_release(it_y->first == it_x1->first \
        && "inconsistent dates between the two trajectories"); \
      it_y->second = f(it_x1->second,it_y->second); \
      it_y++; it_x1++; \
    } \
    return y; \
  } \

  #define macro_binary_real_traj(f) \
  { \
    auto y = x2; \
    for(auto it_y = y.begin() ; it_y != y.end() ; it_y++) \
      it_y->second = f(x1,it_y->second); \
    return y; \
  } \

  #define macro_binary_traj_real(f) \
  { \
    auto y = x1; \
    for(auto it_y = y.begin() ; it_y != y.end() ; it_y++) \
      it_y->second = f(it_y->second,x2); \
    return y; \
  } \

  #define macro_member_binary_traj_traj(f) \
  { \
    assert_release(x1.nb_samples() == x2.nb_samples()); \
    auto it_x1 = x1.begin(); \
    auto it_x2 = x2.begin(); \
    while(it_x1 != x1.end()) \
    { \
      assert_release(it_x1->first == it_x2->first \
        && "inconsistent dates between the two trajectories"); \
      it_x1->second = f(it_x1->second,it_x2->second); \
      it_x1++; it_x2++; \
    } \
    return x1; \
  } \

  #define macro_member_binary_traj_real(f) \
  { \
    for(auto it_x1 = x1.begin() ; it_x1 != x1.end() ; it_x1++) \
      it_x1->second = f(it_x1->second,x2); \
    return x1; \
  } \

  template<typename T>
  inline T operator_add(const T& x1, const T& x2) { return x1 + x2; }
  template<typename T>
  inline T operator_sub(const T& x1, const T& x2) { return x1 - x2; }
  template<typename T>
  inline T operator_mul(const T& x1, const T& x2) { return x1 * x2; }
  template<typename T>
  inline T operator_mul_scal(double x1, const T& x2) { return x1 * x2; }
  template<typename T>
  inline T operator_mul_scal(const T& x1, double x2) { return x1 * x2; }
  inline Matrix operator_mul_vec(const Matrix& x1, const Vector& x2) { return x1 * x2; }
  template<typename T>
  inline T operator_div(const T& x1, const T& x2) { return x1 / x2; }
  template<typename T>
  inline T operator_div_scal(const T& x1, double x2) { return x1 / x2; }

  /** \brief \f$x1(\cdot)\f$
    * \param x1
    * \return trajectory output
    */
  template<typename T>
  inline const SampledTraj<T>& operator+(const SampledTraj<T>& x1) {
    return x1;
  }

  /** \brief \f$x_1(\cdot)+x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator+(const SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_binary_traj_traj(operator_add);

  /** \brief \f$x_1(\cdot)+x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator+(const SampledTraj<T>& x1, const T& x2)
    macro_binary_traj_real(operator_add);

  /** \brief \f$x+x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator+(const T& x1, const SampledTraj<T>& x2)
    macro_binary_real_traj(operator_add);

  /**
   * \brief Operates +=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator+=(SampledTraj<T>& x1, const T& x2)
    macro_member_binary_traj_real(operator_add);

  /**
   * \brief Operates +=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator+=(SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_member_binary_traj_traj(operator_add);

  /** \brief \f$-x_1(\cdot)\f$
    * \param x1
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator-(const SampledTraj<T>& x1) {
    return -1.*x1;
  }

  /** \brief \f$x_1(\cdot)-x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator-(const SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_binary_traj_traj(operator_sub);

  /** \brief \f$x_1(\cdot)-x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator-(const SampledTraj<T>& x1, const T& x2)
    macro_binary_traj_real(operator_sub);

  /** \brief \f$x-x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator-(const T& x1, const SampledTraj<T>& x2)
    macro_binary_real_traj(operator_sub);

  /**
   * \brief Operates -=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator-=(SampledTraj<T>& x1, const T& x2)
    macro_member_binary_traj_real(operator_sub);

  /**
   * \brief Operates -=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator-=(SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_member_binary_traj_traj(operator_sub);

  /** \brief \f$x_1\cdot x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
    requires (!std::is_same_v<T,double>)
  inline SampledTraj<T> operator*(double x1, const SampledTraj<T>& x2)
    macro_binary_real_traj(operator_mul_scal);

  /** \brief \f$x_1(\cdot)\cdot x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
    requires (!std::is_same_v<T,double>)
  inline SampledTraj<T> operator*(const SampledTraj<T>& x1, double x2)
    macro_binary_traj_real(operator_mul_scal);

  /** \brief \f$x_1(\cdot)\cdot x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator*(const SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_binary_traj_traj(operator_mul);

  /** \brief \f$x_1(\cdot)\cdot x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator*(const SampledTraj<T>& x1, const T& x2)
    macro_binary_traj_real(operator_mul);

  /** \brief \f$x\cdot x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator*(const T& x1, const SampledTraj<T>& x2)
    macro_binary_real_traj(operator_mul);

  /** \brief \f$x_1(\cdot)\cdot x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  inline SampledTraj<Vector> operator*(const SampledTraj<Matrix>& x1, const SampledTraj<Vector>& x2)
    macro_binary_traj_traj(operator_mul_vec);

  /**
   * \brief Operates *=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator*=(SampledTraj<T>& x1, const T& x2)
    macro_member_binary_traj_real(operator_mul);

  /**
   * \brief Operates *=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator*=(SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_member_binary_traj_traj(operator_mul);

  /** \brief \f$x_2(\cdot)/x_1\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
    requires (!std::is_same_v<T,double>)
  inline SampledTraj<T> operator/(const SampledTraj<T>& x1, double x2)
    macro_binary_traj_real(operator_div_scal);

  /** \brief \f$x_1(\cdot)/x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator/(const SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_binary_traj_traj(operator_div);

  /** \brief \f$x_1(\cdot)/x_2\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator/(const SampledTraj<T>& x1, const T& x2)
    macro_binary_traj_real(operator_div);

  /** \brief \f$x/x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  template<typename T>
  inline SampledTraj<T> operator/(const T& x1, const SampledTraj<T>& x2)
    macro_binary_real_traj(operator_div);

  /**
   * \brief Operates /=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator/=(SampledTraj<T>& x1, const T& x2)
    macro_member_binary_traj_real(operator_div);

  /**
   * \brief Operates /=
   * \param x1
   * \param x2
   * \return updated output
   */
  template<typename T>
  inline SampledTraj<T>& operator/=(SampledTraj<T>& x1, const SampledTraj<T>& x2)
    macro_member_binary_traj_traj(operator_div);

  /** \brief \f$x^2(\cdot)\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> sqr(const SampledTraj<double>& x1);

  /** \brief \f$\sqrt{x_1(\cdot)}\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> sqrt(const SampledTraj<double>& x1);

  /** \brief \f$x^x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> pow(const SampledTraj<double>& x1, int x2);

  /** \brief \f$x^x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> pow(const SampledTraj<double>& x1, double x2);

  /** \brief \f$x^x_2(\cdot)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> root(const SampledTraj<double>& x1, int x2);

  /** \brief \f$\exp(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> exp(const SampledTraj<double>& x1);

  /** \brief \f$\log(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> log(const SampledTraj<double>& x1);

  /** \brief \f$\cos(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> cos(const SampledTraj<double>& x1);

  /** \brief \f$\sin(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> sin(const SampledTraj<double>& x1);

  /** \brief \f$\tan(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> tan(const SampledTraj<double>& x1);

  /** \brief \f$\arccos(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> acos(const SampledTraj<double>& x1);

  /** \brief \f$\arcsin(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> asin(const SampledTraj<double>& x1);

  /** \brief \f$\arctan(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> atan(const SampledTraj<double>& x1);

  /** \brief \f$\mathrm{arctan2}(x_1(\cdot),x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> atan2(const SampledTraj<double>& x1, const SampledTraj<double>& x2);

  /** \brief \f$\mathrm{arctan2}(x_1(\cdot),x_2)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> atan2(const SampledTraj<double>& x1, double x2);

  /** \brief \f$\mathrm{arctan2}(x_1, x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> atan2(double x1, const SampledTraj<double>& x2);

  /** \brief \f$\cosh(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> cosh(const SampledTraj<double>& x1);

  /** \brief \f$\sinh(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> sinh(const SampledTraj<double>& x1);

  /** \brief \f$\tanh(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> tanh(const SampledTraj<double>& x1);

  /** \brief \f$\mathrm{arccosh}(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> acosh(const SampledTraj<double>& x1);

  /** \brief \f$\mathrm{arcsinh}(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> asinh(const SampledTraj<double>& x1);

  /** \brief \f$\mathrm{arctanh}(x_1(\cdot))\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> atanh(const SampledTraj<double>& x1);

  /** \brief \f$\mid x_1(\cdot)\mid\f$
    * \param x1
    * \return trajectory output
    */
  SampledTraj<double> abs(const SampledTraj<double>& x1);

  /** \brief \f$\min(x_1(\cdot),x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> min(const SampledTraj<double>& x1, const SampledTraj<double>& x2);

  /** \brief \f$\min(x_1(\cdot),x_2)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> min(const SampledTraj<double>& x1, double x2);

  /** \brief \f$\min(x_1, x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> min(double x1, const SampledTraj<double>& x2);

  /** \brief \f$\max(x_1(\cdot),x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> max(const SampledTraj<double>& x1, const SampledTraj<double>& x2);

  /** \brief \f$\max(x_1(\cdot),x_2)\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> max(const SampledTraj<double>& x1, double x2);

  /** \brief \f$\max(x_1, x_2(\cdot))\f$
    * \param x1
    * \param x2
    * \return trajectory output
    */
  SampledTraj<double> max(double x1, const SampledTraj<double>& x2);

  SampledTraj<double> sign(const SampledTraj<double>& x1);

  SampledTraj<double> integer(const SampledTraj<double>& x1);

  SampledTraj<double> floor(const SampledTraj<double>& x1);

  SampledTraj<double> ceil(const SampledTraj<double>& x1);
}