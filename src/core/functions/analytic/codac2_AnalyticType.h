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
#include "codac2_Wrapper.h"

namespace codac2
{
  struct AnalyticTypeBase
  {
    virtual ~AnalyticTypeBase() = default;
  };

  template<typename T,typename D>
  struct AnalyticType : public AnalyticTypeBase
  {
    using Scalar = T;
    using Domain = D;

    D m;
    D a;
    IntervalMatrix da;
    bool def_domain;

    AnalyticType() = delete;

    AnalyticType(const D& a_, bool def_domain_)
      : a(a_), def_domain(def_domain_)
    { }

    AnalyticType(const D& m_, const D& a_, const IntervalMatrix& da_, bool def_domain_)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    AnalyticType<T,D>& operator&=(const AnalyticType<T,D>& x)
    {
      a &= x.a;
      // restore this? da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }
  };

  using ScalarType = AnalyticType<double,Interval>;
  using VectorType = AnalyticType<Vector,IntervalVector>;
  using MatrixType = AnalyticType<Matrix,IntervalMatrix>;

  template<typename... T>
  bool centered_form_not_available_for_args(const T&... a)
  {
    return ((a.da.size() == 0) || ...);
  }
}