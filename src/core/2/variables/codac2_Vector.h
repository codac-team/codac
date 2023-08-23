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
  template<size_t N>
  class Vector : public Eigen::Matrix<double,N,1>
  {
    public:
    
      Vector()
      {
        
      }

      Vector(double l) : Vector({ l })
      {
      
      }

      Vector(std::initializer_list<double> l)
      {
        size_t i = 0;
        for(double li : l)
          (*this)(i++,0) = li;
      }
    
      template<typename OtherDerived>
      Vector(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<double,N,1>(other)
      { }

      // This method allows you to assign Eigen expressions to MyVectorType
      template<typename OtherDerived>
      Vector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,N,1>::operator=(other);
        return *this;
      }

      Matrix<N,N> as_diag() const
      {
        return Matrix<N,N>(Eigen::Matrix<double,N,N>(this->asDiagonal()));
      }

      static Vector<N> zeros()
      {
        Vector<N> v;
        return v;
      }
  };

  template<size_t N>
  Matrix<N,N> diag(const Vector<N> v)
  {
    return v.as_diag();
  }

  template<size_t N>
  Vector<N> to_codac2(const codac::Vector& x)
  {
    assert(x.size() == N);
    Vector<N> x_;
    for(size_t i = 0 ; i < N ; i++)
      x_[i] = x[i];
    return x_;
  }

} // namespace codac

#endif