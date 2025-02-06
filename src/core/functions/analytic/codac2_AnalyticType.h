/** 
 *  \file codac2_AnalyticType.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  struct AnalyticTypeBase
  {
    virtual ~AnalyticTypeBase() = default;
  };

  template<typename S, typename T, typename M>
  struct AnalyticType : public AnalyticTypeBase
  {
    using Scalar = S;
    using Domain = T;

    T m;
    T a;
    M da;
    bool def_domain;

    AnalyticType() = delete;

    AnalyticType(const T& a_, bool def_domain_)
      : a(a_), def_domain(def_domain_)
    { }

    AnalyticType(const T& m_, const T& a_, const M& da_, bool def_domain_)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    AnalyticType<S,T,M>& operator&=(const AnalyticType<S,T,M>& x)
    {
      a &= x.a;
      // restore this? da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }
  };

  using ScalarType = AnalyticType<double,Interval,IntervalMatrix>;
  using VectorType = AnalyticType<Vector,IntervalVector,IntervalMatrix>;
  using MatrixType = AnalyticType<Matrix,IntervalMatrix,IntervalMatrix>;

  template<typename... T>
  bool centered_form_not_available_for_args(const T&... a)
  {
    return ((a.da.size() == 0) || ...);
  }
}