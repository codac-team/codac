/** 
 *  \file codac2_det.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_arith_sub.h"
#include "codac2_arith_mul.h"

namespace codac2
{
  struct DetOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "det(" + x1->str() + ")";
    }

    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return "det(" + x1->str() + "," + x2->str() + ")";
    }

    template<typename X1,typename X2,typename X3>
    static std::string str(const X1& x1, const X2& x2, const X3& x3)
    {
      return "det(" + x1->str() + "," + x2->str() + "," + x3->str() + ")";
    }

    // For matrices
    template<typename X1>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1) {
      return {1,1};
    }
    static Interval fwd(const IntervalMatrix& x);
    static ScalarType fwd_natural(const MatrixType& x);
    static ScalarType fwd_centered(const MatrixType& x);
    static void bwd(const Interval& y, IntervalMatrix& x);

    // For two vectors (merged into a 2×2 matrix)
    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2) {
      return {1,1};
    }
    static Interval fwd(const IntervalVector& x1, const IntervalVector& x2);
    static ScalarType fwd_natural(const VectorType& x1, const VectorType& x2);
    static ScalarType fwd_centered(const VectorType& x1, const VectorType& x2);
    static void bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2);

    // For three vectors (merged into a 3×3 matrix)
    template<typename X1, typename X2, typename X3>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1, [[maybe_unused]] const X2& s2, [[maybe_unused]] const X3& s3) {
      return {1,1};
    }
    static Interval fwd(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3);
    static ScalarType fwd_natural(const VectorType& x1, const VectorType& x2, const VectorType& x3);
    static ScalarType fwd_centered(const VectorType& x1, const VectorType& x2, const VectorType& x3);
    static void bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2, IntervalVector& x3);
  };

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline ScalarExpr
  det(const MatrixExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<DetOp,ScalarType,MatrixType>>(x1) };
  }

  inline ScalarExpr
  det(const VectorExpr& x1, const VectorExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<DetOp,ScalarType,VectorType,VectorType>>(x1,x2) };
  }

  inline ScalarExpr
  det(const VectorExpr& x1, const VectorExpr& x2, const VectorExpr& x3)
  {
    return { std::make_shared<AnalyticOperationExpr<DetOp,ScalarType,VectorType,VectorType,VectorType>>(x1,x2,x3) };
  }
  
  // Inline functions

  inline Interval DetOp::fwd(const IntervalMatrix& x)
  {
    assert_release(x.is_squared() && "can only compute determinants for a square matrix");
    assert_release((x.rows() == 1 || x.rows() == 2) && "determinant not yet computable for n×n matrices, n>2");

    if(x.rows() == 1) // 1×1 matrix
      return x(0,0);

    else if(x.rows() == 2) // 2×2 matrix
      return x(0,0)*x(1,1)-x(0,1)*x(1,0);

    else
      return Interval::empty(); // unhandled case
  }

  inline ScalarType DetOp::fwd_natural(const MatrixType& x)
  {
    return {
      fwd(x.a),
      x.def_domain
    };
  }

  inline ScalarType DetOp::fwd_centered(const MatrixType& x)
  {
    if(centered_form_not_available_for_args(x)) 
      return fwd_natural(x);

    if(x.a.rows()==1) {
      return {
        fwd(x.m),
        fwd(x.a),
        x.da, // keep matrix type for diff 
        x.def_domain
      };
    }
    
    IntervalMatrix d(1, x.da.cols());
    if (x.a.rows()==2) { /* otherwise, will fail afterwards */
       for (Index i=0; i < d.cols() ; i++) {
          d(0,i) = x.da(0,i)*x.a(1,1) + x.da(3,i)*x.a(0,0)
		 - x.da(1,i)*x.a(0,1) - x.da(2,i)*x.a(1,0);
		/* note:  ColMajor is assumed here */
       }      
    }
 
    return {
      fwd(x.m),
      fwd(x.a),
      d, 
      x.def_domain
    };
  }

  inline void DetOp::bwd(const Interval& y, IntervalMatrix& x)
  {
    assert_release(x.is_squared() && "can only compute determinants for a square matrix");
    assert_release((x.rows() == 1 || x.rows() == 2) && "determinant not yet computable for n×n matrices, n>2");

    if(x.rows() == 1) // 1×1 matrix
      x(0,0) &= y;

    else if(x.rows() == 2) // 2×2 matrix
    {
      Interval z1 = x(0,0)*x(1,1), z2 = x(1,0)*x(0,1);
      SubOp::bwd(y, z1, z2);
      MulOp::bwd(z1, x(0,0), x(1,1));
      MulOp::bwd(z2, x(1,0), x(0,1));
    }

    else
    {
      // unhandled case
    }
  }

  inline Interval DetOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    assert_release(x1.size() == 2 && x2.size() == 2 && "determinant only computable for pairs of 2d vectors");
    IntervalMatrix m(2,2);
    m.col(0) = x1; m.col(1) = x2;
    return DetOp::fwd(m);
  }

  inline ScalarType DetOp::fwd_natural(const VectorType& x1, const VectorType& x2)
  {
    IntervalMatrix a(2,2);
    a.col(0) = x1.a; a.col(1) = x2.a;

    return {
      fwd(a),
      x1.def_domain && x2.def_domain
    };
  }

  inline ScalarType DetOp::fwd_centered(const VectorType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    IntervalMatrix m(2,2);
    m.col(0) = x1.m; m.col(1) = x2.m;
    IntervalMatrix a(2,2);
    a.col(0) = x1.a; a.col(1) = x2.a;

    assert(x1.da.cols() == x2.da.cols());
    IntervalMatrix d(1, x1.da.cols());
    for (Index i=0; i < d.cols() ; i++) {
          d(0,i) = x1.da(0,i)*x2.a[1] + x1.a[0]*x2.da(1,i)
		 - x1.da(1,i)*x2.a[0] - x1.a[1]*x2.da(0,i);
    }
 
    return {
      fwd(m),
      fwd(a),
      d, 
      x1.def_domain && x2.def_domain
    };
  }

  inline void DetOp::bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert_release(x1.size() == 2 && x2.size() == 2 && "determinant only computable for pairs of 2d vectors");
    IntervalMatrix m(2,2);
    m.col(0) = x1; m.col(1) = x2;
    DetOp::bwd(y,m);
    x1 &= m.col(0);
    x2 &= m.col(1);
  }

  inline Interval DetOp::fwd(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3)
  {
    assert_release(x1.size() == 3 && x2.size() == 3 && x3.size() == 3 && "determinant only computable for triplet of 3d vectors");
    IntervalMatrix m(3,3);
    m.col(0) = x1; m.col(1) = x2; m.col(2) = x3;
    return DetOp::fwd(m);
  }

  inline ScalarType DetOp::fwd_natural(const VectorType& x1, const VectorType& x2, const VectorType& x3)
  {
    IntervalMatrix a(3,3);
    a.col(0) = x1.a; a.col(1) = x2.a; a.col(2) = x3.a;

    return {
      fwd(a),
      x1.def_domain && x2.def_domain && x3.def_domain
    };
  }

  inline ScalarType DetOp::fwd_centered(const VectorType& x1, const VectorType& x2, const VectorType& x3)
  {
    if(centered_form_not_available_for_args(x1,x2,x3))
      return fwd_natural(x1,x2,x3);

    IntervalMatrix m(3,3);
    m.col(0) = x1.m; m.col(1) = x2.m; m.col(2) = x3.m;
    IntervalMatrix a(3,3);
    a.col(0) = x1.a; a.col(1) = x2.a; a.col(2) = x3.a;

    return {
      fwd(m),
      fwd(a),
      IntervalMatrix(0,0), // not supported yet for auto diff
      x1.def_domain && x2.def_domain && x3.def_domain
    };
  }

  inline void DetOp::bwd(const Interval& y, IntervalVector& x1, IntervalVector& x2, IntervalVector& x3)
  {
    assert_release(x1.size() == 3 && x2.size() == 3 && x3.size() == 3 && "determinant only computable for triplet of 3d vectors");
    
    IntervalMatrix m(3,3);
    m.col(0) = x1; m.col(1) = x2; m.col(2) = x3;

    DetOp::bwd(y,m);
    x1 &= m.col(0);
    x2 &= m.col(1);
    x3 &= m.col(2);
  }
}
