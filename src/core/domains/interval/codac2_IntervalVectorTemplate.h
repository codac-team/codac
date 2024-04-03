/** 
 *  \file
 *  IntervalVector class
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Vector.h"
#include "codac2_IntervalMatrixTemplate.h"

namespace codac2
{
  template<int M>
  class IntervalVector_;

  template<typename S,typename V,int N=Dynamic>
  class IntervalVectorTemplate_ : public IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>
  {
    public:

      IntervalVectorTemplate_()
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>()
      { }

      explicit IntervalVectorTemplate_(size_t n)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(n,1)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit IntervalVectorTemplate_(size_t n, const Interval& x)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(n,1,x)
      {
        assert(N == Dynamic || N == (int)n);
      }

      explicit IntervalVectorTemplate_(const Interval& x)
        : IntervalVectorTemplate_(1,x)
      { }
      
      template<int M>
      explicit IntervalVectorTemplate_(const Vector_<M>& v)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(v.size(),1)
      {
        static_assert(N == M || N == Dynamic || M == Dynamic);
        for(size_t i = 0 ; i < (size_t)v.size() ; i++)
          (*this)[i] = Interval(v[i]);
      }
      
      explicit IntervalVectorTemplate_(size_t n, const double bounds[][2])
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(n,1,bounds)
      { }

      explicit IntervalVectorTemplate_(const double bounds[][2])
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(N, bounds)
      {
        static_assert(N != Dynamic);
      }
      
      explicit IntervalVectorTemplate_(const Vector_<N>& lb, const Vector_<N>& ub)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(lb, ub)
      { }
      
      IntervalVectorTemplate_(std::initializer_list<Interval> l)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(l.size(),1)
      {
        assert(N == Dynamic || (int)l.size() == N);
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
        // todo: use thias as faster? std::copy(l.begin(), l.end(), vec);
      }

      template<typename SM,typename VM,int M=Dynamic>
      IntervalVectorTemplate_(const IntervalVectorTemplate_<SM,VM,M>& x)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(x.size())
      {
        static_assert(M == Dynamic || M == N);
        for(size_t i = 0 ; i < x.size() ; i++)
          (*this)[i] = x[i];
      }

      template<typename OtherDerived>
      IntervalVectorTemplate_(const Eigen::MatrixBase<OtherDerived>& other)
        : IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>(other)
      { }
 
      template<typename OtherDerived>
      IntervalVectorTemplate_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::operator=(other);
        return *this;
      }

      void resize(size_t n)
      {
        static_assert(N == Dynamic);
        this->IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::resize(n,1);
      }

      template<size_t N1,size_t N2>
      auto subvector() const
      {
        static_assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
        return this->template block<N2-N1+1,1>(N1,0);
      }

      S subvector(size_t start_id, size_t end_id) const
      {
        assert(end_id >= 0 && start_id >= 0);
        assert(end_id < this->size() && start_id <= end_id);
        return this->block(start_id, 0, end_id-start_id+1, 1);
      }

      template<size_t I,int M>
      void put(const IntervalVector_<M>& x)
      {
        static_assert(I >= 0 && I < N && M+I <= N && M != Dynamic);
        this->template block<M,1>(I,0) << x;
      }

      template<typename T, typename = typename std::enable_if<
          std::is_base_of_v<IntervalVectorTemplate_,T>
        >::type>
      void put(size_t start_id, const T& x)
      {
        assert(start_id >= 0 && start_id < this->size());
        assert(start_id+x.size() <= this->size());
        this->block(start_id,0,x.size(),1) << x;
      }

      auto as_diag() const
      {
        return Eigen::Matrix<Interval,N,N>(this->asDiagonal());
      }

      std::vector<S> complementary() const
      {
        return S(this->size()).diff(*this);
      }

      std::vector<S> diff(const S& y, bool compactness = true) const
      {
        // This code originates from the ibex-lib
        // See: ibex_TemplateVector.h
        // Author: Gilles Chabert
        // It has been revised with modern C++ and templated types

        const size_t n = this->size();
        assert(y.size() == n);

        if(y == *this)
          return { S::empty(n) };

        S x = *this;
        S z = x & y;

        if(z.is_empty())
          return { x };

        else
        {
          // Check if in one dimension y is flat and x not,
          // in which case the diff returns also x directly
          if(compactness)
            for(size_t i = 0 ; i < n ; i++)
              if(z[i].is_degenerated() && !x[i].is_degenerated())
                return { x };
        }

        std::vector<S> l;

        for(size_t var = 0 ; var < n ; var++)
        {
          Interval c1, c2;
          
          for(const auto& ci : x[var].diff(y[var], compactness))
          {
            assert(!ci.is_empty());

            S v(n);
            for(size_t i = 0 ; i < var ; i++)
              v[i] = x[i];
            v[var] = ci;
            for(size_t i = var+1 ; i < n ; i++)
              v[i] = x[i];
            l.push_back(v);
          }

          x[var] = z[var];
        }

        return l;
      }

      static S empty(size_t n = N)
      {
        assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::empty(n,1);
      }

      static S zeros(size_t n = N)
      {
        assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::zeros(n,1);
      }

      static S ones(size_t n = N)
      {
        assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::ones(n,1);
      }
  };

  template<typename S,typename V,int N=Dynamic>
  std::ostream& operator<<(std::ostream& os, const IntervalVectorTemplate_<S,V,N>& x)
  {
    if(x.is_empty()) return os << "empty vector";
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

  template<typename S,typename V,int N=Dynamic>
  inline auto diag(const IntervalVectorTemplate_<S,V,N>& v)
  {
    return v.as_diag();
  }
}