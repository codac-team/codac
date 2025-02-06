/** 
 *  \file codac2_AnalyticTraj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticFunction.h"
#include "codac2_TrajBase.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  template<typename O, typename S = typename O::Scalar>
  class AnalyticTraj : public TrajBase<S>, public AnalyticFunction<O>
  {
    public:

      AnalyticTraj(const AnalyticFunction<O>& f, const Interval& tdomain)
        : TrajBase<S>(), AnalyticFunction<O>(f), _tdomain(tdomain)
      {
        assert_release(f.args().total_size() == 1 && "domain of f must be 1d");
      }

      virtual Index size() const
      {
        return AnalyticFunction<O>::output_size();
      }

      virtual bool is_empty() const
      {
        return _tdomain.is_empty();
      }

      virtual Interval tdomain() const
      {
        return _tdomain;
      }

      virtual void truncate_tdomain(const Interval& new_tdomain)
      {
        assert_release(this->tdomain().is_superset(new_tdomain));
        _tdomain &= new_tdomain;
      }

      virtual typename Wrapper<S>::Domain codomain() const
      {
        return AnalyticFunction<O>::eval(this->_tdomain);
      }

      virtual S operator()(double t) const
      {
        if(!this->tdomain().contains(t))
          return this->nan_value();
        return AnalyticFunction<O>::real_eval(t);
      }

      virtual typename Wrapper<S>::Domain operator()(const Interval& t) const
      {
        if(!this->tdomain().is_superset(t))
          return typename Wrapper<S>::Domain((*this)(tdomain().lb())) // we obtain the output dimension by an evalution...
            .init(Interval(-oo,oo));
        return AnalyticFunction<O>::eval(t);
      }

    protected:

      Interval _tdomain;
  };
}