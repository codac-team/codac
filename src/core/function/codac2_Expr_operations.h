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

#ifndef __CODAC2_EXPR_OPERATIONS__
#define __CODAC2_EXPR_OPERATIONS__

#include <memory>
#include "codac2_Expr.h"
#include "codac2_Arg.h"
#include "codac2_ConstValue.h"

namespace codac2
{
  // operator+ (unary case)
    
    inline std::shared_ptr<Expr<Interval>>
    operator+(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,Interval,Interval>>(x1);
    }

    inline std::shared_ptr<Expr<IntervalVector>>
    operator+(const std::shared_ptr<Expr<IntervalVector>>& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalVector,IntervalVector>>(x1);
    }

    inline std::shared_ptr<Expr<IntervalMatrix>>
    operator+(const std::shared_ptr<Expr<IntervalMatrix>>& x1)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalMatrix,IntervalMatrix>>(x1);
    }

  // operator+
    
    inline std::shared_ptr<Expr<Interval>>
    operator+(const std::shared_ptr<Expr<Interval>>& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,Interval,Interval,Interval>>(x1, x2);
    }

    inline std::shared_ptr<Expr<IntervalVector>>
    operator+(const std::shared_ptr<Expr<IntervalVector>>& x1, const std::shared_ptr<Expr<IntervalVector>>& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalVector,IntervalVector,IntervalVector>>(x1, x2);
    }

    inline std::shared_ptr<Expr<IntervalMatrix>>
    operator+(const std::shared_ptr<Expr<IntervalMatrix>>& x1, const std::shared_ptr<Expr<IntervalMatrix>>& x2)
    {
      return std::make_shared<OperationExpr<CtcAdd,IntervalMatrix,IntervalMatrix,IntervalMatrix>>(x1, x2);
    }

    template<typename X1>
    inline std::shared_ptr<Expr<Interval>>
    operator+(const X1& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return operator+(const_value(x1), x2);
    }

    template<typename X2>
    inline std::shared_ptr<Expr<Interval>>
    operator+(const std::shared_ptr<Expr<Interval>>& x1, const X2& x2)
    {
      return operator+(x1, const_value(x2));
    }

  // operator- (unary case)
    
    inline std::shared_ptr<Expr<Interval>>
    operator-(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,Interval,Interval>>(x1);
    }

    inline std::shared_ptr<Expr<IntervalVector>>
    operator-(const std::shared_ptr<Expr<IntervalVector>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalVector,IntervalVector>>(x1);
    }

    inline std::shared_ptr<Expr<IntervalMatrix>>
    operator-(const std::shared_ptr<Expr<IntervalMatrix>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalMatrix,IntervalMatrix>>(x1);
    }

  // operator-
    
    inline std::shared_ptr<Expr<Interval>>
    operator-(const std::shared_ptr<Expr<Interval>>& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,Interval,Interval,Interval>>(x1, x2);
    }

    inline std::shared_ptr<Expr<IntervalVector>>
    operator-(const std::shared_ptr<Expr<IntervalVector>>& x1, const std::shared_ptr<Expr<IntervalVector>>& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalVector,IntervalVector,IntervalVector>>(x1, x2);
    }

    inline std::shared_ptr<Expr<IntervalMatrix>>
    operator-(const std::shared_ptr<Expr<IntervalMatrix>>& x1, const std::shared_ptr<Expr<IntervalMatrix>>& x2)
    {
      return std::make_shared<OperationExpr<CtcSub,IntervalMatrix,IntervalMatrix,IntervalMatrix>>(x1, x2);
    }

    template<typename X1>
    inline std::shared_ptr<Expr<Interval>>
    operator-(const X1& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return operator-(const_value(x1), x2);
    }

    template<typename X2>
    inline std::shared_ptr<Expr<Interval>>
    operator-(const std::shared_ptr<Expr<Interval>>& x1, const X2& x2)
    {
      return operator-(x1, const_value(x2));
    }

  // operator*
    
    inline std::shared_ptr<Expr<Interval>>
    operator*(const std::shared_ptr<Expr<Interval>>& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return std::make_shared<OperationExpr<CtcMul,Interval,Interval,Interval>>(x1, x2);
    }

    template<typename X1>
    inline std::shared_ptr<Expr<Interval>>
    operator*(const X1& x1, const std::shared_ptr<Expr<Interval>>& x2)
    {
      return operator*(const_value(x1), x2);
    }

    template<typename X2>
    inline std::shared_ptr<Expr<Interval>>
    operator*(const std::shared_ptr<Expr<Interval>>& x1, const X2& x2)
    {
      return operator*(x1, const_value(x2));
    }

    inline std::shared_ptr<Expr<IntervalVector>>
    operator*(const std::shared_ptr<Expr<IntervalMatrix>>& x1, const std::shared_ptr<Expr<IntervalVector>>& x2)
    {
      return std::make_shared<OperationExpr<CtcMul,IntervalVector,IntervalMatrix,IntervalVector>>(x1, x2);
    }

  // Other operators

    inline std::shared_ptr<Expr<Interval>>
    sqr(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSqr,Interval,Interval>>(x1);
    }

    inline std::shared_ptr<Expr<Interval>> 
    sqrt(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSqrt,Interval,Interval>>(x1);
    }

    inline std::shared_ptr<Expr<Interval>>
    cos(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcCos,Interval,Interval>>(x1);
    }

    inline std::shared_ptr<Expr<Interval>>
    sin(const std::shared_ptr<Expr<Interval>>& x1)
    {
      return std::make_shared<OperationExpr<CtcSin,Interval,Interval>>(x1);
    }

  // Structural operators

    template<typename... X>
    inline std::shared_ptr<Expr<IntervalVector>>
    vec(const std::shared_ptr<Expr<X>>&... x)
    {
      return std::make_shared<OperationExpr<CtcVector,IntervalVector,X...>>(x...);
    }

    template<typename... X>
    inline std::shared_ptr<Expr<IntervalMatrix>>
    mat(const std::shared_ptr<Expr<X>>&... x)
    {
      return std::make_shared<OperationExpr<CtcMatrix,IntervalMatrix,X...>>(
        OperationExpr<CtcMatrix,IntervalMatrix,X...>(x...));
    }
}

#endif