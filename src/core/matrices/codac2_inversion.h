/** 
 *  \file codac2_Inversion.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  enum LeftOrRightInv { LEFT_INV, RIGHT_INV };

  /** \brief Compute an upper bound of A+A^2+A^3+..., with A a matrix of intervals
   *  as an "error term" (use only bounds on coefficients)
   *  
   *  The function also returns mrad, which gives an idea of the “magnification” of
   *  the matrix during calculation (in particular, if mrad = oo, then the inversion
   *  calculation (e.g., performed by Eigen) has somehow failed and some coefficients
   *  of the output interval matrix are [-oo,+oo]).
   *
   *  \pre A is a square matrix
   *
   *  \param A a matrix of intervals (supposed around 0)
   *  \param mrad the maximum radius of the result added (output argument)
   *  \return the enclosure. May include (-oo,oo) 
   */
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double &mrad);

  /** \brief Correct the approximate inverse of a matrix
   *
   *  \pre A and B are square matrices
   * 
   *  \tparam O if LEFT_INV, use the inverse of BA (otherwise use the inverse of AB,
   *   left inverse is normally better)
   *  \param A a matrix expression
   *  \param B a (almost punctual) approximation of its inverse,
   *  \return the enclosure
   */
  template<LeftOrRightInv O=LEFT_INV,typename OtherDerived,typename OtherDerived_>
  inline IntervalMatrix inverse_correction(const Eigen::MatrixBase<OtherDerived>& A, const Eigen::MatrixBase<OtherDerived_>& B)
  {
    assert_release(A.is_squared());
    assert_release(B.is_squared());

    auto A_ = A.template cast<Interval>();
    auto B_ = B.template cast<Interval>();

    Index N = A_.rows();
    assert_release(N==B_.rows());

    auto Id = IntervalMatrix::Identity(N,N);
    auto erMat = [&]() { if constexpr(O == LEFT_INV) return -B_*A_+Id; else return -A_*B_+Id; }();
    
    double mrad=0.0;
    IntervalMatrix E = infinite_sum_enclosure(erMat,mrad);
    IntervalMatrix Ep = Id+erMat*(Id+E); 
        /* one could use several iterations here, either
           using mrad, or directly */

    auto res = (O == LEFT_INV) ? IntervalMatrix(Ep*B_) : IntervalMatrix(B_*Ep);

    // We want the presence of non-invertible matrices to
    // result in coefficients of the form [-oo,+oo].
    if (mrad==oo) {
       for (Index c=0;c<N;c++) {
         for (Index r=0;r<N;r++) {
           if (Ep(r,c).is_unbounded()) {
              for (Index k=0;k<N;k++) {
                   if constexpr(O == LEFT_INV)
                       res(r,k) = Interval();
                   else 
                       res(k,c) = Interval();
              }
           }
         }
       }
    }
    return  res;
  }

  /** \brief Enclosure of the inverse of a (non-singular) matrix expression
   *
   *  \pre A is a square matrix
   *
   *  \param A a matrix expression
   *  \return the enclosure. Can have (-oo,oo) coefficients if A is singular
   *  or almost singular
   */
  template<typename OtherDerived>
  inline IntervalMatrix inverse_enclosure(const Eigen::MatrixBase<OtherDerived>& A)
  {
    assert_release(A.is_squared());
    Index N=A.rows();
    return inverse_correction<LEFT_INV>(A, 
      A.fullPivLu().solve(Matrix::Identity(N,N)));
  }


  /** \brief Enclosure of the inverse of a matrix of intervals
   *
   *  \pre A is a square matrix
   *
   *  \param A a matrix of intervals
   *  \return the enclosure. Can have (-oo,oo) coefficients if the 
   *  inversion "failed"
   */
  IntervalMatrix inverse_enclosure(const IntervalMatrix &A);
}
