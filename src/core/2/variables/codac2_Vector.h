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

#include <unsupported/Eigen/MatrixFunctions>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <codac_Vector.h>
#include <codac2_Matrix.h>

namespace codac2
{
  using Eigen::Dynamic;

  template<int N=Dynamic>
  class Vector_ : public Eigen::Matrix<double,N,1>
  {
    public:
    
      Vector_()
      {
        
      }

      Vector_(size_t n)
        : Eigen::Matrix<double,N,1>(n)
      {
        assert(N == Dynamic || N == n);
      }

      Vector_(std::initializer_list<double> l) : Eigen::Matrix<double,N,1>(l.size())
      {
        assert(N == l.size() || N == -1);
        size_t i = 0;
        for(double li : l)
          (*this)(i++,0) = li;
      }
    
      template<typename OtherDerived>
      Vector_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<double,N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to MyVector_Type
      template<typename OtherDerived>
      Vector_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,N,1>::operator=(other);
        return *this;
      }

      Matrix<N,N> as_diag() const
      {
        return Matrix<N,N>(Eigen::Matrix<double,N,N>(this->asDiagonal()));
      }

      static Vector_<N> zeros()
      {
        Vector_<N> v;
        return v;
      }
  };

  template<int N>
  Matrix<N,N> diag(const Vector_<N> v)
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

      Vector(const Vector_<>& x)
        : Vector_<>(x)
      { }
      
      explicit Vector(std::initializer_list<double> l)
        : Vector_<>(l)
      { }
      
      template<int N>
      explicit Vector(const Vector_<N>& v)
        : Vector_<>(v)
      {

      }
  };

} // namespace codac

#endif