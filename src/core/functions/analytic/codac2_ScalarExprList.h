/** 
 *  \file codac2_ScalarExprList.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  // Template parameter packs cannot be used for ScalarExprList because arguments are
  // provided by std::initializer_list (see template deduction guides for AnalyticFunction)

  template<typename S>
  concept IsScalar = std::is_same_v<S,ScalarExpr> || std::is_same_v<S,ScalarVar> ||
      std::is_same_v<typename ValueType<S>::Type,ScalarType>;

  struct ScalarExprList : public AnalyticExprWrapper<VectorType>
  {
    template<typename S1>
      requires IsScalar<S1>
    ScalarExprList(const S1& y1)
      : AnalyticExprWrapper<VectorType>(vec(to_ScalarExpr(y1)))
    { }

    template<typename S1,typename S2>
      requires IsScalar<S1> && IsScalar<S2>
    ScalarExprList(const S1& y1, const S2& y2)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1),to_ScalarExpr(y2)
      ))
    { }

    template<typename S1,typename S2,typename S3>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5,
      typename S6>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
        && IsScalar<S6>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5,
      const S6& y6)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5),
        to_ScalarExpr(y6)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5,
      typename S6,typename S7>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
        && IsScalar<S6> && IsScalar<S7>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5,
      const S6& y6, const S7& y7)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5),
        to_ScalarExpr(y6), to_ScalarExpr(y7)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5,
      typename S6,typename S7,typename S8>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
        && IsScalar<S6> && IsScalar<S7> && IsScalar<S8>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5,
      const S6& y6, const S7& y7, const S8& y8)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5),
        to_ScalarExpr(y6), to_ScalarExpr(y7), to_ScalarExpr(y8)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5,
      typename S6,typename S7,typename S8,typename S9>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
        && IsScalar<S6> && IsScalar<S7> && IsScalar<S8> && IsScalar<S9>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5,
      const S6& y6, const S7& y7, const S8& y8, const S9& y9)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5),
        to_ScalarExpr(y6), to_ScalarExpr(y7), to_ScalarExpr(y8), to_ScalarExpr(y9)
      ))
    { }

    template<typename S1,typename S2,typename S3,typename S4,typename S5,
      typename S6,typename S7,typename S8,typename S9,typename S10>
      requires IsScalar<S1> && IsScalar<S2> && IsScalar<S3> && IsScalar<S4> && IsScalar<S5>
        && IsScalar<S6> && IsScalar<S7> && IsScalar<S8> && IsScalar<S9> && IsScalar<S10>
    ScalarExprList(const S1& y1, const S2& y2, const S3& y3, const S4& y4, const S5& y5,
      const S6& y6, const S7& y7, const S8& y8, const S9& y9, const S10& y10)
      : AnalyticExprWrapper<VectorType>(vec(
        to_ScalarExpr(y1), to_ScalarExpr(y2), to_ScalarExpr(y3), to_ScalarExpr(y4), to_ScalarExpr(y5),
        to_ScalarExpr(y6), to_ScalarExpr(y7), to_ScalarExpr(y8), to_ScalarExpr(y9), to_ScalarExpr(y10)
      ))
    { }

    const ScalarExpr& to_ScalarExpr(const ScalarExpr& e)
    {
      return e;
    }

    template<typename T>
      requires std::is_same_v<typename ValueType<T>::Type,ScalarType>
    ScalarExpr to_ScalarExpr(const T& e)
    {
      return const_value(e);
    }
  };
}