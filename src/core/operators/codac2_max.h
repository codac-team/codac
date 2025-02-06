/** 
 *  \file codac2_max.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
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
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain
    };
  }

  // Inline functions

  inline Interval MaxOp::fwd(const Interval& x1, const Interval& x2)
  {
    return max(x1,x2);
  }

  inline void MaxOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
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
  }
}