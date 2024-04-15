/** 
 *  \file
 *  Operations
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>
#include "codac2_Expr.h"
#include "codac2_Arg.h"
#include "codac2_ConstValue.h"

namespace codac2
{
  using ScalarExpr_ptr = std::shared_ptr<Expr<Interval>>;
  using VectorExpr_ptr = std::shared_ptr<Expr<IntervalVector>>;
  using MatrixExpr_ptr = std::shared_ptr<Expr<IntervalMatrix>>;

  // operator+ (unary case)
    
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,Interval,Interval>>(x1);
    }

    inline VectorExpr_ptr
    operator+(const VectorExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalVector,IntervalVector>>(x1);
    }

    inline MatrixExpr_ptr
    operator+(const MatrixExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalMatrix,IntervalMatrix>>(x1);
    }

  // operator+
    
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,Interval,Interval,Interval>>(x1,x2);
    }

    inline VectorExpr_ptr
    operator+(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalVector,IntervalVector,IntervalVector>>(x1,x2);
    }

    inline MatrixExpr_ptr
    operator+(const MatrixExpr_ptr& x1, const MatrixExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalMatrix,IntervalMatrix,IntervalMatrix>>(x1,x2);
    }

    template<typename X1>
    inline ScalarExpr_ptr
    operator+(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator+(const_value(x1),x2);
    }

    template<typename X2>
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator+(x1, const_value(x2));
    }

  // operator- (unary case)
    
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,Interval,Interval>>(x1);
    }

    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalVector,IntervalVector>>(x1);
    }

    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalMatrix,IntervalMatrix>>(x1);
    }

  // operator-
    
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,Interval,Interval,Interval>>(x1,x2);
    }

    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalVector,IntervalVector,IntervalVector>>(x1,x2);
    }

    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1, const MatrixExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalMatrix,IntervalMatrix,IntervalMatrix>>(x1,x2);
    }

    template<typename X1>
    inline ScalarExpr_ptr
    operator-(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator-(const_value(x1),x2);
    }

    template<typename X2>
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

  // operator*
    
    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcMul,Interval,Interval,Interval>>(x1,x2);
    }

    template<typename X1>
    inline ScalarExpr_ptr
    operator*(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator*(const_value(x1),x2);
    }

    template<typename X2>
    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator*(x1, const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const MatrixExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcMul,IntervalVector,IntervalMatrix,IntervalVector>>(x1,x2);
    }

  // operator/
    
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<OperationExpr<CtcDiv,Interval,Interval,Interval>>(x1,x2);
    }

    template<typename X1>
    inline ScalarExpr_ptr
    operator/(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator/(const_value(x1),x2);
    }

    template<typename X2>
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator/(x1, const_value(x2));
    }

  // Other operators

    inline ScalarExpr_ptr
    sqr(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSqr,Interval,Interval>>(x1);
    }

    inline ScalarExpr_ptr 
    sqrt(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSqrt,Interval,Interval>>(x1);
    }

    inline ScalarExpr_ptr
    cos(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcCos,Interval,Interval>>(x1);
    }

    inline ScalarExpr_ptr
    sin(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcSin,Interval,Interval>>(x1);
    }

    inline ScalarExpr_ptr
    tanh(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcTanh,Interval,Interval>>(x1);
    }

    inline ScalarExpr_ptr
    abs(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<OperationExpr<CtcAbs,Interval,Interval>>(x1);
    }

  // Structural operators

    template<typename... X>
    inline VectorExpr_ptr
    vec(const std::shared_ptr<Expr<X>>&... x)
    {
      return std::make_shared<OperationExpr<CtcVector,IntervalVector,X...>>(x...);
    }

    template<typename... X>
    inline MatrixExpr_ptr
    mat(const std::shared_ptr<Expr<X>>&... x)
    {
      return std::make_shared<OperationExpr<CtcMatrix,IntervalMatrix,X...>>(
        OperationExpr<CtcMatrix,IntervalMatrix,X...>(x...));
    }
}