/* ============================================================================
 *  tubex-lib - Arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBEX_ARITHMETIC
#define TUBEX_ARITHMETIC

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_TubeSlice.h"

namespace tubex
{
  /* Unary operations */

    #define header_unary_op(f) \
      Tube f(const Tube& x);

    header_unary_op(cos);
    header_unary_op(sin);
    header_unary_op(abs);
    header_unary_op(sqr);
    header_unary_op(sqrt);
    header_unary_op(exp);
    header_unary_op(log);
    header_unary_op(tan);
    header_unary_op(acos);
    header_unary_op(asin);
    header_unary_op(atan);
    header_unary_op(cosh);
    header_unary_op(sinh);
    header_unary_op(tanh);
    header_unary_op(acosh);
    header_unary_op(asinh);
    header_unary_op(atanh);
    Tube operator+(const Tube& x);
    Tube operator-(const Tube& x);

  /* Unary operations with parameter */
  
    #define header_unary_param_op(f, p) \
      Tube f(const Tube& x1, p param);

    header_unary_param_op(pow, int);
    header_unary_param_op(pow, double);
    header_unary_param_op(pow, const ibex::Interval&);
    header_unary_param_op(root, int);

  /* Binary operations */
  
    #define header_binary_op(f) \
      Tube f(const Tube& tube_x1, const Tube& tube_x2); \
      Tube f(const ibex::Interval& x1, const Tube& tube_x2); \
      Tube f(const Tube& tube_x1, const ibex::Interval& x2); \
      Tube f(const Trajectory& x1, const Tube& tube_x2); \
      Tube f(const Tube& tube_x1, const Trajectory& x2);

    header_binary_op(atan2);
    header_binary_op(operator+);
    header_binary_op(operator-);
    header_binary_op(operator*);
    header_binary_op(operator/);
    header_binary_op(operator|);
    header_binary_op(operator&);







  /*Subtube operator+(const Subtube& x);
  Subtube operator+(const Subtube& x1, const Subtube& x2);
  Subtube operator+(const Subtube& x1, double x2);
  Subtube operator+(double x1, const Subtube& x2);
  Subtube operator+(const Subtube& x1, const ibex::Interval& x2);
  Subtube operator+(const ibex::Interval& x1, const Subtube& x2);

  Subtube operator-(const Subtube& x);
  Subtube operator-(const Subtube& x1, const Subtube& x2);
  Subtube operator-(const Subtube& x1, double x2);
  Subtube operator-(double x1, const Subtube& x2);
  Subtube operator-(const Subtube& x1,  const ibex::Interval& x2);
  Subtube operator-(const ibex::Interval& x1, const Subtube& x2);

  Subtube operator*(const Subtube& x1, const Subtube& x2);
  Subtube operator*(const Subtube& x1, double x2);
  Subtube operator*(double x1, const Subtube& x2);
  Subtube operator*(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator*(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator/(const Subtube& x1, const Subtube& x2);
  Subtube operator/(const Subtube& x1, double x2);
  Subtube operator/(double x1, const Subtube& x2);
  Subtube operator/(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator/(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator|(const Subtube& x1, const Subtube& x2);
  Subtube operator|(const Subtube& x1, double x2);
  Subtube operator|(double x1, const Subtube& x2);
  Subtube operator|(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator|(const Subtube& x1, const ibex::Interval& x2);

  Subtube operator&(const Subtube& x1, const Subtube& x2);
  Subtube operator&(const Subtube& x1, double x2);
  Subtube operator&(double x1, const Subtube& x2);
  Subtube operator&(const ibex::Interval& x1, const Subtube& x2);
  Subtube operator&(const Subtube& x1, const ibex::Interval& x2);

  Subtube abs(const Subtube& x);
  Subtube sqr(const Subtube& x);
  Subtube sqrt(const Subtube& x);
  Subtube pow(const Subtube& x, int p);
  Subtube pow(const Subtube& x, double p);
  Subtube pow(const Subtube &x, const ibex::Interval& p);
  Subtube root(const Subtube& x, int p);
  Subtube exp(const Subtube& x);
  Subtube log(const Subtube& x);
  Subtube cos(const Subtube& x);
  Subtube sin(const Subtube& x);
  Subtube tan(const Subtube& x);
  Subtube acos(const Subtube& x);
  Subtube asin(const Subtube& x);
  Subtube atan(const Subtube& x);
  Subtube cosh(const Subtube& x);
  Subtube sinh(const Subtube& x);
  Subtube tanh(const Subtube& x);
  Subtube acosh(const Subtube& x);
  Subtube asinh(const Subtube& x);
  Subtube atanh(const Subtube& x);
  Subtube atan2(const Subtube& y, const Subtube& x);*/
}

#endif