/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTERVALVECTOR_H__
#define __CODAC2_INTERVALVECTOR_H__

#include <codac_Interval.h>
#include <codac_IntervalVector.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <ibex_LargestFirst.h>
#include "codac2_Interval.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  template<int N>
  class Vector_;

  using Eigen::Dynamic;

  template<int N=Dynamic>
  class IntervalVector_ : public Eigen::Matrix<Interval,N,1>
  {
    public:
      
      IntervalVector_()
        : Eigen::Matrix<Interval,N,1>()
      { }

      IntervalVector_(size_t n)
        : Eigen::Matrix<Interval,N,1>(n)
      {
        assert(N == Dynamic || N == n);
      }
      
      template<int M>
      explicit IntervalVector_(const Vector_<M>& v)
        : Eigen::Matrix<Interval,N,1>(v.size())
      {
        static_assert(N == M || N == -1 || M == -1);
        for(size_t i = 0 ; i < size() ; i++)
          (*this)[i] = v[i];
      }
      
      //explicit IntervalVector_(const Interval& xi)
      //  : Eigen::Matrix<Interval,N,1>()
      //{
      //  (*this)(0,0) = xi;
      //}
      
      IntervalVector_(std::initializer_list<Interval> l)
        : Eigen::Matrix<Interval,N,1>()
      {
        assert(l.size() == size());
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
      }

      template<typename OtherDerived>
      IntervalVector_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<Interval,N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to IntervalVector_
      template<typename OtherDerived>
      IntervalVector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<Interval,N,1>::operator=(other);
        return *this;
      }

      constexpr size_t size() const
      {
        return this->Eigen::Matrix<Interval,N,1>::size();
      }

      bool is_empty() const
      {
        for(size_t i = 0 ; i < size() ; i++)
          if((*this)[i].is_empty())
            return true;
        return false;
      }

      bool intersects(const IntervalVector_<N>& x) const
      {
        // todo: case of Dynamic vs Fixed
        for(size_t i = 0 ; i < size() ; i++)
          if(!(*this)[i].intersects(x[i]))
            return false;
        return true;
      }

      std::pair<IntervalVector_<N>,IntervalVector_<N>> bisect(float ratio = 0.49) const
      {
        assert(Interval(0,1).interior_contains(ratio));
        size_t i = largest_dim();
        assert((*this)[i].is_bisectable());

        auto p = std::make_pair(*this,*this);
        auto pi = (*this)[i].bisect(ratio);
        p.first[i] = pi.first;
        p.second[i] = pi.second;
        return p;
      }

      size_t largest_dim() const
      {
        return to_codac1(*this).extr_diam_index(false);
      }

      double volume() const
      {
        if(is_empty())
          return 0.;
        double v = 0.;
        for(size_t i = 0 ; i < size() ; i++)
        {
          if((*this)[i].is_unbounded()) return POS_INFINITY;
          if((*this)[i].is_degenerated()) return 0.;
          v += log((*this)[i].diam());
        }
        return exp(v);
      }

      Vector_<N> lb() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector_<N> lb(size());
        for(size_t i = 0 ; i < size() ; i++)
          lb[i] = (*this)[i].lb();
        return lb;
      }

      Vector_<N> ub() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector_<N> ub(size());
        for(size_t i = 0 ; i < size() ; i++)
          ub[i] = (*this)[i].ub();
        return ub;
      }

      Vector_<N> mid() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector_<N> m(size());
        for(size_t i = 0 ; i < size() ; i++)
          m[i] = (*this)[i].mid();
        return m;
      }

      void set_empty()
      {
        for(size_t i = 0 ; i < size() ; i++)
          (*this)[i].set_empty();
      }

      static IntervalVector_<N> empty_set(size_t n = N)
      {
        IntervalVector_<N> x(n);
        x.set_empty();
        return x;
      }

      IntervalVector_<N>& inflate(double r)
      {
        assert(r >= 0.);
        for(size_t i = 0 ; i < size() ; i++)
          (*this)[i].inflate(r);
        return *this;
      }

      IntervalVector_<N>& operator&=(const IntervalVector_<N>& x)
      {
        if(!is_empty())
        {
          if(x.is_empty())
            set_empty();

          else
            for(size_t i = 0 ; i < size() ; i++)
              (*this)[i] &= x[i];
        }
        return *this;
      }

      IntervalVector_<N>& operator|=(const IntervalVector_<N>& x)
      {
        if(!x.is_empty())
        {
          if(is_empty())
            *this = x;

          else
            for(size_t i = 0 ; i < size() ; i++)
              (*this)[i] |= x[i];
        }
        return *this;
      }

      IntervalVector_<N>& operator+=(const Vector_<N>& x)
      {
        (*this).noalias() += x.template cast<Interval>();
        return *this;
      }

      IntervalVector_<N>& operator-=(const Vector_<N>& x)
      {
        (*this).noalias() -= x.template cast<Interval>();
        return *this;
      }

      template<size_t N1,size_t N2>
      IntervalVector_<N2-N1+1> subvector() const
      {
        assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
        return this->template block<N2-N1+1,1>(N1,0);
      }

      IntervalVector_<> subvector(size_t start_index, size_t end_index) const
      {
        assert(end_index >= 0 && start_index >= 0);
        assert(end_index < size() && start_index <= end_index);

        IntervalVector_<> s(end_index-start_index+1);
        for(size_t i = 0 ; i < s.size() ; i++)
          s[i] = (*this)[i+start_index];
        return s;
      }

      template<size_t I,int M>
      void put(const IntervalVector_<M>& x)
      {
        assert(I >= 0 && I < N && M+I <= N);
        this->template block<M,1>(I,0) << x;
      }
  };

  template<int N>
  codac::IntervalVector to_codac1(const IntervalVector_<N>& x)
  {
    ibex::IntervalVector x_(x.size());
    for(size_t i = 0 ; i < x.size() ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<int N>
  IntervalVector_<N> to_codac2(const codac::IntervalVector& x)
  {
    assert(x.size() == N);
    IntervalVector_<N> x_(x.size());
    for(size_t i = 0 ; i < x.size() ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<int N>
  IntervalVector_<N+1> cart_prod(const IntervalVector_<N>& x1, const Interval& x2)
  {
    IntervalVector_<N+1> x;
    x << x1,x2;
    return x;
  }

  template<int N>
  IntervalVector_<N+1> cart_prod(const Interval& x1, const IntervalVector_<N>& x2)
  {
    IntervalVector_<N+1> x;
    x << x1,x2;
    return x;
  }

  template<int N,int M>
  IntervalVector_<N+M> cart_prod(const IntervalVector_<N>& x1, const IntervalVector_<M>& x2)
  {
    IntervalVector_<N+M> x;
    x << x1,x2;
    return x;
  }

  template<typename T1, typename T2, typename... Args>
  auto cart_prod(const T1& x1, const T2& x2, const Args&... xi) // recursive variadic function
  {
    auto x_ = cart_prod(x1, x2);
    if constexpr(sizeof...(xi) > 0)
      return cart_prod(x_, xi...);
    else
      return x_;
  }

  class IntervalVector : public IntervalVector_<>
  {
    public:

      explicit IntervalVector(int n)
        : IntervalVector_<>(n)
      { }

      IntervalVector(const IntervalVector_<>& x)
        : IntervalVector_<>(x)
      { }
      
      template<int N>
      explicit IntervalVector(const Vector_<N>& v)
        : IntervalVector_<>(v)
      {

      }
  };

} // namespace codac

#endif