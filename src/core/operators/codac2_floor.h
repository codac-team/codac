/** 
 *  \file codac2_floor.h
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
  struct FloorOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "⌊" + x1->str() + "⌋";
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
  floor(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<FloorOp,ScalarType,ScalarType>>(x1) };
  }

  // Inline functions

  inline Interval FloorOp::fwd(const Interval& x1)
  {
    return floor(x1);
  }

  inline ScalarType FloorOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline ScalarType FloorOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    return {
      fwd(x1.m),
      fwd(x1.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain
    };
  }

  inline void FloorOp::bwd(const Interval& y, Interval& x1)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(y.is_empty())
      x1.set_empty();

    else
    {
      double r = std::floor(y.ub());
      double l = std::ceil(y.lb());

      if(r < l)
        x1.set_empty();

      else
        x1 &= Interval(l,r) + Interval(0,1);
    }
  }
}
