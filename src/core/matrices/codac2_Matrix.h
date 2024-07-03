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

      Matrix(const MatrixBase<Matrix,double>& x);

      Matrix(std::initializer_list<std::initializer_list<double>> l);

      template<typename OtherDerived>
      Matrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<Matrix,double>(x)
      { }

      Matrix transpose() const;

      Matrix diagonal_matrix() const;

      Matrix inverse() const;
  };

  Vector operator*(const Matrix& x1, const Vector& x2);

  template<typename Q1_,typename Q2_>
  Matrix operator*(const MatrixBaseBlock<Q1_,double>& x1, const MatrixBaseBlock<Q2_,double>& x2)
  {
    return x1.eval() * x2.eval();
  }
}