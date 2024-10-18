/** 
 *  \file codac2_analytic_operations.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_AnalyticExpr.h"
#include "codac2_analytic_variables.h"
#include "codac2_analytic_constants.h"

namespace codac2
{
  using ScalarExpr_ptr = std::shared_ptr<AnalyticExpr<ScalarOpValue>>;
  using VectorExpr_ptr = std::shared_ptr<AnalyticExpr<VectorOpValue>>;
  using MatrixExpr_ptr = std::shared_ptr<AnalyticExpr<MatrixOpValue>>;

  template<class X>
  concept IsScalarExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<ScalarExpr_ptr,X>);

  template<class X>
  concept IsVectorExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<VectorExpr_ptr,X>);

  template<class X>
  concept IsMatrixExprOrVar = (std::is_base_of_v<VarBase,X> || std::is_base_of_v<MatrixExpr_ptr,X>);

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
    operator+(ScalarExpr_ptr x1, ScalarExpr_ptr x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsScalarExprOrVar<X1>)
    inline ScalarExpr_ptr
    operator+(const X1& x1, ScalarExpr_ptr x2)
    {
      return operator+(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsScalarExprOrVar<X2>)
    inline ScalarExpr_ptr
    operator+(ScalarExpr_ptr x1, const X2& x2)
    {
      return operator+(x1, const_value(x2));
    }

    inline VectorExpr_ptr
    operator+(VectorExpr_ptr x1, VectorExpr_ptr x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,VectorOpValue,VectorOpValue,VectorOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsVectorExprOrVar<X1>)
    inline VectorExpr_ptr
    operator+(const X1& x1, VectorExpr_ptr x2)
    {
      return operator+(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsVectorExprOrVar<X2>)
    inline VectorExpr_ptr
    operator+(VectorExpr_ptr x1, const X2& x2)
    {
      return operator+(x1, const_value(x2));
    }

    inline MatrixExpr_ptr
    operator+(MatrixExpr_ptr x1, MatrixExpr_ptr x2)
    {
      return std::make_shared<AnalyticOperationExpr<AddOp,MatrixOpValue,MatrixOpValue,MatrixOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsMatrixExprOrVar<X1>)
    inline MatrixExpr_ptr
    operator+(const X1& x1, MatrixExpr_ptr x2)
    {
      return operator+(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsMatrixExprOrVar<X2>)
    inline MatrixExpr_ptr
    operator+(MatrixExpr_ptr x1, const X2& x2)
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

    template<typename X1>
      requires (!IsScalarExprOrVar<X1>)
    inline ScalarExpr_ptr
    operator-(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator-(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsScalarExprOrVar<X2>)
    inline ScalarExpr_ptr
    operator-(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,VectorOpValue,VectorOpValue,VectorOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsVectorExprOrVar<X1>)
    inline VectorExpr_ptr
    operator-(const X1& x1, const VectorExpr_ptr& x2)
    {
      return operator-(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsVectorExprOrVar<X2>)
    inline VectorExpr_ptr
    operator-(const VectorExpr_ptr& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1, const MatrixExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<SubOp,MatrixOpValue,MatrixOpValue,MatrixOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsMatrixExprOrVar<X1>)
    inline MatrixExpr_ptr
    operator-(const X1& x1, const MatrixExpr_ptr& x2)
    {
      return operator-(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsMatrixExprOrVar<X2>)
    inline MatrixExpr_ptr
    operator-(const MatrixExpr_ptr& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

  // operator*
    
    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }
    
    inline VectorExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,ScalarOpValue,VectorOpValue>>(x1,x2);
    }
    
    inline VectorExpr_ptr
    operator*(const VectorExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,VectorOpValue,ScalarOpValue>>(x1,x2);
    }

    inline ScalarExpr_ptr
    operator*(const Interval& x1, const ScalarExpr_ptr& x2)
    {
      return operator*(const_value(x1),x2);
    }

    inline ScalarExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const Interval& x2)
    {
      return operator*(x1,const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const Interval& x1, const VectorExpr_ptr& x2)
    {
      return operator*(const_value(x1),x2);
    }

    inline VectorExpr_ptr
    operator*(const VectorExpr_ptr& x1, const Interval& x2)
    {
      return operator*(x1,const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const ScalarExpr_ptr& x1, const IntervalVector& x2)
    {
      return operator*(x1,const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const IntervalVector& x1, const ScalarExpr_ptr& x2)
    {
      return operator*(const_value(x1),x2);
    }

    inline VectorExpr_ptr
    operator*(const MatrixExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,MatrixOpValue,VectorOpValue>>(x1,x2);
    }

    inline VectorExpr_ptr
    operator*(const MatrixExpr_ptr& x1, const IntervalVector& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,MatrixOpValue,VectorOpValue>>(x1,const_value(x2));
    }

    inline VectorExpr_ptr
    operator*(const IntervalMatrix& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<MulOp,VectorOpValue,MatrixOpValue,VectorOpValue>>(const_value(x1),x2);
    }

  // operator/
    
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<DivOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsScalarExprOrVar<X1>)
    inline ScalarExpr_ptr
    operator/(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return operator/(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsScalarExprOrVar<X2>)
    inline ScalarExpr_ptr
    operator/(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return operator/(x1, const_value(x2));
    }
    
    inline VectorExpr_ptr
    operator/(const VectorExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<DivOp,VectorOpValue,VectorOpValue,ScalarOpValue>>(x1,x2);
    }

  // Other operators

    inline ScalarExpr_ptr
    pow(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<PowOp,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    template<typename X1>
      requires (!IsScalarExprOrVar<X1>)
    inline ScalarExpr_ptr
    pow(const X1& x1, const ScalarExpr_ptr& x2)
    {
      return pow(const_value(x1),x2);
    }

    template<typename X2>
      requires (!IsScalarExprOrVar<X2>)
    inline ScalarExpr_ptr
    pow(const ScalarExpr_ptr& x1, const X2& x2)
    {
      return pow(x1,const_value(x2));
    }

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
    exp(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<ExpOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    log(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<LogOp,ScalarOpValue,ScalarOpValue>>(x1);
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
    tan(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<TanOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    acos(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AcosOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    asin(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AsinOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    atan(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<AtanOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    atan2(const ScalarExpr_ptr& x1, const ScalarExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<Atan2Op,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(x1,x2);
    }

    inline ScalarExpr_ptr
    cosh(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<CoshOp,ScalarOpValue,ScalarOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    sinh(const ScalarExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<SinhOp,ScalarOpValue,ScalarOpValue>>(x1);
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

    inline ScalarExpr_ptr _add_to_vec(const ScalarExpr_ptr& x)
    {
      return x;
    }

    inline ScalarExpr_ptr _add_to_vec(double x)
    {
      return const_value(x);
    }

    template<class X>
    concept IsNotScalarExpr = !std::is_base_of_v<ScalarExpr_ptr,X>;

    template<typename X1>
      requires IsNotScalarExpr<X1>
    inline VectorExpr_ptr
    vec(const X1& x1)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue>>(
          _add_to_vec(x1));
    }

    template<typename X1, typename X2>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2));
    }

    template<typename X1, typename X2, typename X3>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3));
    }

    template<typename X1, typename X2, typename X3, typename X4>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename X9>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8> && IsNotScalarExpr<X9>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8, const X9& x9)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8),_add_to_vec(x9));
    }

    template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename X9, typename X10>
      requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8> && IsNotScalarExpr<X9> && IsNotScalarExpr<X10>)
    inline VectorExpr_ptr
    vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8, const X9& x9, const X10& x10)
    {
      return std::make_shared<AnalyticOperationExpr<VectorOp,VectorOpValue,
        ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue,ScalarOpValue>>(
          _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8),_add_to_vec(x9),_add_to_vec(x10));
    }

    // Generic variadic case, cannot handle const values (int, double) for now

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

  // Vectorial operations

    inline ScalarExpr_ptr
    det(const MatrixExpr_ptr& x1)
    {
      return std::make_shared<AnalyticOperationExpr<DetOp,ScalarOpValue,MatrixOpValue>>(x1);
    }

    inline ScalarExpr_ptr
    det(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2)
    {
      return std::make_shared<AnalyticOperationExpr<DetOp,ScalarOpValue,VectorOpValue,VectorOpValue>>(x1,x2);
    }

    inline ScalarExpr_ptr
    det(const VectorExpr_ptr& x1, const VectorExpr_ptr& x2, const VectorExpr_ptr& x3)
    {
      return std::make_shared<AnalyticOperationExpr<DetOp,ScalarOpValue,VectorOpValue,VectorOpValue,VectorOpValue>>(x1,x2,x3);
    }
}