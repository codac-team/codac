/** 
 *  \file codac2_Vector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_assert.h"
#include "codac2_VectorBase.h"
#include "codac2_Matrix.h"

namespace codac2
{
  class Matrix;

  class Vector : public VectorBase<Vector,Matrix,double>
  {
    public:

      explicit Vector(size_t n)
        : Vector(n,0.)
      {
        assert_release(n > 0);
      }

      explicit Vector(size_t n, double x)
        : MatrixBase<Vector,double>(n,1,x),
          VectorBase<Vector,Matrix,double>(n)
      {
        assert_release(n > 0);
      }

      Vector(std::initializer_list<double> l)
        : MatrixBase<Vector,double>(l.size(),1),
          VectorBase<Vector,Matrix,double>(l)
      {
        assert_release(!std::empty(l));
      }

      template<typename OtherDerived>
      Vector(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<Vector,double>(x),
          VectorBase<Vector,Matrix,double>(x)
      { }

      size_t min_coeff_index() const
      {
        size_t r,c;
        this->_e.minCoeff(&r,&c);
        assert(c == 0);
        return r;
      }

      size_t max_coeff_index() const
      {
        size_t r,c;
        this->_e.maxCoeff(&r,&c);
        assert(c == 0);
        return r;
      }

      static Vector zeros(size_t n)
      {
        assert_release(n > 0);
        return EigenMatrix<double>::Zero(n,1);
      }

      static Vector ones(size_t n)
      {
        assert_release(n > 0);
        return EigenMatrix<double>::Ones(n,1);
      }

      friend std::ostream& operator<<(std::ostream& os, const Vector& x);
  };

  inline std::ostream& operator<<(std::ostream& os, const Vector& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

  inline Vector operator*(const Matrix& x1, const Vector& x2)
  {
    return x1._e * x2._e;
  }

}