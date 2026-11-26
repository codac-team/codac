/** 
 *  \file codac2_max.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct MaxOp
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return "max(" + x1->str() + "," + x2->str() + ")";
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  max(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MaxOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }

  inline ScalarType MaxOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline ScalarType MaxOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    assert(x1.da.rows() == 1);
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());

    IntervalMatrix d(1,x1.da.cols());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = chi(x1.a-x2.a, x2.da(0,i), x1.da(0,i));

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
        && !x1.a.intersects(x2.a) // def domain of the derivative of max
    };
  }

  // Inline functions

  inline Interval MaxOp::fwd(const Interval& x1, const Interval& x2)
  {
    return max(x1,x2);
  }

  inline void MaxOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
#if CODAC_GAOL_GUARDS
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(y.is_empty())
    {
      x1.set_empty();
      x2.set_empty();
    }

    // Disjoint intervals
    else if(x2.lb() > x1.ub() || y.lb() > x1.ub())
    {
      // Then, max(x,x2) is necessarily x2
      if((x2 &= y).is_empty())
        x1.set_empty();
    }

    else if(x1.lb() > x2.ub() || y.lb() > x2.ub())
    {
      if((x1 &= y).is_empty())
        x2.set_empty();
    }

    else if(y.ub() < x1.lb() || y.ub() < x2.lb())
    {
      x1.set_empty();
      x2.set_empty();
    }

    else
    {
      // At this point, x1, x2 and y all mutually intersect

      if(x1.ub() > y.ub())
        x1 = Interval(x1.lb(),y.ub());

      if(x2.ub() > y.ub())
        x2 = Interval(x2.lb(),y.ub());
    }
#else
    // max_rel() of GAOL v5, the reverse of max: max_rel(y,x2,x1) is the hull of
    // the x of x1 with max(x,v) in y for some v of x2 (the result, the other
    // operand, then the operand it contracts), and x2 is contracted in turn
    // with the x1 just contracted. Each operand is empty when y lies below it,
    // contracted to y when the other operand lies below y, and to [-oo,y.ub()]
    // when the other operand meets y. The bounds are those of x1, x2 and y, the
    // tightest. The code of IBEX above gave the same intervals, but left an
    // operand as it was when the other one was empty, where GAOL gives the
    // empty set for both (compared on every triple of 41 intervals).
    x1 = gaol::max_rel(y, x2, x1);
    x2 = gaol::max_rel(y, x1, x2);
    if(x2.is_empty())
      x1.set_empty();
#endif
  }
}
