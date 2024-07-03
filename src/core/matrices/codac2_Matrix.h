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
  class Matrix : public MatrixBase<Matrix,double>
  {
    public:

      explicit Matrix(size_t r, size_t c)
        : Matrix(r,c,0.)
      {
        assert_release(r > 0 && c > 0);
      }

      explicit Matrix(size_t r, size_t c, double x)
        : MatrixBase<Matrix,double>(r,c,x)
      {
        assert_release(r > 0 && c > 0);
      }

      explicit Matrix(size_t r, size_t c, const double values[])
        : MatrixBase<Matrix,double>(r,c,values)
      {
        assert_release(r > 0 && c > 0);
      }

      Matrix(std::initializer_list<std::initializer_list<double>> l)
        : MatrixBase<Matrix,double>(l)
      {
        assert_release(!std::empty(l));
      }

      template<typename OtherDerived>
      Matrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<Matrix,double>(x)
      { }

      Matrix transpose() const
      {
        return this->_e.transpose();
      }

      Matrix diagonal_matrix() const
      {
        return this->_e.diagonal().asDiagonal().toDenseMatrix();
      }

      Matrix inverse() const
      {
        return this->_e.inverse();
      }
  };
}