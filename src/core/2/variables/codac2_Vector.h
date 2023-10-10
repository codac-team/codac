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

#ifndef __CODAC2_VECTOR_H__
#define __CODAC2_VECTOR_H__

#include <ostream>
#include <codac_Vector.h>
#include <codac2_Matrix.h>

namespace codac2
{
  using Eigen::Dynamic;

  template<int N=Dynamic>
  class Vector_ : public Matrix_<N,1>
  {
    public:
    
      Vector_()
      { }

      Vector_(size_t n)
        : Matrix_<N,1>(n,1)
      {
        assert(N == Dynamic || N == (int)n);
      }

      Vector_(size_t n, double x)
        : Matrix_<N,1>(n,1,x)
      {
        assert(N == Dynamic || N == (int)n);
      }

      Vector_(std::initializer_list<double> l) : Matrix_<N,1>(l.size(),1)
      {
        assert(N == (int)l.size() || N == -1);
        size_t i = 0;
        for(double li : l)
          (*this)(i++,0) = li;
      }

      template<int M>
      Vector_(const Matrix_<M,1>& x)
        : Matrix_<M,1>(x)
      {
        static_assert(M == Dynamic || M == N);
      }

      explicit Vector_(size_t n, double values[])
        : Matrix_<N,1>(n,1,values)
      { }

      explicit Vector_(double values[])
        : Matrix_<N,1>(N,1,values)
      { }

      template<typename T,size_t M>
      explicit Vector_(const std::array<T,M>& array)
        : Matrix_<N,1>(N,1)
      {
        static_assert(N == Dynamic || N == (int)M);
        for(size_t i = 0 ; i < M ; i++)
          *(this->data()+i) = array[i];
      }

      template<typename OtherDerived>
      Vector_(const Eigen::MatrixBase<OtherDerived>& other)
          : Matrix_<N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to Vector_
      template<typename OtherDerived>
      Vector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,N,1>::operator=(other);
        return *this;
      }

      Matrix_<N,N> as_diag() const
      {
        return Matrix_<N,N>(Eigen::Matrix<double,N,N>(this->asDiagonal()));
      }

      Matrix_<1,N> transpose() const
      {
        return Matrix_<1,N>(Eigen::Matrix<double,N,1>::transpose());
      }

      // todo: place this in common inheritance with IntervalVector_
      template<size_t N1,size_t N2>
      Vector_<N2-N1+1> subvector() const
      {
        assert(N1 >= 0 && N1 < N && N2 >= 0 && N2 < N && N1 <= N2);
        return this->template block<N2-N1+1,1>(N1,0);
      }
  };

  template<int N>
  std::ostream& operator<<(std::ostream& os, const Vector_<N>& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

  template<int N>
  Matrix_<N,N> diag(const Vector_<N> v)
  {
    return v.as_diag();
  }

  template<int N>
  codac::Vector to_codac1(const Vector_<N>& x)
  {
    ibex::Vector x_(x.size());
    for(size_t i = 0 ; i < x.size() ; i++)
      x_[i] = x[i];
    return x_;
  }

  template<int N>
  Vector_<N> to_codac2(const codac::Vector& x)
  {
    assert(x.size() == N);
    Vector_<N> x_;
    for(size_t i = 0 ; i < N ; i++)
      x_[i] = x[i];
    return x_;
  }

  class Vector : public Vector_<>
  {
    public:

      explicit Vector(int n)
        : Vector_<>(n)
      { }

      Vector(const Vector& x)
        : Vector_<>(x)
      { }
      
      explicit Vector(std::initializer_list<double> l)
        : Vector_<>(l)
      { }
      
      template<int N>
      Vector(const Vector_<N>& v)
        : Vector_<>(v)
      { }
      
      template<int N>
      Vector(const Matrix_<N,1>& v)
        : Vector_<>(v)
      { }
      
  };

} // namespace codac

#endif