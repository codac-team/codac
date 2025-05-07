/** 
 *  \file codac2_chi.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  // chi([x1],[x2],[x3]) = [x2] if (x1^+)<=0, [x3] if (x1^-)>0, hull([x2],[x3]) else
  struct ChiOp
  {
    template<typename X1,typename X2,typename X3>
    static std::string str(const X1& x1, const X2& x2, const X3& x3)
    {
      return "χ(" + x1->str() + "," + x2->str() + "," + x3->str() + ")";
    }

    template<typename X1, typename X2, typename X3>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, const X2& s2, const X3& s3)
    {
      auto shape2 = s2->output_shape();
      auto shape3 = s3->output_shape();
      assert(shape2 == shape3);
      return shape2;
    }

    static Interval fwd(const Interval& x1, const Interval& x2, const Interval& x3);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2, const ScalarType& x3);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2, const ScalarType& x3);
    static void bwd(const Interval& y, Interval& x1, Interval& x2, Interval& x3);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  chi(const ScalarExpr& x1, const ScalarExpr& x2, const ScalarExpr& x3)
  {
    return { std::make_shared<AnalyticOperationExpr<ChiOp,ScalarType,ScalarType,ScalarType,ScalarType>>(x1,x2,x3) };
  }
  
  // Inline functions

  inline Interval ChiOp::fwd(const Interval& x1, const Interval& x2, const Interval& x3)
  {
    return chi(x1,x2,x3);
  }

  inline ScalarType ChiOp::fwd_natural(const ScalarType& x1, const ScalarType& x2, const ScalarType& x3)
  {
    return {
      fwd(x1.a,x2.a,x3.a),
      x1.def_domain && x2.def_domain && x3.def_domain
    };
  }

  inline ScalarType ChiOp::fwd_centered(const ScalarType& x1, const ScalarType& x2, const ScalarType& x3)
  {
    return {
      fwd(x1.m,x2.m,x3.m),
      fwd(x1.a,x2.a,x3.a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain && x3.def_domain
    };
  }

  inline void ChiOp::bwd(const Interval& y, Interval& x1, Interval& x2, Interval& x3)
  {
    // The content of this function comes from the IBEX library.
    // See ibex::Interval (IBEX lib, main author: Gilles Chabert)
    //   https://ibex-lib.readthedocs.io

    if(x1.ub() <= 0)
    {
      if((x2 &= y).is_empty())
      {
        x1.set_empty();
        x3.set_empty();
      }
    }

    else if(x1.lb() > 0)
    {
      if((x3 &= y).is_empty())
      {
        x1.set_empty();
        x2.set_empty();
      }
    }

    if(y.is_disjoint(x2))
    {
      if((x1 &= Interval(0,oo)).is_empty())
      {
        x2.set_empty();
        x3.set_empty();
      }

      if((x3 &= y).is_empty())
      {
        x1.set_empty();
        x2.set_empty();
      }
    }

    if(y.is_disjoint(x3))
    {
      if((x1 &= Interval(-oo,0)).is_empty())
      {
        x2.set_empty();
        x3.set_empty();
      }

      if((x2 &= y).is_empty())
      {
        x1.set_empty();
        x3.set_empty();
      }
    }
  }
}
