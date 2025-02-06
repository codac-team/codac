/** 
 *  \file codac2_acos.h
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
  struct AcosOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  acos(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AcosOp,ScalarType,ScalarType>>(x1) };
  }
  
  // Inline functions

  inline Interval AcosOp::fwd(const Interval& x1)
  {
    return acos(x1);
  }

  inline ScalarType AcosOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.a.is_subset({-1,1}) // def domain of acos
      && x1.a != 1. // def domain of the derivative of acos
      && x1.def_domain
    };
  }

  inline ScalarType AcosOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);
    
    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = -x1.da(0,i)/sqrt(1.-sqr(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({-1,1}) // def domain of acos
      && x1.a != 1. // def domain of the derivative of acos
      && x1.def_domain
    };
  }

  inline void AcosOp::bwd(const Interval& y, Interval& x1)
  {
    x1 &= cos(y);
  }
}