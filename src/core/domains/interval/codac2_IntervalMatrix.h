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
#include "codac2_IntervalVector.h"

namespace codac2
{
  class IntervalVector;

  class IntervalMatrix : public IntervalMatrixBase<IntervalMatrix,Matrix>
  {
    public:

      explicit IntervalMatrix(size_t r, size_t c);

      explicit IntervalMatrix(size_t r, size_t c, const Interval& x);

      IntervalMatrix(const Matrix& x);

      explicit IntervalMatrix(const Matrix& lb, const Matrix& ub);

      explicit IntervalMatrix(size_t r, size_t c, const double bounds[][2]);

      IntervalMatrix(std::initializer_list<std::initializer_list<Interval>> l);

      template<typename... IV>
        requires (std::is_same_v<IntervalVector,IV> && ...)
      IntervalMatrix(const IV&... x)
        : MatrixBase<IntervalMatrix,Interval>(sizeof...(IV), std::get<0>(std::tie(x...)).size()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(sizeof...(IV), std::get<0>(std::tie(x...)).size())
      {
        size_t i = 0;
        ((this->row(i++) = x), ...);
        assert(i == nb_rows());
      }

      IntervalMatrix(const MatrixBase<Matrix,double>& x);

      IntervalMatrix(const MatrixBase<IntervalMatrix,Interval>& x);

      IntervalMatrix(const IntervalVector& x);

      template<typename Q,typename T>
      IntervalMatrix(const MatrixBaseBlock<Q,T>& x)
        : IntervalMatrix(x.eval())
      { }

      template<typename OtherDerived>
      IntervalMatrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<IntervalMatrix,Interval>(x.template cast<Interval>()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(x.template cast<Interval>())
      { }

      IntervalMatrix transpose() const;

      IntervalMatrix diag_matrix() const;

      friend bool operator==(const IntervalMatrix& x1, const IntervalMatrix& x2);

      static IntervalMatrix empty(size_t r, size_t c);

      // Operators

      IntervalMatrix& operator+=(const IntervalMatrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e += x._e;
        return *this;
      }

      template<typename Q,typename T>
      IntervalMatrix& operator+=(const MatrixBaseBlock<Q,T>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e += eigen(x).template cast<Interval>();
        return *this;
      }

      IntervalMatrix& operator-=(const IntervalMatrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e -= x._e;
        return *this;
      }

      template<typename Q,typename T>
      IntervalMatrix& operator-=(const MatrixBaseBlock<Q,T>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e -= eigen(x).template cast<Interval>();
        return *this;
      }

      IntervalMatrix& operator*=(const Interval& x)
      {
        this->_e *= x;
        return *this;
      }

      IntervalMatrix& operator*=(const IntervalMatrix& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e *= x._e;
        return *this;
      }

      template<typename Q,typename T>
      IntervalMatrix& operator*=(const MatrixBaseBlock<Q,T>& x)
      {
        assert_release(this->nb_rows() == x.nb_rows() && this->nb_cols() == x.nb_cols());
        this->_e *= eigen(x).template cast<Interval>();
        return *this;
      }

      IntervalMatrix& operator/=(const Interval& x)
      {
        this->_e /= x;
        return *this;
      }
  };

  inline std::ostream& operator<<(std::ostream& os, const IntervalMatrix& x)
  {
    if(x.is_empty())
      return os << "( empty matrix )";

    else
      return os << (const MatrixBase<IntervalMatrix,Interval>&)x;
  }
}