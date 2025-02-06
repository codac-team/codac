/** 
 *  \file codac2_min.h
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
  struct MinOp
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  min(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MinOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }

  // Inline functions

  inline Interval MinOp::fwd(const Interval& x1, const Interval& x2)
  {
    return min(x1,x2);
  }

  inline ScalarType MinOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline ScalarType MinOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain
    };
  }

  inline void MinOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    Interval mx1 = -x1;
    Interval mx2 = -x2;

    MaxOp::bwd(-y,mx1,mx2);

    if(mx1.is_empty() || mx2.is_empty())
    {
      x1.set_empty();
      x2.set_empty();
    }

    else
    {
      x1 = -mx1;
      x2 = -mx2;
    }
  }
}