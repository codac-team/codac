/** 
 *  \file codac2_arith_mul.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalRow.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_arith_add.h"

#include "codac2_linear_ctc.h"
#include "codac2_GaussJordan.h"

namespace codac2
{
  struct MulOp
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return x1->str(!x1->is_str_leaf()) + "*" + x2->str(!x2->is_str_leaf());
    }
    
    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape(const X1& s1, const X2& s2)
    {
      auto shape1 = s1->output_shape();
      auto shape2 = s2->output_shape();

      if(shape1.first == 1 && shape1.second == 1)
        return shape2;
      
      else if(shape2.first == 1 && shape2.second == 1)
        return shape1;
      
      else
      {
        assert(shape1.second == shape2.first);
        return { shape1.first, shape2.second };
      }
    }

    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const Interval& x1, const IntervalVector& x2);
    static VectorType fwd_natural(const ScalarType& x1, const VectorType& x2);
    static VectorType fwd_centered(const ScalarType& x1, const VectorType& x2);
    static void bwd(const IntervalVector& y, Interval& x1, IntervalVector& x2);

    static IntervalVector fwd(const IntervalVector& x1, const Interval& x2);
    static VectorType fwd_natural(const VectorType& x1, const ScalarType& x2);
    static VectorType fwd_centered(const VectorType& x1, const ScalarType& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2);

    static Interval fwd(const IntervalRow& x1, const IntervalVector& x2);
    //static ScalarType fwd(const RowType& x1, const VectorType& x2); // RowType not yet defined
    static void bwd(const Interval& y, IntervalRow& x1, IntervalVector& x2);

    static IntervalMatrix fwd(const Interval& x1, const IntervalMatrix& x2);
    static MatrixType fwd_natural(const ScalarType& x1, const MatrixType& x2);
    static MatrixType fwd_centered(const ScalarType& x1, const MatrixType& x2);
    static void bwd(const IntervalMatrix& y, Interval& x1, IntervalMatrix& x2);

    static IntervalVector fwd(const IntervalMatrix& x1, const IntervalVector& x2);
    static VectorType fwd_natural(const MatrixType& x1, const VectorType& x2);
    static VectorType fwd_centered(const MatrixType& x1, const VectorType& x2);
    static void bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2);

    static IntervalMatrix fwd(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static MatrixType fwd_natural(const MatrixType& x1, const MatrixType& x2);
    static MatrixType fwd_centered(const MatrixType& x1, const MatrixType& x2);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  // operator*
  // The following functions can be used to build analytic expressions.
  
  inline ScalarExpr
  operator*(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }
  
  inline VectorExpr
  operator*(const ScalarExpr& x1, const VectorExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,VectorType,ScalarType,VectorType>>(x1,x2) };
  }
  
  inline VectorExpr
  operator*(const VectorExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,VectorType,VectorType,ScalarType>>(x1,x2) };
  }

  inline MatrixExpr
  operator*(const ScalarExpr& x1, const MatrixExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,MatrixType,ScalarType,MatrixType>>(x1,x2) };
  }

  inline VectorExpr
  operator*(const MatrixExpr& x1, const VectorExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,VectorType,MatrixType,VectorType>>(x1,x2) };
  }

  inline MatrixExpr
  operator*(const MatrixExpr& x1, const MatrixExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,MatrixType,MatrixType,MatrixType>>(x1,x2) };
  }

  // Inline functions

  inline Interval MulOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 * x2;
  }

  inline ScalarType MulOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline ScalarType MulOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.rows() == 1);
    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());

    IntervalMatrix d(1,x1.da.cols());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)*x2.a + x1.a*x2.da(0,i);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  inline void MulOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    x1 = gaol::div_rel(y, x2, x1);
    x2 = gaol::div_rel(y, x1, x2);
  }

  inline IntervalVector MulOp::fwd(const Interval& x1, const IntervalVector& x2)
  {
    return x1 * x2;
  }

  inline VectorType MulOp::fwd_natural(const ScalarType& x1, const VectorType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline VectorType MulOp::fwd_centered(const ScalarType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.rows() == 1);
    assert(x1.da.cols() == x2.da.cols());
    assert(x2.a.size() == x2.da.rows());

    IntervalMatrix d(x2.da.rows(),x2.da.cols());
    for(Index i = 0 ; i < d.rows() ; i++)
      for(Index j = 0 ; j < d.cols() ; j++)
        d(i,j) = x1.da(0,j)*x2.a[i]+x1.a*x2.da(i,j);
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  inline void MulOp::bwd(const IntervalVector& y, Interval& x1, IntervalVector& x2)
  {
    assert(y.size() == x2.size());
    for(Index i = 0 ; i < x2.size() ; i++)
      MulOp::bwd(y[i], x1, x2[i]);
  }

  inline IntervalVector MulOp::fwd(const IntervalVector& x1, const Interval& x2)
  {
    return MulOp::fwd(x2,x1);
  }

  inline VectorType MulOp::fwd_natural(const VectorType& x1, const ScalarType& x2)
  {
    return MulOp::fwd_natural(x2,x1);
  }

  inline VectorType MulOp::fwd_centered(const VectorType& x1, const ScalarType& x2)
  {
    return MulOp::fwd_centered(x2,x1);
  }

  inline void MulOp::bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2)
  {
    MulOp::bwd(y, x2, x1);
  }

  inline Interval MulOp::fwd(const IntervalRow& x1, const IntervalVector& x2)
  {
    assert(x1.size() == x2.size());
    Interval s(0.);
    for(Index i = 0 ; i < x1.size() ; i++)
      s += x1[i]*x2[i];
    return s;
  }

  //ScalarType MulOp::fwd(const RowType& x1, const VectorType& x2)
  //{
  //  // RowType not yet defined
  //}

  inline IntervalMatrix MulOp::fwd(const Interval& x1, const IntervalMatrix& x2)
  {
    return x1 * x2;
  }

  inline MatrixType MulOp::fwd_natural(const ScalarType& x1, const MatrixType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline MatrixType MulOp::fwd_centered(const ScalarType& x1, const MatrixType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x2.da.cols() == x1.da.cols());
    IntervalMatrix d(x2.da.rows(),x2.da.cols());
    for (Index j=0; j<d.cols(); j++) 
      for (Index i=0; i<d.rows(); i++) {
        d(i,j) = x1.da(0,j)*x2.a.reshaped<Eigen::ColMajor>()[i]+x1.a*x2.da(i,j);
    }
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d, 
      x1.def_domain && x2.def_domain
    };
  }

  inline void MulOp::bwd([[maybe_unused]] const IntervalMatrix& y, [[maybe_unused]] Interval& x1, [[maybe_unused]] IntervalMatrix& x2)
  {
    // todo
  }

  inline IntervalVector MulOp::fwd(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    assert(x1.cols() == x2.size());
    return x1 * x2;
  }

  inline VectorType MulOp::fwd_natural(const MatrixType& x1, const VectorType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline VectorType MulOp::fwd_centered(const MatrixType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    assert(x2.da.cols() == x1.da.cols());
    IntervalMatrix d = IntervalMatrix::zero(x1.a.rows(),x1.da.cols());
    for (Index j=0; j<d.cols(); j++) 
      for (Index i=0; i<d.rows(); i++) {
        for (Index k=0; k<x2.a.size(); k++) {
          d(i,j) += x1.da(i+k*x1.a.rows(),j)*x2.a[k]+x1.a(i,k)*x2.da(k,j);
        }
    }
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d, 
      x1.def_domain && x2.def_domain
    };
  }

  inline IntervalMatrix MulOp::fwd(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    assert(x1.cols() == x2.rows());
    return x1 * x2;
  }

  inline MatrixType MulOp::fwd_natural(const MatrixType& x1, const MatrixType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline MatrixType MulOp::fwd_centered(const MatrixType& x1, const MatrixType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    assert(x2.da.cols() == x1.da.cols());
    IntervalMatrix d = IntervalMatrix::zero(x1.a.rows()*x2.a.cols(),x1.da.cols());
    for (Index j=0; j<d.cols(); j++) 
      for (Index i=0; i<d.rows(); i++)
      {
        Index row_i = i%x1.a.rows();
        Index col_i = i/x1.a.rows();
        for (Index k=0; k<x2.a.rows(); k++) {
          d(i,j) += x1.da(row_i+k*x1.a.rows(),j)*x2.a(k,col_i)
                    +x1.a(row_i,k)*x2.da(k+col_i*x2.a.rows(),j);
        }
    }
    
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain
    };
  }

  inline void MulOp::bwd([[maybe_unused]] const IntervalMatrix& y, [[maybe_unused]] IntervalMatrix& x1, [[maybe_unused]] IntervalMatrix& x2)
  {
    assert(x1.rows() == x2.cols());
    assert(y.rows() == x1.rows() && y.cols() == x2.cols());

    // todo
  }
}
