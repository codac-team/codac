/** 
 *  \file codac2_ceil.h
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
  struct CeilOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "⌈" + x1->str() + "⌉";
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
  ceil(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<CeilOp,ScalarType,ScalarType>>(x1) };
  }

  // Inline functions

  inline Interval CeilOp::fwd(const Interval& x1)
  {
    return ceil(x1);
  }

  inline ScalarType CeilOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline ScalarType CeilOp::fwd_centered(const ScalarType& x1)
  {
    return {
      fwd(x1.m),
      fwd(x1.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain
    };
  }

  inline void CeilOp::bwd(const Interval& y, Interval& x1)
  {
#if CODAC_GAOL_GUARDS
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
        x1 &= Interval(l,r) + Interval(-1,0);
    }
#else
    // GAOL v5 has no reverse of ceil, but ceil(x) = -floor(-x) for every real
    // x: the x of x1 whose ceil is in y are the opposites of the u of -x1 whose
    // floor is in -y, which floor_rel() of GAOL v5 gives (see FloorOp::bwd()),
    // the opposite of an interval being exact. The result is the hull of the
    // part of x1 in (ceil(y.lb())-1,floor(y.ub())], and the empty set when y
    // holds no integer or one of y and x1 is empty. The code of IBEX above
    // computed the same interval, closed at ceil(y.lb())-1, whose ceil is not
    // in y: GAOL leaves that end out where x1 ends there (the reverse of ceil
    // for y = [1] and x1 = [0] is empty, where IBEX gave [0]). A zero bound of
    // the result may be -0, the opposite of +0, which is the same number.
    x1 = -gaol::floor_rel(-y, -x1);
#endif
  }
}
