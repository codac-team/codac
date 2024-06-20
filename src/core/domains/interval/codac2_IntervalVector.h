/** 
 *  \file codac2_IntervalVector.h
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
#include "codac2_IntervalVectorTemplate.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  template<int N=Dynamic>
  class IntervalVector_ : public IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>
  {
    public:

      IntervalVector_()
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>()
      { }

      explicit IntervalVector_(size_t n)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(n)
      { }

      explicit IntervalVector_(size_t n, const Interval& x)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(n,x)
      { }

      explicit IntervalVector_(const Interval& x)
        : IntervalVector_<N>(1,x)
      { }
      
      template<int M>
      explicit IntervalVector_(const Vector_<M>& v)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(v)
      { }
      
      explicit IntervalVector_(size_t n, const double bounds[][2])
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(n,bounds)
      { }

      explicit IntervalVector_(const double bounds[][2])
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(bounds)
      { }

      explicit IntervalVector_(const std::vector<Interval>& x)
        : IntervalVector_(x.size(), &x[0])
      {
        assert(!x.empty());
      }
      
      explicit IntervalVector_(const Vector_<N>& lb, const Vector_<N>& ub)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(lb, ub)
      { }
      
      IntervalVector_(std::initializer_list<Interval> l)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(l)
      { }

      template<int M=Dynamic>
      IntervalVector_(const IntervalVector_<M>& x)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(x)
      { }

      template<typename OtherDerived>
      IntervalVector_(const Eigen::MatrixBase<OtherDerived>& other)
        : IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>(other)
      { }
 
      template<typename OtherDerived>
      IntervalVector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->IntervalVectorTemplate_<IntervalVector_<N>,Vector_<N>,N>::operator=(other);
        return *this;
      }
  };

  class IntervalVector : public IntervalVectorTemplate_<IntervalVector,Vector>
  {
    public:
      
      explicit IntervalVector(size_t n)
        : IntervalVectorTemplate_<IntervalVector,Vector>(n)
      { }

      explicit IntervalVector(size_t n, const Interval& x)
        : IntervalVectorTemplate_<IntervalVector,Vector>(n,x)
      { }

      explicit IntervalVector(const Interval& x)
        : IntervalVector(1,x)
      { }
      
      template<int M>
      explicit IntervalVector(const Vector_<M>& v)
        : IntervalVectorTemplate_<IntervalVector,Vector>(v)
      { }
      
      explicit IntervalVector(size_t n, const double bounds[][2])
        : IntervalVectorTemplate_<IntervalVector,Vector>(n,bounds)
      { }
      
      explicit IntervalVector(const Vector& lb, const Vector& ub)
        : IntervalVectorTemplate_<IntervalVector,Vector>(lb, ub)
      { }
      
      IntervalVector(std::initializer_list<Interval> l)
        : IntervalVectorTemplate_<IntervalVector,Vector>(l)
      { }

      template<int M=Dynamic>
      IntervalVector(const IntervalVector_<M>& x)
        : IntervalVectorTemplate_<IntervalVector,Vector>(x)
      { }

      explicit IntervalVector(const IntervalVector& x)
        : IntervalVector(x.size())
      {
        *this = x;
      }

      template<typename OtherDerived>
      IntervalVector(const Eigen::MatrixBase<OtherDerived>& other)
        : IntervalVectorTemplate_<IntervalVector,Vector>(other)
      { }
 
      template<typename OtherDerived>
      IntervalVector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->IntervalVectorTemplate_<IntervalVector,Vector>::operator=(other);
        return *this;
      }

      IntervalMatrix transpose() const
      {
        return IntervalVectorTemplate_<IntervalVector,Vector>::transpose().eval();
      }

      IntervalMatrix as_diag() const
      {
        IntervalMatrix diag(size(),size(),0.);
        for(size_t i = 0 ; i < (size_t)size() ; i++)
          diag(i,i) = (*this)[i];
        return diag;
      }

      template<typename OtherDerived>
      IntervalVector operator+(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator+(x));
      }

      template<typename OtherDerived>
      IntervalVector operator+=(const Eigen::MatrixBase<OtherDerived>& x)
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator+=(x));
      }

      template<typename OtherDerived>
      IntervalVector operator+=(const Vector& x)
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator+=(x.template cast<Interval>()));
      }

      IntervalVector operator-() const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator-());
      }

      template<typename OtherDerived>
      IntervalVector operator-(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator-(x));
      }

      template<typename OtherDerived>
      IntervalVector operator-=(const Eigen::MatrixBase<OtherDerived>& x)
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator-=(x));
      }

      template<typename OtherDerived>
      IntervalVector operator-=(const Vector& x)
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator-=(x.template cast<Interval>()));
      }

      template<typename OtherDerived>
      IntervalVector operator*(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator*(x));
      }

      template<typename OtherDerived>
      IntervalVector operator&(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator&(x));
      }
      
      template<typename OtherDerived>
      IntervalVector operator|(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalVector(IntervalVectorTemplate_<IntervalVector,Vector>::operator|(x));
      }
  };

  /*inline Vector operator*(int a, const Vector& x)
  {
    return a * (MatrixTemplate_<Matrix_<Dynamic,1>,double,Dynamic,1>)(x);
  }

  inline Vector operator*(double a, const Vector& x)
  {
    return a * (MatrixTemplate_<Matrix_<Dynamic,1>,double,Dynamic,1>)(x);
  }

  inline IntervalVector operator*(const Interval& a, const Vector& x)
  {
    return a * x.template cast<Interval>();
  }*/

  inline IntervalVector operator|(const Vector& x, const Vector& y)
  {
    return IntervalVector(x).operator|(y);
  }

  template<typename... X, typename = typename std::enable_if<(true && ... && (
      (std::is_same_v<Interval,X> || std::is_same_v<IntervalVector,X> || std::is_same_v<Vector,X>)
    )), void>::type>
  inline IntervalVector cart_prod(const X&... x)
  {
    size_t n = 0;
    ((n += x.size()), ...);
    IntervalVector x_(n);

    size_t i = 0;

    auto increm = [](size_t& i, size_t n)
    {
      size_t i_ = i;
      i += n;
      return i_;
    };

    (x_.put(increm(i,x.size()),IntervalVector(x)), ...);
    return x_;
  }
}