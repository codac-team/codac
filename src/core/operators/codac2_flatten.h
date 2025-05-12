/** 
 *  \file codac2_flatten.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"

namespace codac2
{
  struct FlattenOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "flatten(" + x1->str() + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1)
    {
      auto shape1 = s1->output_shape();
      return { shape1.second*shape1.first, 1 };
    }

    static IntervalVector fwd(const IntervalMatrix& x1);
    static VectorType fwd_natural(const MatrixType& x1);
    static VectorType fwd_centered(const MatrixType& x1);
    static void bwd(const IntervalVector& y, IntervalMatrix& x1);
  };

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline VectorExpr
  flatten(const MatrixExpr &x1) 
  {
     return { std::make_shared<AnalyticOperationExpr<FlattenOp,VectorType,MatrixType>>(x1) };
  }

  // Inline functions

    inline IntervalVector FlattenOp::fwd(const IntervalMatrix& x1)
    {
      return x1.reshaped();
    }

    inline VectorType FlattenOp::fwd_natural(const MatrixType& x1)
    {
      return {
        fwd(x1.a),
        x1.def_domain
      };
    }

    inline VectorType FlattenOp::fwd_centered(const MatrixType& x1)
    {
      if(centered_form_not_available_for_args(x1))
        return fwd_natural(x1);
      
      return {
        fwd(x1.m),
        fwd(x1.a),
        x1.da,
        x1.def_domain
      };
    }

    inline void FlattenOp::bwd(const IntervalVector& y, IntervalMatrix& x1)
    {
      x1 &= y.reshaped(x1.rows(),x1.cols());
    }

}
