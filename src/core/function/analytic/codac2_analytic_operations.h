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
#include "codac2_AnalyticExpr.h"
#include "codac2_analytic_variables.h"
#include "codac2_analytic_values.h"

namespace codac2
{
  using ScalarExpr_ptr = std::shared_ptr<AnalyticExpr<ScalarOpValue>>;
  using VectorExpr_ptr = std::shared_ptr<AnalyticExpr<VectorOpValue>>;
  using MatrixExpr_ptr = std::shared_ptr<AnalyticExpr<MatrixOpValue>>;

  // operator+ (unary case)
    
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline VectorExpr_ptr
    operator+(const VectorExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,VectorOpValue,VectorOpValue>>(x1);
    }

    inline MatrixExpr_ptr
    operator+(const MatrixExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,MatrixOpValue,MatrixOpValue>>(x1);
    }

  // operator+
    
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    inline VectorExpr_ptr
    operator+(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,VectorOpValue,VectorOpValue,VectorOpValue>>(x1,x2);
    }

    inline MatrixExpr_ptr
    operator+(const MatrixExpr_ptr& x1, const MatrixExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,MatrixOpValue,MatrixOpValue,MatrixOpValue>>(x1,x2);
    }

    template<typename X1, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X1>
      >::type>
    inline ScalarExpr_ptr
    operator+(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator+(const_value(x1),x2);
    }

    template<typename X2, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X2>
      >::type>
    inline ScalarExpr_ptr
    operator+(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator+(x1, const_value(x2));
    }

  // operator- (unary case)
    
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,VectorOpValue,VectorOpValue>>(x1);
    }

    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,MatrixOpValue,MatrixOpValue>>(x1);
    }

  // operator-
    
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,VectorOpValue,VectorOpValue,VectorOpValue>>(x1,x2);
    }

    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1, const MatrixExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,MatrixOpValue,MatrixOpValue,MatrixOpValue>>(x1,x2);
    }

    template<typename X1, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X1>
      >::type>
    inline ScalarExpr_ptr
    operator-(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator-(const_value(x1),x2);
    }

    template<typename X2, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X2>
      >::type>
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

  // operator*
    
    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    template<typename X1, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X1>
      >::type>
    inline ScalarExpr_ptr
    operator*(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator*(const_value(x1),x2);
    }

    template<typename X2, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X2>
      >::type>
    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator*(x1, const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const MatrixExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,MatrixOpValue,VectorOpValue>>(x1,x2);
    }

  // operator/
    
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<DivOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    template<typename X1, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X1>
      >::type>
    inline ScalarExpr_ptr
    operator/(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator/(const_value(x1),x2);
    }

    template<typename X2, typename = typename std::enable_if<
        !std::is_base_of_v<VarBase,X2>
      >::type>
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator/(x1, const_value(x2));
    }

  // Other operators

    inline ScalarExpr_ptr
    sqr(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SqrOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr 
    sqrt(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SqrtOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    cos(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<CosOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    sin(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SinOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    tanh(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<TanhOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    abs(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AbsOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

  // Structural operators

    template<typename... X>
    inline VectorExpr_ptr
    vec(const std::shared_ptr<AnalyticExpr<X>>&... x)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,X...>>(x...);
    }

    template<typename... X>
    inline MatrixExpr_ptr
    mat(const std::shared_ptr<AnalyticExpr<X>>&... x)
    {
      return std::make_shared<AnalyticOperationExpr<MatrixOp,MatrixOpValue,X...>>(
        AnalyticOperationExpr<MatrixOp,MatrixOpValue,X...>(x...));
    }
}