/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CARTPROD_H__
#define __CODAC2_CARTPROD_H__

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  IntervalVector cart_prod_dyn(const Interval& x1, const Interval& x2);
  IntervalVector_<2> cart_prod_static(const Interval& x1, const Interval& x2);
  IntervalVector cart_prod_dyn(const IntervalVector& x1, const Interval& x2);

  template<int N>
  IntervalVector_<N+1> cart_prod_static(const IntervalVector_<N>& x1, const Interval& x2)
  {
    IntervalVector_<N+1> x;
    x << x1,x2;
    return x;
  }

  IntervalVector cart_prod_dyn(const Interval& x1, const IntervalVector& x2);

  template<int N>
  IntervalVector_<N+1> cart_prod_static(const Interval& x1, const IntervalVector_<N>& x2)
  {
    IntervalVector_<N+1> x;
    x << x1,x2;
    return x;
  }

  IntervalVector cart_prod_dyn(const IntervalVector& x1, const IntervalVector& x2);

  template<int N,int M>
  IntervalVector_<N+M> cart_prod_static(const IntervalVector_<N>& x1, const IntervalVector_<M>& x2)
  {
    IntervalVector_<N+M> x;
    x << x1,x2;
    return x;
  }

  template<int N, typename T1, typename T2, typename... Args>
  IntervalVector_<N> cart_prod(const T1& x1, const T2& x2, const Args&... xi) // recursive variadic function
  {
    auto x_ = cart_prod_static(x1, x2);
    if constexpr(sizeof...(xi) > 0)
      return cart_prod<N>(x_, xi...);
    else
      return x_;
  }

  template<typename T1, typename T2, typename... Args>
  IntervalVector cart_prod(const T1& x1, const T2& x2, const Args&... xi) // recursive variadic function
  {
    IntervalVector x_ = cart_prod_dyn(x1, x2);
    if constexpr(sizeof...(xi) > 0)
      return cart_prod(x_, xi...);
    else
      return x_;
  }

} // namespace codac

#endif