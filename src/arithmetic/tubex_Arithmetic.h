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

#ifndef __TUBEX_ARITHMETIC_H__
#define __TUBEX_ARITHMETIC_H__

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_TubeSlice.h"

namespace tubex
{
  /* Unary operations */

    #define header_unary_op(f) \
      const Tube f(const Tube& x);

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
    const TubeVector operator+(const TubeVector& x);
    const TubeVector operator-(const TubeVector& x);
    const Tube operator-(const Tube& x);

  /* Unary operations with parameter */
  
    #define header_unary_param_op(f, p) \
      const Tube f(const Tube& x1, p param);

    header_unary_param_op(pow, int);
    header_unary_param_op(pow, double);
    header_unary_param_op(pow, const ibex::Interval&);
    header_unary_param_op(root, int);

  /* Binary operations */
  
    #define header_binary_op_scalar_commutative_types(f) \
      const Tube f(const Tube& tube_x1, const Tube& tube_x2); \
      const Tube f(const Tube& tube_x1, const ibex::Interval& x2); \
      const Tube f(const ibex::Interval& x1, const Tube& tube_x2); \
      const Tube f(const Tube& tube_x1, const Trajectory& x2); \
      const Tube f(const Trajectory& x1, const Tube& tube_x2);
  
    header_binary_op_scalar_commutative_types(operator+);
    header_binary_op_scalar_commutative_types(operator-);
    header_binary_op_scalar_commutative_types(operator*);
    header_binary_op_scalar_commutative_types(atan2);
  
    #define header_binary_op_scalar_noncommutative_types(f) \
      const Tube f(const Tube& tube_x1, const Tube& tube_x2); \
      const Tube f(const Tube& tube_x1, const ibex::Interval& x2); \
      const Tube f(const Tube& tube_x1, const Trajectory& x2);
  
    header_binary_op_scalar_noncommutative_types(operator/);

    #define header_binary_op(f) \
      const TubeVector f(const TubeVector& tube_x1, const TubeVector& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const ibex::IntervalVector& x2); \
      const TubeVector f(const ibex::IntervalVector& x1, const TubeVector& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const TrajectoryVector& x2); \
      const TubeVector f(const TrajectoryVector& x1, const TubeVector& tube_x2);

    header_binary_op(operator+);
    header_binary_op(operator-);
    header_binary_op(operator|);
    header_binary_op(operator&);

    #define header_binary_op_with_scalar_commutative_types(f) \
      const TubeVector f(const Tube& tube_x1, const TubeVector& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const Tube& tube_x2); \
      const TubeVector f(const ibex::Interval& x1, const TubeVector& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const ibex::Interval& x2); \
      const TubeVector f(const Trajectory& x1, const TubeVector& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const Trajectory& x2);

    header_binary_op_with_scalar_commutative_types(operator+);
    header_binary_op_with_scalar_commutative_types(operator*);
    header_binary_op_with_scalar_commutative_types(operator-);
  
    #define header_binary_op_with_scalar_noncommutative_types(f) \
      const TubeVector f(const TubeVector& tube_x1, const Tube& tube_x2); \
      const TubeVector f(const TubeVector& tube_x1, const ibex::Interval& x2); \
      const TubeVector f(const TubeVector& tube_x1, const Trajectory& x2);

    header_binary_op_with_scalar_noncommutative_types(operator/);
}

#endif