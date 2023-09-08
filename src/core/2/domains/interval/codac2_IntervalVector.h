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

#include <type_traits>
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

      explicit IntervalVector_(size_t n)
        : IntervalMatrix_<N,1>(n,1)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit IntervalVector_(size_t n, const Interval& x)
        : IntervalMatrix_<N,1>(n,1,x)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit IntervalVector_(const Interval& x)
        : IntervalMatrix_<N,1>(1,1,x)
      { }
      
      template<int M>
      explicit IntervalVector_(const Matrix_<M,1>& v)
        : IntervalMatrix_<N,1>(v.size(),1)
      {
        static_assert(N == M || N == -1 || M == -1);
        for(size_t i = 0 ; i < IntervalMatrix_<N,1>::size() ; i++)
          (*this)[i] = v[i];
      }
      
      explicit IntervalVector_(size_t n, double bounds[][2])
        : IntervalMatrix_<N,1>(n,1)
      {
        for(size_t i = 0 ; i < n ; i++)
        {
          if(bounds == 0) // in case the user called IntervalVector(n,0) and 0 is interpreted as NULL!
            (*this)[i] = Interval::zero();
          else
            (*this)[i] = Interval(bounds[i][0],bounds[i][1]);
        }
      }
      
      explicit IntervalVector_(double bounds[][2])
        : IntervalVector_(this->size(), bounds)
      {
        
      }
      
      IntervalVector_(std::initializer_list<Interval> l)
        : IntervalMatrix_<N,1>(l.size(),1)
      {
        assert(N == Dynamic || (int)l.size() == N);
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
        // todo: use thias as faster? std::copy(l.begin(), l.end(), vec);
      }

      template<int M>
      explicit IntervalVector_(const IntervalMatrix_<M,1>& x)
        : IntervalMatrix_<M,1>(x)
      {
        assert(M == Dynamic || M == N);
      }

      // This constructor allows you to construct IntervalVector_ from Eigen expressions
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

      static IntervalVector_<N> empty_set()
      {
        IntervalVector_<N> x;
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

  class IntervalVector : public IntervalVector_<>
  {
    public:

      explicit IntervalVector(size_t n)
        : IntervalVector_<>(n)
      { }

      explicit IntervalVector(size_t n, const Interval& x)
        : IntervalVector_<>(n, x)
      { }

      explicit IntervalVector(const Interval& x)
        : IntervalVector_<>({x})
      { }

      explicit IntervalVector(const IntervalVector_<>& x)
        : IntervalVector_<>(x)
      { }
      
      template<int N>
      explicit IntervalVector(const Vector_<N>& v)
        : IntervalVector_<>(v)
      { }
      
      explicit IntervalVector(size_t n, double bounds[][2])
        : IntervalVector_<>(n, bounds)
      { }
      
      IntervalVector(std::initializer_list<Interval> l)
        : IntervalVector_<>(l)
      { }

      // This constructor allows you to construct IntervalVector from Eigen expressions
      template<typename OtherDerived>
      IntervalVector(const Eigen::MatrixBase<OtherDerived>& other)
          : IntervalVector_<>(other)
      { }
 
      // This method allows you to assign Eigen expressions to IntervalVector
      template<typename OtherDerived>
      IntervalVector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->IntervalVector_<>::operator=(other);
        return *this;
      }
  
      void resize(size_t n)
      {
        // With resize of Eigen, the data is reallocated and all previous values are lost.
        auto save = *this;
        IntervalVector_<>::resize(n);
        for(size_t i = 0 ; i < min(save.size(),n) ; i++)
          (*this)[i] = save[i];
      }

      static IntervalVector empty_set(size_t n)
      {
        return IntervalVector(n, Interval::empty_set());
      }
  };

} // namespace codac

#endif