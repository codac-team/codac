/** 
 *  \file codac2_arith_mul.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
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

    static IntervalVector fwd(const IntervalMatrix& x1, const IntervalVector& x2);
    static VectorType fwd_natural(const MatrixType& x1, const VectorType& x2);
    static VectorType fwd_centered(const MatrixType& x1, const VectorType& x2);
    static void bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2);
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

  inline VectorExpr
  operator*(const MatrixExpr& x1, const VectorExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<MulOp,VectorType,MatrixType,VectorType>>(x1,x2) };
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

  inline void MulOp::bwd(const Interval& y, IntervalRow& x1, IntervalVector& x2)
  {
    assert(x1.size() == x2.size());

    const Index n = x1.size();
    std::vector<Interval> sums(n), prods(n);

    // Forward propagation

      for(Index i = 0 ; i < n ; i++)
      {
        prods[i] = x1[i]*x2[i];
        sums[i] = prods[i];
        if(i > 0) sums[i] += sums[i-1];
      }

    // Backward propagation

      sums[n-1] &= y;

      for(Index i = n-1 ; i >= 0 ; i--)
      {
        if(i > 0) AddOp::bwd(sums[i],sums[i-1],prods[i]);
        else prods[0] &= sums[0];
        MulOp::bwd(prods[i],x1[i],x2[i]);
      }
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
    
    return {
      fwd(x1.a, /* <<----- x1.m */ x2.m),
      fwd(x1.a, x2.a),
      IntervalMatrix::zero(x1.a.rows(),x1.a.cols()), // todo
      x1.def_domain && x2.def_domain
    };
  }

  inline void MulOp::bwd(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2)
  {
    assert(x1.rows() == y.size());
    assert(x1.cols() == x2.size());

    /*if(x1.is_squared()) // not working for any squared x1
    {
      CtcGaussElim ctc_ge;
      CtcLinearPrecond ctc_gep(ctc_ge);
      IntervalVector y_(y);
      ctc_gep.contract(x1,x2,y_);
    }*/

    if(x1.rows() > x1.cols())
    {
      Index last_row = 0;
      Index i = 0;

      do
      {
        double vol_x2 = x2.volume();
        IntervalRow row_i = x1.row(i);
        MulOp::bwd(y[i],row_i,x2);

        if(row_i.is_empty())
        {
          x1.set_empty();
          return;
        }

        else
          x1.row(i) = row_i;

        if(x2.volume()/vol_x2 < 0.98)
          last_row = i;
        i = (i+1)%y.size();
      } while(i != last_row);
    }

    else
    {
      IntervalMatrix Q = gauss_jordan(x1.mid());
      IntervalVector b_tilde = Q*y;
      IntervalMatrix A_tilde = Q*x1; // should be a tree matrix

      for(int a = 0 ; a < 1 ; a++)
      {
        for(Index i = 0 ; i < x2.size() ; i++)
        {
          for(Index k = 0 ; k < b_tilde.size() ; k++)
          {
            Interval u = b_tilde[k];

            for(Index j = 0 ; j < x2.size() ; j++)
              if(i != j)
                u -= x2[j]*A_tilde(k,j);

            MulOp::bwd(u, x2[i], A_tilde(k,i));
          }
        }
      }
    }
  }
}