/** 
 *  \file codac2_IntervalMatrix.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrix. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrix (IBEX lib, author: Gilles Chabert)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_assert.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrixBase.h"

namespace codac2
{
  class IntervalVector;

  class IntervalMatrix : public IntervalMatrixBase<IntervalMatrix,Matrix>
  {
    public:

      explicit IntervalMatrix(size_t r, size_t c)
        : IntervalMatrix(r,c,Interval())
      {
        assert_release(r > 0 && c > 0);
      }

      explicit IntervalMatrix(size_t r, size_t c, const Interval& x)
        : MatrixBase<IntervalMatrix,Interval>(r,c,x),
          IntervalMatrixBase<IntervalMatrix,Matrix>(r,c,x)
      {
        assert_release(r > 0 && c > 0);
      }

      IntervalMatrix(const Matrix& x)
        : MatrixBase<IntervalMatrix,Interval>(x._e.template cast<Interval>()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(x._e.template cast<Interval>())
      { }

      explicit IntervalMatrix(const Matrix& lb, const Matrix& ub)
        : MatrixBase<IntervalMatrix,Interval>(lb._e.template cast<Interval>()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(lb._e.template cast<Interval>())
      {
        assert_release(lb.size() == ub.size());
        *this |= ub;
      }

      explicit IntervalMatrix(size_t r, size_t c, const double bounds[][2])
        : MatrixBase<IntervalMatrix,Interval>(r,c),
          IntervalMatrixBase<IntervalMatrix,Matrix>(r,c,bounds)
      {
        assert_release(r > 0 && c > 0);
      }

      IntervalMatrix(std::initializer_list<std::initializer_list<Interval>> l)
        : MatrixBase<IntervalMatrix,Interval>(l),
          IntervalMatrixBase<IntervalMatrix,Matrix>(l)
      {
        assert_release(!std::empty(l));
      }

      template<typename... IV, typename = typename std::enable_if<(true && ... && 
          std::is_same_v<IntervalVector,IV>
        ), void>::type>
      explicit IntervalMatrix(const IV&... x)
        : MatrixBase<IntervalMatrix,Interval>(std::get<0>(std::tie(x...)).size(), sizeof...(IV)),
          IntervalMatrixBase<IntervalMatrix,Matrix>(std::get<0>(std::tie(x...)).size(), sizeof...(IV))
      {
        size_t i = 0;
        ((this->col(i++) = x), ...);
        assert(i == nb_cols());
      }

      IntervalMatrix(const MatrixBase<IntervalMatrix,Interval>& x)
        : MatrixBase<IntervalMatrix,Interval>(x._e),
          IntervalMatrixBase<IntervalMatrix,Matrix>(x._e)
      { }

      template<typename OtherDerived>
      IntervalMatrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<IntervalMatrix,Interval>(x.template cast<Interval>()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(x.template cast<Interval>())
      { }

      template<typename OtherDerived>
      IntervalMatrix(const MatrixBaseBlock<OtherDerived,IntervalMatrix,Interval>& x)
        : IntervalMatrix(x.eval())
      { }

      IntervalMatrix transpose() const
      {
        return this->_e.transpose();
      }

      IntervalMatrix diagonal_matrix() const
      {
        return _e.diagonal().asDiagonal().toDenseMatrix();
      }

      static IntervalMatrix zeros(size_t r, size_t c)
      {
        assert_release(r > 0 && c > 0);
        return EigenMatrix<Interval>::Zero(r,c);
      }

      static IntervalMatrix ones(size_t r, size_t c)
      {
        assert_release(r > 0 && c > 0);
        return EigenMatrix<Interval>::Ones(r,c);
      }

      static IntervalMatrix eye(size_t r, size_t c)
      {
        assert_release(r > 0 && c > 0);
        return EigenMatrix<Interval>::Identity(r,c);
      }

      static IntervalMatrix empty(size_t r, size_t c)
      {
        assert_release(r > 0 && c > 0);
        return IntervalMatrix(r,c,Interval::empty());
      }
  };

  inline std::ostream& operator<<(std::ostream& os, const IntervalMatrix& x)
  {
    if(x.is_empty())
      return os << "( empty matrix )";

    else
      return os << (const MatrixBase<IntervalMatrix,Interval>&)x;
  }

  inline IntervalMatrix operator*(const codac2::Interval& a, const IntervalMatrix& x)
  {
    return a * x._e;
  }
}