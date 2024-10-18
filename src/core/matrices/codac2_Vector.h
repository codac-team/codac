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

      explicit Vector(size_t n);

      explicit Vector(size_t n, double x);

      Vector(std::initializer_list<double> l);

      Vector(const std::vector<double>& l);

      Vector(const MatrixBase<Vector,double>& x);

      template<typename OtherDerived>
      Vector(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<Vector,double>(x),
          VectorBase<Vector,Matrix,double>(x)
      { }

      double& operator()(size_t i, size_t j) = delete;
      const double& operator()(size_t i, size_t j) const = delete;

      size_t min_coeff_index() const;

      size_t max_coeff_index() const;

      // Operators

      Vector& operator+=(const Vector& x)
      {
        assert_release(this->size() == x.size());
        this->_e += x._e;
        return *this;
      }

      Vector& operator-=(const Vector& x)
      {
        assert_release(this->size() == x.size());
        this->_e -= x._e;
        return *this;
      }

      Vector& operator*=(double x)
      {
        this->_e *= x;
        return *this;
      }

      Vector& operator/=(double x)
      {
        this->_e /= x;
        return *this;
      }

  };

  std::ostream& operator<<(std::ostream& os, const Vector& x);
}