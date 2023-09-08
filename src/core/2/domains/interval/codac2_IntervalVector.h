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
  class IntervalVector_ : public IntervalMatrix_<N,1>
  {
    public:
      
      IntervalVector_()
        : IntervalMatrix_<N,1>()
      { }

      IntervalVector_(size_t n)
        : IntervalMatrix_<N,1>(n,1)
      {
        assert(N == Dynamic || N == n);
      }
      
      template<int M>
      IntervalVector_(const Matrix_<M,1>& v)
        : IntervalMatrix_<N,1>(v.size(),1)
      {
        static_assert(N == M || N == -1 || M == -1);
        for(size_t i = 0 ; i < IntervalMatrix_<N,1>::size() ; i++)
          (*this)[i] = v[i];
      }
      
      //explicit IntervalVector_(const Interval& xi)
      //  : IntervalMatrix_<N,1>()
      //{
      //  (*this)(0,0) = xi;
      //}
      
      IntervalVector_(std::initializer_list<Interval> l)
        : IntervalMatrix_<N,1>(l.size(),1)
      {
        assert(l.size() == this->size());
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
      }

      template<int M>
      IntervalVector_(const IntervalMatrix_<M,1>& x)
        : IntervalMatrix_<M,1>(x)
      {
        assert(M == Dynamic || M == N);
      }

      template<typename OtherDerived>
      IntervalVector_(const Eigen::MatrixBase<OtherDerived>& other)
          : IntervalMatrix_<N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to IntervalVector_
      template<typename OtherDerived>
      IntervalVector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->IntervalMatrix_<N,1>::operator=(other);
        return *this;
      }

      static IntervalVector_<N> empty_set(size_t n = N)
      {
        IntervalVector_<N> x(n);
        x.set_empty();
        return x;
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
        assert(end_index < this->size() && start_index <= end_index);

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

      auto& operator+=(const IntervalVector_<N>& x)
      {
        (*this).noalias() += x;//.template cast<Interval>();
        return *this;
      }
      
      auto& operator-=(const IntervalVector_<N>& x)
      {
        (*this).noalias() -= x;//.template cast<Interval>();
        return *this;
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

      explicit IntervalVector(size_t n)
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