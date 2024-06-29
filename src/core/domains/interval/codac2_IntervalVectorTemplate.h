/** 
 *  \file codac2_IntervalVectorTemplate.h
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
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Vector.h"
#include "codac2_IntervalMatrixTemplate.h"

namespace codac2
{
  #if 0
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

      static S empty(size_t n = N)
      {
        //assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::empty(n,1);
      }

      static S zeros(size_t n = N)
      {
        //assert(n > 0);
        assert(N == Dynamic || N == (int)n);
        return IntervalMatrixTemplate_<IntervalVectorTemplate_<S,V,N>,V,N,1>::zeros(n,1);
      }

      static S ones(size_t n = N)
      {
        //assert(n > 0);
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
  #endif 
}