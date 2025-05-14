/** 
 *  \file codac2_transpose.h
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
  struct TransposeOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "transpose(" + x1->str() + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1)
    {
      auto shape1 = s1->output_shape();
      return { shape1.second, shape1.first };
    }

    static IntervalMatrix fwd(const IntervalMatrix& x1);
    static MatrixType fwd_natural(const MatrixType& x1);
    static MatrixType fwd_centered(const MatrixType& x1);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1);
  };

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline MatrixExpr
  transpose(const MatrixExpr &x1) 
  {
     return { std::make_shared<AnalyticOperationExpr<TransposeOp,MatrixType,MatrixType>>(x1) };
  }

  // Inline functions

    inline IntervalMatrix TransposeOp::fwd(const IntervalMatrix& x1)
    {
      return x1.transpose();
    }

    inline MatrixType TransposeOp::fwd_natural(const MatrixType& x1)
    {
      return {
        fwd(x1.a),
        x1.def_domain
      };
    }

    inline MatrixType TransposeOp::fwd_centered(const MatrixType& x1)
    {
      if(centered_form_not_available_for_args(x1))
        return fwd_natural(x1);
      
      IntervalMatrix d(x1.da.rows(),x1.da.cols());
      for (Index i=0; i<x1.a.rows(); i++)
        for (Index j=0;j<x1.a.cols(); j++) {
            /* x2.a(j,i) = x1.a(i,j) */
         d.row(j+i*x1.a.cols()) = x1.da.row(i+j*x1.a.rows());
      }

      return {
        fwd(x1.m),
        fwd(x1.a),
        d,
        x1.def_domain
      };
    }

    inline void TransposeOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1)
    {
      x1 &= y.transpose();
    }

}
