/** 
 *  \file
 *  Directed contractors
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_DIRECTEDCTC__
#define __CODAC2_DIRECTEDCTC__

#include <string>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

namespace codac2
{
  struct CtcAdd
  {
    // Unary operations
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
    static IntervalVector fwd_eval(const IntervalVector& x1);
    static void bwd_eval(const IntervalVector& y, IntervalVector& x1);
    static IntervalMatrix fwd_eval(const IntervalMatrix& x1);
    static void bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1);

    // Binary operations
    static Interval fwd_eval(const Interval& x1, const Interval& x2);
    static void bwd_eval(const Interval& y, Interval& x1, Interval& x2);
    static IntervalVector fwd_eval(const IntervalVector& x1, const IntervalVector& x2);
    static void bwd_eval(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);
    static IntervalMatrix fwd_eval(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static void bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  struct CtcSub
  {
    // Unary operations
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
    static IntervalVector fwd_eval(const IntervalVector& x1);
    static void bwd_eval(const IntervalVector& y, IntervalVector& x1);
    static IntervalMatrix fwd_eval(const IntervalMatrix& x1);
    static void bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1);

    // Binary operations
    static Interval fwd_eval(const Interval& x1, const Interval& x2);
    static void bwd_eval(const Interval& y, Interval& x1, Interval& x2);
    static IntervalVector fwd_eval(const IntervalVector& x1, const IntervalVector& x2);
    static void bwd_eval(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);
    static IntervalMatrix fwd_eval(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static void bwd_eval(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  struct CtcMul
  {
    static Interval fwd_eval(const Interval& x1, const Interval& x2);
    static void bwd_eval(const Interval& y, Interval& x1, Interval& x2);
    static IntervalVector fwd_eval(const IntervalMatrix& x1, const IntervalVector& x2);
    static void bwd_eval(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2);
  };

  struct CtcSqrt
  {
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
  };

  struct CtcSqr
  {
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
  };

  struct CtcCos
  {
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
  };

  struct CtcSin
  {
    static Interval fwd_eval(const Interval& x1);
    static void bwd_eval(const Interval& y, Interval& x1);
  };

  struct CtcComponent
  {
    static Interval fwd_eval(const IntervalVector& x1, size_t i);
    static void bwd_eval(const Interval& y, IntervalVector& x1, size_t i);
  };

  struct CtcVector
  {
    template<typename... X>
    static IntervalVector fwd_eval(const X&... x)
    {
      IntervalVector iv(sizeof...(X));
      size_t i = 0;
      ((iv[i++] &= x), ...);
      return iv;
    }

    template<typename... X>
    static void bwd_eval(const IntervalVector& y, X&... x)
    {
      size_t i = 0;
      ((x &= y[i++]), ...);
    }
  };

  struct CtcMatrix
  {
    static void fwd_eval_i(IntervalMatrix& m, const IntervalVector& x, size_t i);

    template<typename... X>
    static IntervalMatrix fwd_eval(const X&... x)
    {
      IntervalMatrix m(1, sizeof...(X));
      size_t i = 0;
      (CtcMatrix::fwd_eval_i(m, x, i++), ...);
      return m;
    }

    template<typename... X>
    static void bwd_eval(const IntervalMatrix& y, X&... x)
    {
      size_t i = 0;
      ((x &= y.col(i++)), ...);
    }
  };
}

#endif