/** 
 *  \file codac2_cross_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct CrossProdOp
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return x1->str(!x1->is_str_leaf()) + "×" + x2->str(!x2->is_str_leaf());
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2)
    {
      return {3,1};
    }

    static IntervalVector fwd(const IntervalVector& x1, const IntervalVector& x2);
    static VectorType fwd_natural(const VectorType& x1, const VectorType& x2);
    static VectorType fwd_centered(const VectorType& x1, const VectorType& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline VectorExpr
  cross_prod(const VectorExpr& x1, const VectorExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<CrossProdOp,VectorType,VectorType,VectorType>>(x1,x2) };
  }
  
  // Inline functions

  inline IntervalVector CrossProdOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    assert_release(x1.size() == 3 && x2.size() == 3);
    return {
      x1[1]*x2[2] - x1[2]*x2[1],
      x1[2]*x2[0] - x1[0]*x2[2],
      x1[0]*x2[1] - x1[1]*x2[0]
    };
  }

  inline VectorType CrossProdOp::fwd_natural(const VectorType& x1, const VectorType& x2)
  {
    return {
      fwd(x1.a,x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline VectorType CrossProdOp::fwd_centered(const VectorType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.cols() == x2.da.cols());
    IntervalMatrix d(3,x1.da.cols());
    for (Index i=0;i<x1.da.cols();i++) {
        d(0,i) = x1.da(1,i)*x2.a[2] + x1.a[1]*x2.da(2,i) 
               - x1.da(2,i)*x2.a[1] - x1.a[2]*x2.da(1,i); 
        d(1,i) = x1.da(2,i)*x2.a[0] + x1.a[2]*x2.da(0,i) 
               - x1.da(0,i)*x2.a[2] - x1.a[0]*x2.da(2,i); 
        d(2,i) = x1.da(0,i)*x2.a[1] + x1.a[0]*x2.da(1,i) 
               - x1.da(1,i)*x2.a[0] - x1.a[1]*x2.da(0,i); 
    }

    return {
      fwd(x1.m,x2.m),
      fwd(x1.a,x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  inline void CrossProdOp::bwd([[maybe_unused]] const IntervalVector& y, [[maybe_unused]] IntervalVector& x1, [[maybe_unused]] IntervalVector& x2)
  {
    assert_release(y.size() == 3 && x1.size() == 3 && x2.size() == 3);
    // todo
  }
}
