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
#include "codac2_Traj_operator.h"

namespace codac2
{
  template<typename T>
  class AnalyticTraj : public TrajBase<typename T::Scalar>, public AnalyticFunction<T>
  {
    public:

      using Type = T;

      AnalyticTraj(const AnalyticFunction<T>& f, const Interval& tdomain)
        : TrajBase<typename T::Scalar>(), AnalyticFunction<T>(f), _tdomain(tdomain)
      {
        assert_release(f.args().total_size() == 1 && "domain of f must be 1d");
      }

      virtual Index size() const
      {
        return AnalyticFunction<T>::output_size();
      }

      virtual std::pair<Index,Index> shape() const
      {
        return AnalyticFunction<T>::output_shape();
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

      virtual typename T::Domain codomain() const
      {
        return AnalyticFunction<T>::eval(this->_tdomain);
      }

      virtual typename T::Scalar operator()(double t) const
      {
        if(!this->tdomain().contains(t))
          return this->nan_value();
        return AnalyticFunction<T>::real_eval(t);
      }

      virtual typename T::Domain operator()(const Interval& t) const
      {
        if(!this->tdomain().is_superset(t))
          return typename T::Domain((*this)(tdomain().lb())) // we obtain the output dimension by an evalution...
            .init(Interval(-oo,oo));
        return AnalyticFunction<T>::eval(t);
      }
  
      AnalyticFunction<T> as_function() const
      {
        ScalarVar t;
        return {{t},
          AnalyticExprWrapper<T>(
            std::make_shared<AnalyticOperationExpr<
              TrajectoryOp<AnalyticTraj<T>>,T,ScalarType>>(*this,t))
        };
      }

    protected:

      Interval _tdomain;
  };
}
