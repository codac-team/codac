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

      auto nan_value() const
      {
        if constexpr(std::is_same_v<T,double> || std::is_same_v<typename ExprType<T>::Type,ScalarType>)
          return std::numeric_limits<double>::quiet_NaN();

        else
          return T((*this)(tdomain().lb())) // we obtain the output dimension by an evalution...
            .init(std::numeric_limits<double>::quiet_NaN());
      }

      virtual SampledTraj<T> sampled(double dt) const
      {
        assert_release(dt > 0.);
        assert_release(!is_empty());

        auto tdom = tdomain();
        SampledTraj<T> straj;
        for(double t = tdom.lb() ; t < tdom.ub() ; t+=dt)
          straj.set(t, (*this)(t));
        straj.set(tdom.ub(), (*this)(tdom.ub()));
        return straj;
      }

      template<typename Q>
      SampledTraj<T> sampled_as(const SampledTraj<Q>& x) const
      {
        assert_release(x.tdomain().is_subset(this->tdomain()));
        
        SampledTraj<T> straj;
        for(const auto& [ti,dump] : x)
          straj.set(ti, (*this)(ti));
        return straj;
      }

      SampledTraj<T> primitive(const T& y0, double dt) const
      {
        assert_release(dt > 0.);
        assert_release(!is_empty());

        SampledTraj<T> p;
        double t = tdomain().lb(), last_t = t;
        p.set(t, y0); t += dt;
        T y = y0;

        while(t < tdomain().ub())
        {
          y += ((*this)(last_t)+(*this)(t))*dt/2.;
          p.set(t, y);
          last_t = t;
          t += dt;
        }

        t = tdomain().ub();
        y += ((*this)(last_t)+(*this)(t))*(t-last_t)/2.;
        p.set(t, y);

        return p;
      }

      // Implementation in codac2_Trajectory_operator.h
      AnalyticFunction<typename ExprType<T>::Type> as_function() const;
  };
}
