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

      template<typename... IV, typename = typename std::enable_if<(true && ... && 
          std::is_same_v<IntervalVector,IV>
        ), void>::type>
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

      template<typename OtherDerived>
      IntervalMatrix(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<IntervalMatrix,Interval>(x.template cast<Interval>()),
          IntervalMatrixBase<IntervalMatrix,Matrix>(x.template cast<Interval>())
      { }

      template<typename OtherDerived>
      IntervalMatrix(const MatrixBaseBlock<OtherDerived,Interval>& x)
        : IntervalMatrix(x.eval())
      { }

      //Interval& operator[](size_t i) = delete;
      //const Interval& operator[](size_t i) const = delete;

      IntervalMatrix transpose() const;

      IntervalMatrix diag_matrix() const;

      friend bool operator==(const IntervalMatrix& x1, const IntervalMatrix& x2);

      static IntervalMatrix empty(size_t r, size_t c);
  };

  // Some of the following functions are defined in codac2_Intervalvector.h file

  IntervalMatrix operator*(const Interval& x1, const Matrix& x2);

  IntervalVector operator*(const IntervalMatrix& x1, const Vector& x2);

  template<typename Q_>
  IntervalVector operator*(const MatrixBaseBlock<Q_,Interval>& x1, const Vector& x2);

  IntervalVector operator*(const Matrix& x1, const IntervalVector& x2);

  template<typename Q_>
  IntervalVector operator*(const Matrix& x1, const MatrixBaseBlock<Q_,Interval>& x2);

  template<typename Q1_,typename Q2_,typename T>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,Interval>& x1, const MatrixBaseBlock<Q2_,T>& x2)
  {
    return x1.eval() * x2.eval().template cast<Interval>();
  }

  template<typename Q1_,typename Q2_,typename T>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,T>& x1, const MatrixBaseBlock<Q2_,Interval>& x2)
  {
    return x1.eval().template cast<Interval>() * x2.eval();
  }

  template<typename Q1_,typename Q2_>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,Interval>& x1, const MatrixBaseBlock<Q2_,Interval>& x2)
  {
    return x1.eval() * x2.eval();
  }

  IntervalMatrix operator/(const Matrix& x1, const Interval& x2);

  inline std::ostream& operator<<(std::ostream& os, const IntervalMatrix& x)
  {
    if(x.is_empty())
      return os << "( empty matrix )";

    else
      return os << (const MatrixBase<IntervalMatrix,Interval>&)x;
  }
}