/** 
 *  \file codac2_arith_add.h
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

namespace codac2
{
  struct AddOp
  {
    // Unary operations

    template<typename X1>
    static std::string str(const X1& x1)
    {
      return x1->str();
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1) {
      return s1->output_shape();
    }

    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);

    static IntervalVector fwd(const IntervalVector& x1);
    static VectorType fwd_natural(const VectorType& x1);
    static VectorType fwd_centered(const VectorType& x1);
    static void bwd(const IntervalVector& y, IntervalVector& x1);

    static IntervalMatrix fwd(const IntervalMatrix& x1);
    static MatrixType fwd_natural(const MatrixType& x1);
    static MatrixType fwd_centered(const MatrixType& x1);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1);

    static std::pair<Index,Index> output_shape(const std::pair<Index,Index> &s1);

    // Binary operations

    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return x1->str() + "+" + x2->str();
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape(const X1& s1, const X2& s2) {
      auto shape1=s1->output_shape();
      assert_release(shape1 == s2->output_shape());
      return shape1;
    }

    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const IntervalVector& x1, const IntervalVector& x2);
    static VectorType fwd_natural(const VectorType& x1, const VectorType& x2);
    static VectorType fwd_centered(const VectorType& x1, const VectorType& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);

    static IntervalMatrix fwd(const IntervalMatrix& x1, const IntervalMatrix& x2);
    static MatrixType fwd_natural(const MatrixType& x1, const MatrixType& x2);
    static MatrixType fwd_centered(const MatrixType& x1, const MatrixType& x2);
    static void bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);
  };

  // operator+ (unary case)
  // The following functions can be used to build analytic expressions.
  
  inline ScalarExpr
  operator+(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,ScalarType,ScalarType>>(x1) };
  }

  inline VectorExpr
  operator+(const VectorExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,VectorType,VectorType>>(x1) };
  }

  inline MatrixExpr
  operator+(const MatrixExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,MatrixType,MatrixType>>(x1) };
  }


  // operator+
  // The following functions can be used to build analytic expressions.
  
  inline ScalarExpr
  operator+(ScalarExpr x1, ScalarExpr x2)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }

  inline VectorExpr
  operator+(VectorExpr x1, VectorExpr x2)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,VectorType,VectorType,VectorType>>(x1,x2) };
  }

  inline MatrixExpr
  operator+(MatrixExpr x1, MatrixExpr x2)
  {
    return { std::make_shared<AnalyticOperationExpr<AddOp,MatrixType,MatrixType,MatrixType>>(x1,x2) };
  }

  // Inline functions

  inline Interval AddOp::fwd(const Interval& x1)
  {
    return x1;
  }

  inline ScalarType AddOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline ScalarType AddOp::fwd_centered(const ScalarType& x1)
  {
    return {
      fwd(x1.m),
      fwd(x1.a),
      x1.da,
      x1.def_domain
    };
  }

  inline void AddOp::bwd([[maybe_unused]] const Interval& y, [[maybe_unused]] Interval& x1)
  { }

  inline std::pair<Index,Index> AddOp::output_shape([[maybe_unused]] const std::pair<Index,Index>& s1)
  {
    return s1;
  }

  inline IntervalVector AddOp::fwd(const IntervalVector& x1)
  {
    return x1;
  }

  inline VectorType AddOp::fwd_natural(const VectorType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline VectorType AddOp::fwd_centered(const VectorType& x1)
  {
    return {
      fwd(x1.m),
      fwd(x1.a),
      x1.da,
      x1.def_domain
    };
  }

  inline void AddOp::bwd([[maybe_unused]] const IntervalVector& y, [[maybe_unused]] IntervalVector& x1)
  {
    assert(y.size() == x1.size());
  }

  inline IntervalMatrix AddOp::fwd(const IntervalMatrix& x1)
  {
    return x1;
  }

  inline MatrixType AddOp::fwd_natural(const MatrixType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline MatrixType AddOp::fwd_centered(const MatrixType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    return {
      fwd(x1.m),
      fwd(x1.a),
      x1.da, // unary plus
      x1.def_domain
    };
  }

  inline void AddOp::bwd([[maybe_unused]] const IntervalMatrix& y, [[maybe_unused]] IntervalMatrix& x1)
  {
    assert(y.size() == x1.size());
  }

  inline Interval AddOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 + x2;
  }

  inline ScalarType AddOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline ScalarType AddOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.size() == x2.da.size());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da + x2.da,
      x1.def_domain && x2.def_domain
    };
  }

  inline void AddOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    if((x1 &= y-x2).is_empty())
      x2.set_empty();

    else if((x2 &= y-x1).is_empty())
      x1.set_empty();
  }

  inline IntervalVector AddOp::fwd(const IntervalVector& x1, const IntervalVector& x2)
  {
    assert(x1.size() == x2.size());
    return x1 + x2;
  }

  inline VectorType AddOp::fwd_natural(const VectorType& x1, const VectorType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline VectorType AddOp::fwd_centered(const VectorType& x1, const VectorType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    assert(x1.da.rows() == x2.da.rows() && x1.da.cols() == x2.da.cols());
    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da + x2.da,
      x1.def_domain && x2.def_domain
    };
  }

  inline void AddOp::bwd(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2)
  {
    assert(y.size() == x1.size() && y.size() == x2.size());
    for(Index i = 0 ; i < y.size() ; i++)
      AddOp::bwd(y[i], x1[i], x2[i]);
  }

  inline IntervalMatrix AddOp::fwd(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    assert(x1.cols() == x2.cols() && x1.rows() == x2.rows());
    return x1 + x2;
  }

  inline MatrixType AddOp::fwd_natural(const MatrixType& x1, const MatrixType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain
    };
  }

  inline MatrixType AddOp::fwd_centered(const MatrixType& x1, const MatrixType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      x1.da + x2.da,
      x1.def_domain && x2.def_domain
    };
  }

  inline void AddOp::bwd(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2)
  {
    assert(y.cols() == x1.cols() && y.rows() == x1.rows()
	   && y.cols() == x2.cols() && y.rows() == x2.rows());
    for(Index i = 0 ; i < y.size() ; i++)
      AddOp::bwd(*(y.data()+i), *(x1.data()+i), *(x2.data()+i));
  }
}
