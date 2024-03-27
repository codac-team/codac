/** 
 *  \file
 *  IntervalMatrix class
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
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTERVALMATRIX_H__
#define __CODAC2_INTERVALMATRIX_H__

#include "codac2_Matrix.h"
#include "codac2_IntervalMatrixTemplate.h"

namespace codac2
{
  using Eigen::Dynamic;

  template<int R=Dynamic,int C=Dynamic>
  class IntervalMatrix_ : public IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>
  {
    public:

      IntervalMatrix_()
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>()
      { }
    
      IntervalMatrix_(int nb_rows, int nb_cols)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(nb_rows, nb_cols)
      { }
    
      IntervalMatrix_(int nb_rows, int nb_cols, const Interval& x)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(nb_rows, nb_cols, x)
      { }
      
      explicit IntervalMatrix_(int nb_rows, int nb_cols, const double bounds[][2])
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(nb_rows, nb_cols, bounds)
      { }
      
      explicit IntervalMatrix_(int nb_rows, int nb_cols, const Interval values[])
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(nb_rows, nb_cols, values)
      { }
      
      explicit IntervalMatrix_(const Interval values[])
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(values)
      { }

      IntervalMatrix_(std::initializer_list<std::initializer_list<Interval>> l)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(l)
      { }

      IntervalMatrix_(const Matrix_<R,C>& x)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(x)
      { }

      IntervalMatrix_(const Matrix_<R,C>& lb, const Matrix_<R,C>& ub)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(lb, ub)
      { }
      
      template<typename OtherDerived>
      IntervalMatrix_(const Eigen::MatrixBase<OtherDerived>& other)
        : IntervalMatrixTemplate_<IntervalMatrix_<R,C>,Matrix_<R,C>,R,C>(other)
      { }
  };

  class IntervalMatrix : public IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>
  {
    public:
    
      IntervalMatrix(int nb_rows, int nb_cols)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(nb_rows, nb_cols)
      { }
    
      IntervalMatrix(int nb_rows, int nb_cols, const Interval& x)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(nb_rows, nb_cols, x)
      { }
      
      explicit IntervalMatrix(int nb_rows, int nb_cols, const double bounds[][2])
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(nb_rows, nb_cols, bounds)
      { }
      
      explicit IntervalMatrix(int nb_rows, int nb_cols, const Interval values[])
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(nb_rows, nb_cols, values)
      { }

      IntervalMatrix(std::initializer_list<std::initializer_list<Interval>> l)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(l)
      { }

      IntervalMatrix(const Matrix& x)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(x)
      { }

      IntervalMatrix(const codac2::Matrix& lb, const codac2::Matrix& ub)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(lb, ub)
      { }

      explicit IntervalMatrix(const IntervalMatrix& x)
        : IntervalMatrix(x.rows(),x.cols())
      {
        *this = x;
      }
      
      template<typename OtherDerived>
      IntervalMatrix(const Eigen::MatrixBase<OtherDerived>& other)
        : IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>(other)
      { }

      template<typename OtherDerived>
      IntervalMatrix operator+(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator+(x));
      }

      template<typename OtherDerived>
      IntervalMatrix operator+=(const Eigen::MatrixBase<OtherDerived>& x)
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator+=(x));
      }

      IntervalMatrix operator-() const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator-());
      }

      template<typename OtherDerived>
      IntervalMatrix operator-(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator-(x));
      }

      template<typename OtherDerived>
      IntervalMatrix operator-=(const Eigen::MatrixBase<OtherDerived>& x)
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator-=(x));
      }

      template<typename OtherDerived>
      IntervalMatrix operator*(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator*(x));
      }

      template<typename OtherDerived>
      IntervalMatrix operator&(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator&(x));
      }
      
      template<typename OtherDerived>
      IntervalMatrix operator|(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return IntervalMatrix(IntervalMatrixTemplate_<IntervalMatrix,codac2::Matrix>::operator|(x));
      }
  };

  inline Matrix operator*(int a, const Matrix& x)
  {
    return a * (MatrixTemplate_<Matrix_<Dynamic,Dynamic>,double,Dynamic,Dynamic>)(x);
  }

  inline Matrix operator*(double a, const Matrix& x)
  {
    return a * (MatrixTemplate_<Matrix_<Dynamic,Dynamic>,double,Dynamic,Dynamic>)(x);
  }

  inline IntervalMatrix operator*(const Interval& a, const Matrix& x)
  {
    return a * x.template cast<Interval>();
  }

  inline IntervalMatrix operator|(const Matrix& x, const Matrix& y)
  {
    return IntervalMatrix(x).operator|(y);
  }

} // namespace codac

#endif