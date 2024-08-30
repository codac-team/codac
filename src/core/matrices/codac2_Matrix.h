/** 
 *  \file codac2_Matrix.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_assert.h"
#include "codac2_MatrixBase.h"

namespace codac2
{
  class Vector;
  class IntervalVector;
  class IntervalMatrix;

  class Matrix : public MatrixBase<Matrix,double>
  {
    public:

      explicit Matrix(size_t r, size_t c);

      explicit Matrix(size_t r, size_t c, double x);

      explicit Matrix(size_t r, size_t c, const double values[]);

      Matrix(std::initializer_list<std::initializer_list<double>> l);

      Matrix(const MatrixBase<Matrix,double>& x);

      Matrix(const Vector& x);

      template<typename Q>
      Matrix(const MatrixBaseBlock<Q,double>& x)
        : Matrix(x.eval())
      { }

      template<typename OtherDerived>
      Matrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<Matrix,double>(x)
      { }

      Matrix transpose() const;

      Matrix diag_matrix() const;

      Matrix inverse() const;

      // Operators

      Matrix& operator+=(const Matrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e += x._e;
        return *this;
      }

      template<typename Q>
      Matrix& operator+=(const MatrixBaseBlock<Q,double>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e += eigen(x);
        return *this;
      }

      Matrix& operator-=(const Matrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e -= x._e;
        return *this;
      }

      template<typename Q>
      Matrix& operator-=(const MatrixBaseBlock<Q,double>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e -= eigen(x);
        return *this;
      }

      Matrix& operator*=(double x)
      {
        this->_e *= x;
        return *this;
      }

      Matrix& operator*=(const Matrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e *= x._e;
        return *this;
      }

      template<typename Q>
      Matrix& operator*=(const MatrixBaseBlock<Q,double>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e *= eigen(x);
        return *this;
      }

      Matrix& operator/=(double x)
      {
        this->_e /= x;
        return *this;
      }
  };
}