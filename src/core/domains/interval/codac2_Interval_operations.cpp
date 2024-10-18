/** 
 *  codac2_Interval_operations.cpp
 *  
 *  This class reuses several functions developed for ibex::Interval. 
 *  The original IBEX code is encapsulated in Codac for allowing inheritance 
 *  to Codac classes and also for documentation, binding, and independency purposes.
 *  See ibex::Interval (IBEX lib, main author: Gilles Chabert)
 *    https://ibex-lib.readthedocs.io
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_Interval_operations.h"

using namespace std;

namespace codac2
{
  const Interval& operator+(const Interval& x)
  {
    return x;
  }

  #define interval_arithm_op(f) \
    Interval f(const Interval& x, const Interval& y) \
    { \
      return ibex::f(x, y); \
    } \
    Interval f(double x, const Interval& y) \
    { \
      return ibex::f(x, y); \
    } \
    Interval f(const Interval& x, double y) \
    { \
      return ibex::f(x, y); \
    } \

  interval_arithm_op(operator&)
  interval_arithm_op(operator|)
  interval_arithm_op(operator+)
  interval_arithm_op(operator-)
  interval_arithm_op(operator*)
  interval_arithm_op(operator/)

  #define unary_interval_op(f) \
    Interval f(const Interval& x) \
    { \
      return ibex::f(x); \
    } \

  #define binary_interval_op(f) \
    Interval f(const Interval& x, const Interval& y) \
    { \
      return ibex::f(x, y); \
    } \

  unary_interval_op(sqr)
  unary_interval_op(sqrt)

  Interval pow(const Interval& x, int n)
  {
    return ibex::pow(x, n);
  }

  Interval pow(const Interval& x, double d)
  {
    return ibex::pow(x, d);
  }

  binary_interval_op(pow)

  Interval root(const Interval& x, int n)
  {
    return ibex::root(x, n);
  }

  unary_interval_op(exp)
  unary_interval_op(log)
  unary_interval_op(cos)
  unary_interval_op(sin)
  unary_interval_op(tan)
  unary_interval_op(acos)
  unary_interval_op(asin)
  unary_interval_op(atan)
  binary_interval_op(atan2)
  unary_interval_op(cosh)
  unary_interval_op(sinh)
  unary_interval_op(tanh)
  unary_interval_op(acosh)
  unary_interval_op(asinh)
  unary_interval_op(atanh)
  unary_interval_op(abs)
  binary_interval_op(min)
  binary_interval_op(max)
  unary_interval_op(sign)
  unary_interval_op(integer)
  unary_interval_op(floor)
  unary_interval_op(ceil)

  void bwd_add(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_add(y,x1,x2);
  }

  void bwd_sub(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_sub(y,x1,x2);
  }

  void bwd_mul(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_mul(y,x1,x2);
  }

  void bwd_div(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_div(y,x1,x2);
  }

  void bwd_sqr(const Interval& y, Interval& x)
  {
    ibex::bwd_sqr(y,x);
  }

  void bwd_sqrt(const Interval& y, Interval& x)
  {
    ibex::bwd_sqrt(y,x);
  }

  void bwd_pow(const Interval& y, Interval& x, int p)
  {
    ibex::bwd_pow(y,p,x);
  }

  void bwd_pow(const Interval& y, Interval& x, Interval& p)
  {
    assert(p.is_degenerated() && "bwd_power(y,x1,x2) (with x1 and x2 intervals) not implemented yet with Gaol");
    ibex::bwd_pow(y,p.mid(),x);
  }

  void bwd_root(const Interval& y, Interval& x, int p)
  {
    ibex::bwd_root(y,p,x);
  }

  void bwd_exp(const Interval& y, Interval& x)
  {
    ibex::bwd_exp(y,x);
  }

  void bwd_log(const Interval& y, Interval& x)
  {
    ibex::bwd_log(y,x);
  }

  void bwd_cos(const Interval& y, Interval& x)
  {
    ibex::bwd_cos(y,x);
  }

  void bwd_sin(const Interval& y, Interval& x)
  {
    ibex::bwd_sin(y,x);
  }

  void bwd_tan(const Interval& y, Interval& x)
  {
    ibex::bwd_tan(y,x);
  }

  void bwd_acos(const Interval& y, Interval& x)
  {
    ibex::bwd_acos(y,x);
  }

  void bwd_asin(const Interval& y, Interval& x)
  {
    ibex::bwd_asin(y,x);
  }

  void bwd_atan(const Interval& y, Interval& x)
  {
    ibex::bwd_atan(y,x);
  }

  void bwd_atan2(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_atan2(y,x1,x2);
  }

  void bwd_cosh(const Interval& y, Interval& x)
  {
    ibex::bwd_cosh(y,x);
  }

  void bwd_sinh(const Interval& y, Interval& x)
  {
    ibex::bwd_sinh(y,x);
  }

  void bwd_tanh(const Interval& y, Interval& x)
  {
    ibex::bwd_tanh(y,x);
  }

  void bwd_acosh(const Interval& y, Interval& x)
  {
    ibex::bwd_acosh(y,x);
  }

  void bwd_asinh(const Interval& y, Interval& x)
  {
    ibex::bwd_asinh(y,x);
  }

  void bwd_atanh(const Interval& y, Interval& x)
  {
    ibex::bwd_atanh(y,x);
  }

  void bwd_abs(const Interval& y, Interval& x)
  {
    ibex::bwd_abs(y,x);
  }

  void bwd_min(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_min(y,x1,x2);
  }

  void bwd_max(const Interval& y, Interval& x1, Interval& x2)
  {
    ibex::bwd_max(y,x1,x2);
  }

  void bwd_sign(const Interval& y, Interval& x)
  {
    ibex::bwd_sign(y,x);
  }

  void bwd_floor(const Interval& y, Interval& x)
  {
    ibex::bwd_floor(y,x);
  }

  void bwd_ceil(const Interval& y, Interval& x)
  {
    ibex::bwd_ceil(y,x);
  }

  void bwd_saw(const Interval& y, Interval& x)
  {
    ibex::bwd_saw(y,x);
  }

  void bwd_imod(Interval& x, Interval& y, double p)
  {
    ibex::bwd_imod(x,y,p);
  }
} // namespace codac