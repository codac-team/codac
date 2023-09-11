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
      {
        
      }

      Vector_(size_t n)
        : Matrix_<N,1>(n,1)
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
        assert(M == Dynamic || M == N);
      }

      explicit Vector_(size_t n, double values[])
        : Matrix_<N,1>(n,1,values)
      { }

      explicit Vector_(double values[])
        : Matrix_<N,1>(N,1,values)
      { }

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

      static Vector_<N> zeros()
      {
        return Eigen::Matrix<double,N,1>::Zero();
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
  Matrix_<N,N> diag(const Vector_<N> v)
  {
    return v.as_diag();
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