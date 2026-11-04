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
#include "codac2_arith_mul.h"

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
    static VectorType fwd_centered(bool mixed_diff,const VectorType& x1, const VectorType& x2);
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
      fwd(x1.get_a(),x2.get_a()),
      x1.def_domain && x2.def_domain
    };
  }

  inline VectorType CrossProdOp::fwd_centered(bool mixed_diff,const VectorType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.cols() == x2.da.cols());
    IntervalMatrix d(3,x1.da.cols());
    for (Index i=0;i<x1.da.cols();i++) {
        const IntervalVector& x1a = x1.get_a(mixed_diff,i);
        const IntervalVector& x2a = x1.get_a(mixed_diff,i);
        d(0,i) = x1.da(1,i)*x2a[2] + x1a[1]*x2.da(2,i) 
               - x1.da(2,i)*x2a[1] - x1a[2]*x2.da(1,i); 
        d(1,i) = x1.da(2,i)*x2a[0] + x1a[2]*x2.da(0,i) 
               - x1.da(0,i)*x2a[2] - x1a[0]*x2.da(2,i); 
        d(2,i) = x1.da(0,i)*x2a[1] + x1a[0]*x2.da(1,i) 
               - x1.da(1,i)*x2a[0] - x1a[1]*x2.da(0,i); 
    }

    return {
      fwd(x1.m,x2.m),
      vector_map<IntervalVector,IntervalVector,IntervalVector>(x1.a,x2.a,&fwd),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  inline void CrossProdOp::bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert_release(y.size() == 3 && x1.size() == 3 && x2.size() == 3);
    IntervalRow u { 0.0, x2[2], -x2[1] };
    MulOp::bwd(y[0],u,x1);
    if (x1.is_empty()) { x2.set_empty(); return; }
    x2[2] &= u[1]; x2[1] &= -u[2];
    IntervalRow v { -x2[2], 0.0, x2[0] };
    MulOp::bwd(y[1],v,x1);
    if (x1.is_empty()) { x2.set_empty(); return; }
    x2[2] &= -v[0]; x2[0] &= v[2];
    IntervalRow w { x2[1], -x2[0], 0.0 };
    MulOp::bwd(y[2],w,x1);
    if (x1.is_empty()) { x2.set_empty(); return; }
    x2[1] &= w[0]; x2[0] &= -w[1];
  }
}
