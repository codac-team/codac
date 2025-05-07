/** 
 *  \file codac2_abs.h
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
  struct AbsOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "|" + x1->str() + "|";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& x1)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  abs(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AbsOp,ScalarType,ScalarType>>(x1) };
  }
    
  // Inline functions

  inline Interval AbsOp::fwd(const Interval& x1)
  {
    return abs(x1);
  }

  inline ScalarType AbsOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain && x1.a != 0. // def domain of the derivative of abs
    };
  }

  inline ScalarType AbsOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);
    
    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = (x1.a/abs(x1.a))*x1.da(0,i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain && x1.a != 0. // def domain of the derivative of abs
    };
  }

  inline void AbsOp::bwd(const Interval& y, Interval& x1)
  {
    x1 = gaol::invabs_rel(y, x1);
  }
}
