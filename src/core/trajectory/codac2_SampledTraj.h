/** 
 *  \file codac2_SampledTraj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_TrajBase.h"
#include "codac2_analytic_variables.h"
#include "codac2_template_tools.h"
#include "codac2_Traj_operator.h"

namespace codac2
{
  template<typename T>
  class SampledTraj : public TrajBase<T>, public std::map<double,T>
  {
    public:

      using Type = typename ExprType<T>::Type;

      SampledTraj()
        : TrajBase<T>(), std::map<double,T>()
      { }

      SampledTraj(const std::list<double>& l_t, const std::list<T>& l_x)
        : SampledTraj()
      {
        assert_release(l_t.size() == l_x.size());
        auto it_t = l_t.begin(); auto it_x = l_x.begin();
        while(it_t != l_t.end())
        {
          this->set(*it_t,*it_x);
          it_t++; it_x++;
        }
      }

      SampledTraj(const std::map<double,T>& m)
        : TrajBase<T>(), std::map<double,T>(m)
      { }

      // size is not the std::map<double,T>::size() !
      virtual Index size() const
      {
        if constexpr(std::is_same_v<typename ExprType<T>::Type,ScalarType>)
          return 1;

        else
        {
          if(this->empty())
            return 0;
          else
            return this->begin()->second.size();
        }
      }

      virtual std::pair<Index,Index> shape() const
      {
        if constexpr(std::is_same_v<typename ExprType<T>::Type,ScalarType>)
          return {1,1};
        else
        {
          if(this->empty())
            return {0,0};
          else
            return {this->begin()->second.rows(),this->begin()->second.cols()};
        }
      }

      size_t nb_samples() const
      {
        return std::map<double,T>::size();
      }

      virtual bool is_empty() const
      {
        return std::map<double,T>::empty();
      }

      virtual Interval tdomain() const
      {
        if(this->empty())
          return Interval::empty();
        else
          return { this->begin()->first, this->rbegin()->first };
      }

      virtual void truncate_tdomain(const Interval& new_tdomain)
      {
        assert_release(this->tdomain().is_superset(new_tdomain));

        // Interpolation on the limits of the new tdomain
        T y_lb = (*this)(new_tdomain.lb());
        T y_ub = (*this)(new_tdomain.ub());

        auto it = this->begin();
        while(it != this->end())
        {
          if(!new_tdomain.contains(it->first))
            it = this->erase(it);
          else
            ++it;
        }

        this->set(new_tdomain.lb(), y_lb); // clean truncation
        this->set(new_tdomain.ub(), y_ub);
      }

      virtual typename Wrapper<T>::Domain codomain() const
      {
        typename Wrapper<T>::Domain hull(this->begin()->second);
        for(const auto& [t,v] : *this)
          hull |= v;
        return hull;
      }

      virtual T operator()(double t) const
      {
        if(!this->tdomain().contains(t))
          return this->nan_value();

        auto it_lower = this->lower_bound(t);
        if(it_lower->first == t)
          return it_lower->second;

        auto it_upper = it_lower;
        it_lower--;

        // Linear interpolation
        return it_lower->second +
               (t - it_lower->first) * (it_upper->second - it_lower->second) /
               (it_upper->first - it_lower->first);
      }

      virtual typename Wrapper<T>::Domain operator()(const Interval& t) const
      {
        // We obtain the output dimension by an evalution...
        typename Wrapper<T>::Domain hull(this->begin()->second);

        if(!this->tdomain().is_superset(t))
          return hull.init(Interval(-oo,oo));

        else
        {
          hull = (*this)(t.lb());
          for(auto it = this->lower_bound(t.lb()) ; it != this->upper_bound(t.ub()) ; it++)
            hull |= it->second;
          hull |= (*this)(t.ub());
          return hull;
        }
      }

      void set(double t, const T& x)
      {
        assert(this->empty() || size_of(x) == this->size());
        std::map<double,T>::operator[](t) = x;
      }

      virtual SampledTraj<T> sampled(double dt) const
      {
        return sampled(dt, true);
      }

      SampledTraj<T> sampled(double dt, bool keep_original_values) const
      {
        assert(dt > 0.);
        assert(!is_empty());

        auto straj = TrajBase<T>::sampled(dt);

        if(keep_original_values)
        {
          // Appending values from the initial map:
          for(const auto& [ti,xi] : *this)
            straj.set(ti, xi);
        }
        
        return straj;
      }

      template<typename Q>
      SampledTraj<T> sampled_as(const SampledTraj<Q>& x) const
      {
        return TrajBase<T>::sampled_as(x);
      }

      template<typename Q>
      SampledTraj<T> sampled_as(const SampledTraj<Q>& x, bool keep_original_values) const
      {
        SampledTraj<T> straj = TrajBase<T>::sampled_as(x);
        if(keep_original_values)
          for(const auto& [ti,xi] : *this)
            straj.set(ti, xi);
        return straj;
      }

      SampledTraj<T>& shift_tdomain(double shift)
      {
        std::map<double,T> save = *this;
        this->clear();
        for(const auto& [ti,xi] : save)
          this->std::map<double,T>::operator[](ti+shift) = xi;
        return *this;
      }

      SampledTraj<T>& stretch_tdomain(const Interval& tdomain)
      {
        Interval a = this->tdomain(), b = tdomain;
        std::map<double,T> save = *this;
        this->clear();
        for(const auto& [ti,xi] : save)
            this->std::map<double,T>::operator[]([&]() {
              if(ti == a.ub())
                return b.ub(); // due to floating point possible error
              else
                return ((ti-a.lb())*b.diam()/a.diam())+b.lb();
            }()
          ) = xi;
        assert(this->tdomain() == tdomain);
        return *this;
      }

      template<typename T_=T>
        requires std::is_same_v<T_,Vector>
      SampledTraj<double> operator[](Index i) const
      {
        assert_release(i >= 0 && i < size());
        std::map<double,double> m;
        for(const auto& [t,y] : *this)
        {
          assert(i < y.size());
          m[t] = y[i];
        }

        return { m };
      }

      template<typename T_=T>
        requires std::is_same_v<T_,Vector>
      SampledTraj<Vector> subvector(Index i, Index j) const
      {
        assert_release(i >= 0 && i <= j && j < size());
        std::map<double,Vector> m;
        for(const auto& [t,y] : *this)
        {
          assert(j < y.size());
          m[t] = y.subvector(i,j);
        }

        return { m };
      }

      AnalyticFunction<typename ExprType<T>::Type> as_function() const
      {
        ScalarVar t;
        return {{t},
          AnalyticExprWrapper<typename ExprType<T>::Type>(
            std::make_shared<AnalyticOperationExpr<
              TrajectoryOp<SampledTraj<T>>,typename ExprType<T>::Type,ScalarType>>(*this,t))
        };
      }
  };
  
  template<typename T>
  inline std::ostream& operator<<(std::ostream& os, const SampledTraj<T>& x)
  {
    os << "SampledTraj. " << x.tdomain() << "↦" << x.codomain() << ", " << x.nb_samples() << " pts";
    return os;
  }

  inline SampledTraj<double> continuous_traj(const SampledTraj<double>& x)
  {
    SampledTraj<double> x_continuous;
    const Interval periodicity = x.codomain();

    double prev_xi = 0., value_mod = 0.;

    for(const auto& [ti,xi] : x)
    {
      if(!x_continuous.empty())
      {
        if(prev_xi - xi > periodicity.diam()*0.9)
          value_mod += periodicity.diam();
        else if(prev_xi - xi < -periodicity.diam()*0.9)
          value_mod -= periodicity.diam();
      }

      prev_xi = xi;
      x_continuous.set(ti, xi+value_mod);
    }

    return x_continuous;
  }
}
