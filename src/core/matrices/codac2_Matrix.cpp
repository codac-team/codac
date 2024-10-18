/** 
 *  codac2_Matrix.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  Matrix::Matrix(size_t r, size_t c)
    : Matrix(r,c,0.)
  {
    assert_release(r >= 0 && c >= 0);
  }

  Matrix::Matrix(size_t r, size_t c, double x)
    : MatrixBase<Matrix,double>(r,c,x)
  {
    assert_release(r >= 0 && c >= 0);
  }

  Matrix::Matrix(size_t r, size_t c, const double values[])
    : MatrixBase<Matrix,double>(r,c,values)
  {
    assert_release(r >= 0 && c >= 0);
  }

  Matrix::Matrix(std::initializer_list<std::initializer_list<double>> l)
    : MatrixBase<Matrix,double>(l)
  {
    assert_release(!std::empty(l));
  }

  Matrix::Matrix(const MatrixBase<Matrix,double>& x)
    : Matrix(x._e)
  { }

  Matrix::Matrix(const Vector& x)
    : Matrix(x._e)
  { }


  Matrix Matrix::transpose() const
  {
    return this->_e.transpose();
  }

  Matrix Matrix::diag_matrix() const
  {
    return this->_e.diagonal().asDiagonal().toDenseMatrix();
  }

  Matrix Matrix::inverse() const
  {
    return this->_e.inverse();
  }
}