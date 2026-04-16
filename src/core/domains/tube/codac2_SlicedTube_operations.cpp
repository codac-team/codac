/** 
 *  codac2_SlicedTube_operations.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SlicedTube_operations.h"
#include "codac2_math.h"

namespace codac2
{
  SlicedTube<Interval> sqr(const SlicedTube<Interval>& x1) {
    return pow(x1,2);
  }

  SlicedTube<Interval> sqrt(const SlicedTube<Interval>& x1)
    macro_unary_tube(sqrt)

  SlicedTube<Interval> pow(const SlicedTube<Interval>& x1, int x2)
    macro_binary_tube_real(pow)

  SlicedTube<Interval> pow(const SlicedTube<Interval>& x1, const Interval& x2)
    macro_binary_tube_real(pow)

  //SlicedTube<Interval> root(const SlicedTube<Interval>& x1, int x2)
  //  macro_binary_tube_real(operator_root)

  SlicedTube<Interval> exp(const SlicedTube<Interval>& x1)
    macro_unary_tube(exp)

  SlicedTube<Interval> log(const SlicedTube<Interval>& x1)
    macro_unary_tube(log)

  SlicedTube<Interval> cos(const SlicedTube<Interval>& x1)
    macro_unary_tube(cos)

  SlicedTube<Interval> sin(const SlicedTube<Interval>& x1)
    macro_unary_tube(sin)

  SlicedTube<Interval> tan(const SlicedTube<Interval>& x1)
    macro_unary_tube(tan)

  SlicedTube<Interval> acos(const SlicedTube<Interval>& x1)
    macro_unary_tube(acos)

  SlicedTube<Interval> asin(const SlicedTube<Interval>& x1)
    macro_unary_tube(asin)

  SlicedTube<Interval> atan(const SlicedTube<Interval>& x1)
    macro_unary_tube(atan)

  SlicedTube<Interval> atan2(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(atan2,x1)

  SlicedTube<Interval> atan2(const SlicedTube<Interval>& x1, const Interval& x2)
    macro_binary_tube_real(atan2)

  SlicedTube<Interval> atan2(const Interval& x1, const SlicedTube<Interval>& x2)
    macro_binary_real_tube(atan2)

  SlicedTube<Interval> cosh(const SlicedTube<Interval>& x1)
    macro_unary_tube(cosh)

  SlicedTube<Interval> sinh(const SlicedTube<Interval>& x1)
    macro_unary_tube(sinh)

  SlicedTube<Interval> tanh(const SlicedTube<Interval>& x1)
    macro_unary_tube(tanh)

  SlicedTube<Interval> acosh(const SlicedTube<Interval>& x1)
    macro_unary_tube(acosh)

  SlicedTube<Interval> asinh(const SlicedTube<Interval>& x1)
    macro_unary_tube(asinh)

  SlicedTube<Interval> atanh(const SlicedTube<Interval>& x1)
    macro_unary_tube(atanh)

  SlicedTube<Interval> abs(const SlicedTube<Interval>& x1)
    macro_unary_tube(abs)

  SlicedTube<Interval> min(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(min,x1)

  SlicedTube<Interval> min(const SlicedTube<Interval>& x1, const Interval& x2)
    macro_binary_tube_real(min)

  SlicedTube<Interval> min(const Interval& x1, const SlicedTube<Interval>& x2)
    macro_binary_real_tube(min)

  SlicedTube<Interval> max(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(max,x1)

  SlicedTube<Interval> max(const SlicedTube<Interval>& x1, const Interval& x2)
    macro_binary_tube_real(max)

  SlicedTube<Interval> max(const Interval& x1, const SlicedTube<Interval>& x2)
    macro_binary_real_tube(max)

  SlicedTube<Interval> sign(const SlicedTube<Interval>& x1)
    macro_unary_tube(codac2::sign)

  SlicedTube<Interval> integer(const SlicedTube<Interval>& x1)
    macro_unary_tube(codac2::integer)

  SlicedTube<Interval> floor(const SlicedTube<Interval>& x1)
    macro_unary_tube(floor)

  SlicedTube<Interval> ceil(const SlicedTube<Interval>& x1)
    macro_unary_tube(ceil)
}