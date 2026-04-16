/** 
 *  \file codac2_TrajBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_Wrapper.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_ExprType.h"

namespace codac2
{
  template<typename T>
  class SampledTraj;

  template<typename T>
  class TrajBase
  {
    public:

      TrajBase()
      { }

      virtual Index size() const = 0;
      virtual std::pair<Index,Index> shape() const = 0;
      virtual bool is_empty() const = 0;
      virtual Interval tdomain() const = 0;
      virtual void truncate_tdomain(const Interval& new_tdomain) = 0;
      virtual typename Wrapper<T>::Domain codomain() const = 0;
      virtual T operator()(double t) const = 0;
      virtual typename Wrapper<T>::Domain operator()(const Interval& t) const = 0;

      // Implementation in codac2_TrajBase_impl.h
      auto nan_value() const;
      virtual SampledTraj<T> sampled(double dt) const;
      template<typename Q>
      SampledTraj<T> sampled_as(const SampledTraj<Q>& x) const;
      SampledTraj<T> primitive(double dt) const;
  };
}