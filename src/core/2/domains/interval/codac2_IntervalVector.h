/** 
 *  \file
 *  
 *  This class reuses many of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: G. Chabert)
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

#include <list>
#include <type_traits>
#include <codac_Interval.h>
#include <codac_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include "codac2_Interval.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Vector.h"
//#include "codac2_IntervalVectorComponent.h"
#include <codac2_eigen.h>

namespace codac2
{
  template<int N>
  class Vector_;

  /*template<int N>
  class IntervalVectorComponent_;

  template<int N>
  class IntervalVectorConstComponent_;*/

  class IntervalVectorSubvector;
  class IntervalVectorConstSubvector;

  using Eigen::Dynamic;

  template<int N=Dynamic>
  class IntervalVector_ : public IntervalMatrix_<N,1>
  {
    public:

      //using VECTOR_ITEM = IntervalVectorComponent_<N>;
      
      IntervalVector_()
        : IntervalMatrix_<N,1>()
      {
        // todo: restore this: static_assert(N != Dynamic);
      }

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

      //explicit IntervalVector_(const Interval& x)
      //  : IntervalMatrix_<N,1>(N,1,x)
      //{ }
      
      template<int M>
      explicit IntervalVector_(const Vector_<M>& v)
        : IntervalVector_<N>(v.size())
      {
        static_assert(N == M || N == -1 || M == -1);
        for(size_t i = 0 ; i < (size_t)v.size() ; i++)
          (*this)[i] = Interval(v[i]);
      }
      
      explicit IntervalVector_(size_t n, const double bounds[][2])
        : IntervalMatrix_<N,1>(n,1,bounds)
      { }

      explicit IntervalVector_(const double bounds[][2])
        : IntervalVector_(this->size(), bounds)
      { }
      
      explicit IntervalVector_(const Vector_<N>& lb, const Vector_<N>& ub)
        : IntervalMatrix_<N,1>(lb, ub)
      { }
      
      IntervalVector_(std::initializer_list<Interval> l)
        : IntervalMatrix_<N,1>(l.size(),1)
      {
        assert(N == Dynamic || (int)l.size() == N);
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
        // todo: use thias as faster? std::copy(l.begin(), l.end(), vec);
      }

      template<int M=Dynamic>
      IntervalVector_(const IntervalVector_<M>& x)
        : IntervalVector_(x.size())
      {
        static_assert(M == Dynamic || M == N);
        for(size_t i = 0 ; i < x.size() ; i++)
          (*this)[i] = x[i];
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

      void resize(size_t n)
      {
        this->IntervalMatrix_<N,1>::resize(n,1);
      }

      template<size_t N1,size_t N2>
      IntervalVector_<N2-N1+1> subvector() const
      {
        static_assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
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
        static_assert(I >= 0 && I < N && M+I <= N);
        this->template block<M,1>(I,0) << x;
      }

      IntervalVector_<N>& operator+=(const IntervalVector_<N>& x)
      {
        (*this).noalias() += x;//.template cast<Interval>();
        return *this;
      }
      
      IntervalVector_<N>& operator-=(const IntervalVector_<N>& x)
      {
        (*this).noalias() -= x;//.template cast<Interval>();
        return *this;
      }

      std::list<IntervalVector_<N>> complementary() const
      {
        return IntervalVector_<N>(this->size()).diff(*this);
      }

      std::list<IntervalVector_<N>> diff(const IntervalVector_<N>& y, bool compactness = true) const
      {
        // This code originates from the ibex-lib
        // See: ibex_TemplateVector.h
        // Author: Gilles Chabert
        // It has been revised with modern C++ and templated types

        const size_t n = this->size();
        assert(y.size() == n);

        if(y == *this)
          return { IntervalVector_<N>::empty_set(n) };

        IntervalVector_<N> x = *this;
        IntervalVector_<N> z = x & y;

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

        std::list<IntervalVector_<N>> l;

        for(size_t var = 0 ; var < n ; var++)
        {
          Interval c1, c2;
          x[var].diff(y[var], c1, c2, compactness);

          if(!c1.is_empty())
          {
            IntervalVector_<N> v(n);
            for(size_t i = 0 ; i < var ; i++)
              v[i] = x[i];
            v[var] = c1;
            for(size_t i = var+1 ; i < n ; i++)
              v[i] = x[i];
            l.push_back(v);

            if(!c2.is_empty())
            {
              IntervalVector_<N> w(n);
              for(size_t i = 0 ; i < var ; i++)
                w[i] = x[i];
              w[var] = c2;
              for(size_t i = var+1 ; i<n ; i++)
                w[i] = x[i];
              l.push_back(w);
            }

            x[var] = z[var];
          }
        }

        return l;
      }

    public:

      /*IntervalVectorComponent_<N> operator[](size_t i)
      {
        return IntervalVectorComponent_<N>(*this, i);
      }
      
      IntervalVectorConstComponent_<N> operator[](size_t i) const
      {
        return IntervalVectorConstComponent_<N>(*this, i);
      }*/

      /*Interval& i(size_t i)
      {
        return Eigen::Matrix<Interval,N,1>::operator[](i);
      }

      const Interval& i(size_t i) const
      {
        return Eigen::Matrix<Interval,N,1>::operator[](i);
      }

      friend IntervalVectorComponent_<N>;
      friend IntervalVectorConstComponent_<N>;*/
      friend IntervalVectorSubvector;
  };

  template<int N=Dynamic>
  std::ostream& operator<<(std::ostream& os, const IntervalVector_<N>& x)
  {
    if(x.is_empty()) return os << "empty vector";
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

  template<int N=Dynamic>
  codac::IntervalVector to_codac1(const IntervalVector_<N>& x)
  {
    ibex::IntervalVector x_(x.size());
    for(size_t i = 0 ; i < x.size() ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<int N=Dynamic>
  IntervalVector_<N> to_codac2(const codac::IntervalVector& x)
  {
    assert(x.size() == N);
    IntervalVector_<N> x_(x.size());
    for(size_t i = 0 ; i < (size_t)x.size() ; i++)
      x_[i] = x[i];
    return x_;
  }


  /*class IntervalVector : public IntervalVector_<>
  {
    public:

      using VECTOR_ITEM = IntervalVectorComponent_;

      explicit IntervalVector(size_t n);
      explicit IntervalVector(size_t n, const Interval& x);
      //explicit IntervalVector(const Interval& x);
      explicit IntervalVector(const IntervalVector& x);
      
      template<int N>
      explicit IntervalVector(const Vector_<N>& v)
        : IntervalVector_<>(v)
      { }
      
      explicit IntervalVector(size_t n, const double bounds[][2]);
      IntervalVector(std::initializer_list<Interval> l);

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

      IntervalVectorComponent_ operator[](size_t i);
      IntervalVectorSubvector operator[](std::array<size_t,2> i);
      IntervalVectorConstComponent operator[](size_t i) const;
      IntervalVectorConstSubvector operator[](std::array<size_t,2> i) const;
      void resize(size_t n);
      static IntervalVector empty_set(size_t n);

      virtual void set_name(const std::string& name)
      {
        #ifndef NDEBUG
        Domain::set_name(name);
        for(size_t i = 0 ; i < size() ; i++)
          (*this)[i].set_name(name + "[" + std::to_string(i) + "]");
        #endif
      }
  };*/

 /* template<int N=Dynamic>
  class IntervalVectorComponent_ : public Domain
  {
    public:

      IntervalVectorComponent_(IntervalVector_<N>& x, size_t i)
        : _x(x), _i(i)
      {
        set_name(x.name() + "[" + std::to_string(i) + "]");
      }

      IntervalVectorComponent_<N>& operator=(const IntervalVectorComponent_<N>& xi)
      {
        _x.i(_i) = xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator=(const Interval& xi)
      {
        _x.i(_i) = xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator&=(const Interval& xi)
      {
        _x.i(_i) &= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator|=(const Interval& xi)
      {
        _x.i(_i) |= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator+=(const Interval& xi)
      {
        _x.i(_i) += xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator-=(const Interval& xi)
      {
        _x.i(_i) -= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator*=(const Interval& xi)
      {
        _x.i(_i) *= xi;
        return *this;
      }

      IntervalVectorComponent_<N>& operator/=(const Interval& xi)
      {
        _x.i(_i) /= xi;
        return *this;
      }

      template<typename T>
      Interval operator+(const T& x) const
      {
        return operator const Interval&() + x;
      }

      template<typename T>
      Interval operator-(const T& x) const
      {
        return operator const Interval&() - x;
      }

      Interval& operator()()
      {
        return _x.i(_i);
      }

      const Interval& operator()() const
      {
        return _x.i(_i);
      }

      operator Interval&()
      {
        return _x.i(_i);
      }

      operator const Interval&() const
      {
        return _x.i(_i);
      }

      IntervalVectorComponent_<N>& inflate(double r)
      {
        _x.i(_i).inflate(r);
        return *this;
      }

      friend std::ostream& operator<<(std::ostream& os, const IntervalVectorComponent_<N>& p)
      {
        os << p.operator const Interval&();
        return os;
      }

    //protected:

      IntervalVector_<N>& _x;
      const size_t _i;
  };*/

//  class IntervalVectorConstComponent : public Interval
//  {
//    public:
//
//      IntervalVectorConstComponent(const IntervalVector& x, size_t i);
//      const Interval& operator()() const;
//
//    protected:
//
//      const IntervalVector& _x;
//      const size_t _i;
//  };

//  class IntervalVectorSubvector : public IntervalVector
//  {
//    public:
//
//      explicit IntervalVectorSubvector(IntervalVector& x, const std::array<size_t,2>& i);
//      //IntervalVectorSubvector(const IntervalVectorSubvector& xi) = delete;
//
//      IntervalVectorSubvector& operator=(const IntervalVectorSubvector& xi);
//      const IntervalVectorSubvector& operator=(const IntervalVectorSubvector& xi) const;
//      IntervalVectorSubvector& operator=(const IntervalVector& xi);
//      const IntervalVectorSubvector& operator=(const IntervalVector& xi) const;
//      IntervalVectorSubvector& operator&=(const IntervalVector& xi);
//      IntervalVectorSubvector& operator|=(const IntervalVector& xi);
//      IntervalVectorSubvector& operator+=(const IntervalVector& xi);
//      IntervalVectorSubvector& operator-=(const IntervalVector& xi);
//      IntervalVectorSubvector& operator*=(const IntervalVector& xi);
//      IntervalVectorSubvector& operator/=(const IntervalVector& xi);
//
//    //protected:
//
//      IntervalVector& _x;
//      const std::array<size_t,2> _i;
//  };
//
//  class IntervalVectorConstSubvector : public IntervalVector
//  {
//    public:
//
//      explicit IntervalVectorConstSubvector(const IntervalVector& x, const std::array<size_t,2>& i);
//
//    protected:
//
//      const IntervalVector& _x;
//      const std::array<size_t,2> _i;
//  };

  using IntervalVector = IntervalVector_<>;

} // namespace codac

#endif