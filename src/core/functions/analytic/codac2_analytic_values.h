/** 
 *  \file
 *  Var class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

namespace codac2
{
  struct OpValueBase
  {
    virtual ~OpValueBase() = default;
  };

  template<typename T>
  struct OpValue : public OpValueBase
  {
    using Domain = T;

    T m;
    T a;
    IntervalMatrix da;
    bool def_domain;

    OpValue() = delete;

    OpValue(const T& m_, const T& a_, const IntervalMatrix& da_, bool def_domain_)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    OpValue<T>& operator&=(const OpValue<T>& x)
    {
      a &= x.a;
      // restore this? da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }
  };

  using ScalarOpValue = OpValue<Interval>;
  using VectorOpValue = OpValue<IntervalVector>;
  using MatrixOpValue = OpValue<IntervalMatrix>;

  template<typename T>
  struct Wrapper
  { };

  template<>
  struct Wrapper<int> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<size_t> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<double> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<Interval> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<Vector> {
    using Domain = VectorOpValue;
  };

  template<>
  struct Wrapper<IntervalVector> {
    using Domain = VectorOpValue;
  };
}