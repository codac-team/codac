/** 
 *  \file codac2_sqrt.h
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
  struct SqrtOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "sqrt(" + x1->str() + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1)
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
  sqrt(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<SqrtOp,ScalarType,ScalarType>>(x1) };
  }

  // Inline functions

  inline Interval SqrtOp::fwd(const Interval& x1)
  {
    return sqrt(x1);
  }

  inline ScalarType SqrtOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.a.is_subset({0,oo}) // def domain of sqrt
      && x1.a != 0. // def domain of the derivative of sqrt
      && x1.def_domain
    };
  }

  inline ScalarType SqrtOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/(2.*sqrt(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.a.is_subset({0,oo}) // def domain of sqrt
      && x1.a != 0. // def domain of the derivative of sqrt
      && x1.def_domain
    };
  }

  inline void SqrtOp::bwd(const Interval& y, Interval& x1)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(y.is_empty() || y.ub() < 0)
      x1.set_empty();

    else if(y.lb() < 0)
      x1 &= sqr(Interval(0,y.ub()));

    else
      x1 &= sqr(y);
  }
}
