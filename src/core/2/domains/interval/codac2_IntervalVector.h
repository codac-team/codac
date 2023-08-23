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
#include "codac2_Vector.h"

namespace codac2
{
  template<size_t N>
  class IntervalVector : public Eigen::Matrix<Interval,N,1>
  {
    public:
      
      IntervalVector()
        : Eigen::Matrix<Interval,N,1>()
      {

      }
      
      explicit IntervalVector(const Vector<N>& v)
        : Eigen::Matrix<Interval,N,1>()
      {
        for(size_t i = 0 ; i < N ; i++)
          (*this)[i] = v[i];
      }
      
      explicit IntervalVector(const Interval& xi)
        : Eigen::Matrix<Interval,N,1>()
      {
        (*this)(0,0) = xi;
      }
      
      IntervalVector(std::initializer_list<Interval> l)
        : Eigen::Matrix<Interval,N,1>()
      {
        assert(l.size() == N);
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
      }

      template<typename OtherDerived>
      IntervalVector(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<Interval,N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to IntervalVector
      template<typename OtherDerived>
      IntervalVector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<Interval,N,1>::operator=(other);
        return *this;
      }

      constexpr size_t size() const
      {
        return N;
      }

      bool is_empty() const
      {
        for(size_t i = 0 ; i < N ; i++)
          if((*this)(i,0).is_empty())
            return true;
        return false;
      }

      std::pair<IntervalVector<N>,IntervalVector<N>> bisect(float ratio) const
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
        for(size_t i = 0 ; i < N ; i++)
        {
          if((*this)[i].is_unbounded()) return POS_INFINITY;
          if((*this)[i].is_degenerated()) return 0.;
          v += log((*this)[i].diam());
        }
        return exp(v);
      }

      Vector<N> lb() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector<N> lb;
        for(size_t i = 0 ; i < N ; i++)
          lb[i] = (*this)[i].lb();
        return lb;
      }

      Vector<N> ub() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector<N> ub;
        for(size_t i = 0 ; i < N ; i++)
          ub[i] = (*this)[i].ub();
        return ub;
      }

      Vector<N> mid() const
      {
        assert(!is_empty()); // todo: use nan instead of assert?
        Vector<N> m;
        for(size_t i = 0 ; i < N ; i++)
          m[i] = (*this)[i].mid();
        return m;
      }

      void set_empty()
      {
        for(size_t i = 0 ; i < N ; i++)
          (*this)[i].set_empty();
      }

      static IntervalVector<N> empty_set()
      {
        IntervalVector<N> x;
        x.set_empty();
        return x;
      }

      IntervalVector<N>& operator&=(const IntervalVector<N>& x)
      {
        if(!is_empty())
        {
          if(x.is_empty())
            set_empty();

          else
            for(size_t i = 0 ; i < N ; i++)
              (*this)[i] &= x[i];
        }
        return *this;
      }

      IntervalVector<N>& operator|=(const IntervalVector<N>& x)
      {
        if(!x.is_empty())
        {
          if(is_empty())
            *this = x;

          else
            for(size_t i = 0 ; i < N ; i++)
              (*this)[i] |= x[i];
        }
        return *this;
      }

      IntervalVector<N>& operator+=(const Vector<N>& x)
      {
        (*this).noalias() += x.template cast<Interval>();
        return *this;
      }

      IntervalVector<N>& operator-=(const Vector<N>& x)
      {
        (*this).noalias() -= x.template cast<Interval>();
        return *this;
      }

      template<size_t N1,size_t N2>
      IntervalVector<N2-N1+1> subvector() const
      {
        assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
        return this->template block<N2-N1+1,1>(N1,0);
      }

      template<size_t I,size_t M>
      void put(const IntervalVector<M>& x)
      {
        assert(I >= 0 && I < N && M+I <= N);
        this->template block<M,1>(I,0) << x;
      }
  };

  template<size_t N>
  codac::IntervalVector to_codac1(const IntervalVector<N>& x)
  {
    ibex::IntervalVector x_(N);
    for(size_t i = 0 ; i < N ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<size_t N>
  IntervalVector<N> to_codac2(const codac::IntervalVector& x)
  {
    assert(x.size() == N);
    IntervalVector<N> x_;
    for(size_t i = 0 ; i < N ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<size_t N>
  IntervalVector<N+1> cart_prod(const IntervalVector<N>& x1, const Interval& x2)
  {
    IntervalVector<N+1> x;
    x << x1,x2;
    return x;
  }

  template<size_t N>
  IntervalVector<N+1> cart_prod(const Interval& x1, const IntervalVector<N>& x2)
  {
    IntervalVector<N+1> x;
    x << x1,x2;
    return x;
  }

  template<size_t N,size_t M>
  IntervalVector<N+M> cart_prod(const IntervalVector<N>& x1, const IntervalVector<M>& x2)
  {
    IntervalVector<N+M> x;
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

} // namespace codac

#endif