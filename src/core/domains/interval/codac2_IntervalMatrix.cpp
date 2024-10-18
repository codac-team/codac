/** 
 *  codac2_IntervalMatrix.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_IntervalMatrix.h"
#include "codac2_IntervalVector.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  IntervalMatrix::IntervalMatrix(size_t r, size_t c)
    : IntervalMatrix(r,c,Interval())
  {
    assert_release(r >= 0 && c >= 0);
  }

  IntervalMatrix::IntervalMatrix(size_t r, size_t c, const Interval& x)
    : MatrixBase<IntervalMatrix,Interval>(r,c,x),
      IntervalMatrixBase<IntervalMatrix,Matrix>(r,c,x)
  {
    assert_release(r >= 0 && c >= 0);
  }

  IntervalMatrix::IntervalMatrix(const Matrix& x)
    : MatrixBase<IntervalMatrix,Interval>(x._e.template cast<Interval>()),
      IntervalMatrixBase<IntervalMatrix,Matrix>(x._e.template cast<Interval>())
  { }

  IntervalMatrix::IntervalMatrix(const Matrix& lb, const Matrix& ub)
    : MatrixBase<IntervalMatrix,Interval>(lb._e.template cast<Interval>()),
      IntervalMatrixBase<IntervalMatrix,Matrix>(lb._e.template cast<Interval>())
  {
    assert_release(lb.size() == ub.size());
    *this |= ub;
  }

  IntervalMatrix::IntervalMatrix(size_t r, size_t c, const double bounds[][2])
    : MatrixBase<IntervalMatrix,Interval>(r,c),
      IntervalMatrixBase<IntervalMatrix,Matrix>(r,c,bounds)
  {
    assert_release(r >= 0 && c >= 0);
  }

  IntervalMatrix::IntervalMatrix(std::initializer_list<std::initializer_list<Interval>> l)
    : MatrixBase<IntervalMatrix,Interval>(l),
      IntervalMatrixBase<IntervalMatrix,Matrix>(l)
  {
    assert_release(!std::empty(l));
  }

  IntervalMatrix::IntervalMatrix(const MatrixBase<Matrix,double>& x)
    : IntervalMatrix(x._e.template cast<Interval>())
  { }

  IntervalMatrix::IntervalMatrix(const MatrixBase<IntervalMatrix,Interval>& x)
    : MatrixBase<IntervalMatrix,Interval>(x._e),
      IntervalMatrixBase<IntervalMatrix,Matrix>(x._e)
  { }

  IntervalMatrix::IntervalMatrix(const IntervalVector& x)
    : IntervalMatrix(x._e)
  { }

  IntervalMatrix IntervalMatrix::transpose() const
  {
    return this->_e.transpose();
  }

  IntervalMatrix IntervalMatrix::diag_matrix() const
  {
    return _e.diagonal().asDiagonal().toDenseMatrix();
  }

  bool operator==(const IntervalMatrix& x1, const IntervalMatrix& x2)
  {
    // ^ This overload allows automatic cast for Matrix == IntervalMatrix comparisons
    return (IntervalMatrixBase<IntervalMatrix,Matrix>)x1 == (IntervalMatrixBase<IntervalMatrix,Matrix>)x2;
  }

  IntervalMatrix IntervalMatrix::empty(size_t r, size_t c)
  {
    assert_release(r >= 0 && c >= 0);
    return IntervalMatrix(r,c,Interval::empty());
  }
}