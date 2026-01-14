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
#include "codac2_math.h"
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
          this->set(*it_x, *it_t);
          it_t++; it_x++;
        }
      }

      SampledTraj(const std::map<double,T>& m)
        : TrajBase<T>(), std::map<double,T>(m)
      { }

      // size is not the std::map<double,T>::size() !
      virtual Index size() const
      {
        if constexpr(std::is_same_v<Type,ScalarType>)
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
        if constexpr(std::is_same_v<Type,ScalarType>)
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

        this->set(y_lb, new_tdomain.lb()); // clean truncation
        this->set(y_ub, new_tdomain.ub());
      }

      virtual typename Wrapper<T>::Domain codomain() const
      {
        if(is_empty())
        {
          if constexpr(std::is_same_v<T,double>)
            return Interval::empty();
          else if constexpr(std::is_same_v<T,Vector>)
              return IntervalVector::empty(0);
          else
            return IntervalMatrix::empty(0,0);
        }

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

      void set(const T& x, double t)
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
            straj.set(xi, ti);
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
            straj.set(xi, ti);
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

      AnalyticFunction<Type> as_function() const
      {
        ScalarVar t;
        return {{t},
          AnalyticExprWrapper<Type>(
            std::make_shared<AnalyticOperationExpr<
              TrajectoryOp<SampledTraj<T>>,Type,ScalarType>>(*this,t))
        };
      }

      using TrajBase<T>::primitive;
      SampledTraj<T> primitive() const
      {
        T s = [this]() {
          if constexpr(std::is_same_v<T,double>)
            return 0.;
          else
            return T(this->begin()->second).init(0.);
        }();
        SampledTraj<T> p;
        p.set(s, this->begin()->first);

        for(auto it = std::next(this->begin()) ; it != this->end() ; it++)
        {
          s += (prev(it)->second + it->second) * (it->first - prev(it)->first) / 2.;
          p.set(s, it->first);
        }

        return p;
      }

      SampledTraj<T> derivative() const
      {
        SampledTraj<T> d;
        assert_release(this->nb_samples() >= 3);

        auto it1 = this->begin();
        auto it2 = std::next(it1);

        // First value (forward)
        {
          auto it3 = std::next(it2);
          double t1 = it1->first, t2 = it2->first, t3 = it3->first;
          const T& x1 = it1->second; const T& x2 = it2->second; const T& x3 = it3->second;
          double dt1 = t2-t1, dt2 = t3-t2;

          // Derivative at t1
          T num = std::pow(dt1,2)*(x2-x1)/dt1 + std::pow(dt2,2)*(x3-x2)/dt2;
          d.set(2*num/std::pow(dt1+dt2,2), t1);
        }

        // Intermediate values
        {
          auto it_prev = this->begin();
          auto it = std::next(it_prev);
          auto it_next = std::next(it);

          for(; it_next != this->end(); ++it_prev, ++it, ++it_next)
          {
            double t_prev = it_prev->first;
            double t = it->first;
            double t_next = it_next->first;

            const T& x_prev = it_prev->second;
            const T& x = it->second;
            const T& x_next = it_next->second;
            double dt_prev = t-t_prev, dt_next = t_next-t;

            T num = std::pow(dt_prev,2)*(x_next-x) / dt_next + std::pow(dt_next,2)*(x-x_prev) / dt_prev;
            d.set(2*num/std::pow(dt_prev+dt_next,2), t);
          }
        }

        // Last value (backward)
        {
          auto it3 = std::prev(this->end());
          auto it2b = std::prev(it3);
          auto it1b = std::prev(it2b);

          double t1 = it1b->first, t2 = it2b->first, t3 = it3->first;
          const T& x1 = it1b->second; const T& x2 = it2b->second; const T& x3 = it3->second;
          double dt1 = t2-t1, dt2 = t3-t2;

          // Derivative at t3 (t last)
          T num = std::pow(dt2,2)*(x3-x2)/dt2 + std::pow(dt1,2)*(x2-x1)/dt1;
          d.set(2*num/std::pow(dt1+dt2,2), t3);
        }

        return d;
      }

      T mean() const
      {
        if(this->empty())
          return this->nan_value();

        auto it = this->begin();
        double t_prev = it->first;
        T x_prev = it->second;

        T sum = x_prev;
        double total_time = 0.;

        for(++it; it != this->end(); ++it)
        {
          double dt = it->first - t_prev;
          sum += (x_prev + it->second) * 0.5 * dt;
          total_time += dt;

          t_prev = it->first;
          x_prev = it->second;
        }

        return sum / total_time;
      }

      template<typename X1, typename X2>
      static bool same_sampling(const SampledTraj<X1>& x1, const SampledTraj<X2>& x2)
      {
        auto it1 = x1.cbegin();
        auto it2 = x2.cbegin();

        while(it1 != x1.cend() && it2 != x2.cend())
        {
          if(it1->first != it2->first)
            return false;
          it1++; it2++;
        }

        return it1 == x1.cend() && it2 == x2.cend();
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
      x_continuous.set(xi+value_mod, ti);
    }

    return x_continuous;
  }

  inline std::vector<SampledTraj<double>> as_scalar_trajs(const SampledTraj<Vector>& x)
  {
    std::vector<SampledTraj<double>> v(x.size());
    for(const auto& [ti,xi] : x)
      for(Index i = 0 ; i < x.size() ; i++)
        v[i].set(xi[i],ti);
    return v;
  }

  template<typename... X>
    requires ((std::is_same_v<SampledTraj<double>,X> || std::is_same_v<SampledTraj<Vector>,X>) && ...)
  inline SampledTraj<Vector> cart_prod(const X&... x)
  {
    auto&& x0 = std::get<0>(std::forward_as_tuple(x...));
    assert_release((SampledTraj<Vector>::same_sampling(x0, x) && ...));
    SampledTraj<Vector> y;
    for(auto it = x0.begin() ; it != x0.end() ; it++)
      y.set(cart_prod(x.at(it->first)...), it->first);
    return y;
  }
}
