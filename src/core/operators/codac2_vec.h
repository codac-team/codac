/** 
 *  \file codac2_vector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct VectorOp
  {
    template<typename... X>
    static inline std::string str(const X&... x)
    {
      std::string s = (("\t" + x->str() + ",\n") + ...);
      s.pop_back(); s.pop_back(); // removes last separation
      return "[\n" + s + "\n]";
    }

    template<typename... X>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X&... x)
    {
      return { sizeof...(X), 1 };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline IntervalVector fwd(const X&... x)
    {
      return IntervalVector({x...});
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_natural(const X&... x)
    {
      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);
      
      return {
        fwd(x.a...),
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<ScalarType,X> && ...)
    static inline VectorType fwd_centered(const X&... x)
    {
      if(centered_form_not_available_for_args(x...))
        return fwd_natural(x...);

      IntervalMatrix d(sizeof...(X),std::get<0>(std::tie(x...)).da.cols());
      Index i = 0;
      ((d.row(i++) = x.da), ...);

      bool def_domain = true;
      ((def_domain &= x.def_domain), ...);
      
      return {
        fwd(x.m...),
        fwd(x.a...),
        d,
        def_domain
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<Interval,X> && ...)
    static inline void bwd(const IntervalVector& y, X&... x)
    {
      Index i = 0;
      ((x &= y[i++]), ...);
    }
  };

  // Analytic operator
  // The following functions can be used to build analytic expressions.

  inline ScalarExpr _add_to_vec(const ScalarExpr& x)
  {
    return x;
  }

  inline ScalarExpr _add_to_vec(double x)
  {
    return const_value(x);
  }

  template<class X>
  concept IsNotScalarExpr = !std::is_base_of_v<ScalarExpr,X>;

  template<typename X1>
    requires IsNotScalarExpr<X1>
  inline VectorExpr
  vec(const X1& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType>>(
        _add_to_vec(x1)) };
  }

  template<typename X1, typename X2>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2)) };
  }

  template<typename X1, typename X2, typename X3>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3)) };
  }

  template<typename X1, typename X2, typename X3, typename X4>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename X9>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8> && IsNotScalarExpr<X9>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8, const X9& x9)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8),_add_to_vec(x9)) };
  }

  template<typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename X9, typename X10>
    requires (IsNotScalarExpr<X1> && IsNotScalarExpr<X2> && IsNotScalarExpr<X3> && IsNotScalarExpr<X4> && IsNotScalarExpr<X5> && IsNotScalarExpr<X6> && IsNotScalarExpr<X7> && IsNotScalarExpr<X8> && IsNotScalarExpr<X9> && IsNotScalarExpr<X10>)
  inline VectorExpr
  vec(const X1& x1, const X2& x2, const X3& x3, const X4& x4, const X5& x5, const X6& x6, const X7& x7, const X8& x8, const X9& x9, const X10& x10)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,
      ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType,ScalarType>>(
        _add_to_vec(x1),_add_to_vec(x2),_add_to_vec(x3),_add_to_vec(x4),_add_to_vec(x5),_add_to_vec(x6),_add_to_vec(x7),_add_to_vec(x8),_add_to_vec(x9),_add_to_vec(x10)) };
  }

  // Generic variadic case, cannot handle const values (int, double) for now

  template<typename... X>
  inline VectorExpr
  vec(const AnalyticExprWrapper<X>&... x)
  {
    return { std::make_shared<AnalyticOperationExpr<VectorOp,VectorType,X...>>(x...) };
  }
}
