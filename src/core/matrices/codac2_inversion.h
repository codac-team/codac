/** 
 *  \file codac2_inversion.h
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

  /**
   * \brief Correct the approximation of the inverse \f$\mathbf{B}\approx\mathbf{A}^{-1}\f$ of
   * a square matrix \f$\mathbf{A}\f$ by providing a reliable enclosure \f$[\mathbf{A}^{-1}]\f$.
   *
   * \pre \f$\mathbf{A}\f$ and \f$\mathbf{B}\f$ are square matrices, possibly interval matrices.
   * 
   * \tparam O If ``LEFT_INV``, use the inverse of \f$\mathbf{BA}\f$
   *           (otherwise use the inverse of \f$\mathbf{AB}\f$, left inverse is normally better).
   *           In Python/Matlab, this template parameter is provided as a last boolean argument,
   *           and is ``left_inv = True`` by default.
   * \param A A matrix expression, possibly interval.
   * \param B An (almost punctual) approximation of its inverse.
   * \return The enclosure of the inverse.
   */
  template<LeftOrRightInv O=LEFT_INV,typename OtherDerived,typename OtherDerived_>
  inline IntervalMatrix inverse_correction(const Eigen::MatrixBase<OtherDerived>& A, const Eigen::MatrixBase<OtherDerived_>& B)
  {
    assert_release(B.cols()==A.rows());
    assert_release(A.cols()==B.rows());

    auto A_ = A.template cast<Interval>();
    auto B_ = B.template cast<Interval>();

    Index N = (O == LEFT_INV ? A_.cols() : A_.rows());

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
              if constexpr(O == LEFT_INV)
                 res.row(r).setConstant(Interval());
              else
                 res.col(r).setConstant(Interval());
           }
         }
       }
    }
    return  res;
  }

  /**
   * \brief Enclosure of the inverse of a (non-singular) matrix expression,
   * possibly an interval matrix.
   *
   * \pre \f$\mathbf{A}\f$ is a square matrix.
   *
   * \param A A matrix expression, possibly interval.
   * \return The enclosure of the inverse. Can have \f$(-\infty,\infty)\f$ coefficients if
   * \f$\mathbf{A}\f$ is singular or almost singular, if the inversion "failed".
   */
  template<typename OtherDerived>
  inline IntervalMatrix inverse_enclosure(const Eigen::MatrixBase<OtherDerived>& A)
  {
    assert_release(A.is_squared());
    Index N = A.rows();

    if constexpr(std::is_same_v<typename OtherDerived::Scalar,Interval>)
      return inverse_correction<LEFT_INV>(A, 
        (A.mid()).fullPivLu().solve(Matrix::Identity(N,N)));

    else
      return inverse_correction<LEFT_INV>(A, 
        A.fullPivLu().solve(Matrix::Identity(N,N)));
  }


  /** 
   * \brief Enclosure of a left inverse of a (full-rank) matrix
   * expression, possibly an interval matrix.
   *
   * \pre \f$\mathbf{A}\f$ has more rows than columns.
   *
   * \param A A non-square matrix expression
   * \return The enclosure of a left-inverse. Can have 
   * \f$(-\infty,\infty)\f$ coefficients if the inversion "failed",
   * either because the matrix is (almost) not full-rank, or the full pivot
   * LU decomposition did not use the ''correct'' pivots 
   * (for Interval matrix) */
  template<typename OtherDerived>
  inline IntervalMatrix left_inverse_enclosure
		(const Eigen::MatrixBase<OtherDerived>& A)
  {
    Index N = A.rows();
    Index M = A.cols();
    assert_release(N>M);
    /* we use the transpose to solve, then we correct with LEFT_INV */
    if constexpr(std::is_same_v<typename OtherDerived::Scalar,Interval>) {
        return inverse_correction<LEFT_INV>(A,
			(A.mid().transpose()).fullPivLu()
                                .solve(Matrix::Identity(M,M)).transpose());
    } else {
        return inverse_correction<LEFT_INV>(A,
			(A.transpose()).fullPivLu()
                                .solve(Matrix::Identity(M,M)).transpose());
    }
  }

  /** 
   * \brief Enclosure of a right inverse of a (full-rank) matrix
   * expression, possibly an interval matrix.
   *
   * \pre \f$\mathbf{A}\f$ has more columns than rows.
   *
   * \param A A non-square matrix expression
   * \return The enclosure of a right-inverse. Can have 
   * \f$(-\infty,\infty)\f$ coefficients if the inversion "failed",
   * either because the matrix is (almost) not full-rank, or the full pivot
   * LU decomposition did not use the ''correct'' pivots 
   * (for Interval matrix) */
  template<typename OtherDerived>
  inline IntervalMatrix right_inverse_enclosure
		(const Eigen::MatrixBase<OtherDerived>& A)
  {
    Index N = A.rows();
    Index M = A.cols();
    assert_release(M>N);
    /* we solve directly, and transpose for the inverse_correction (?) */
    if constexpr(std::is_same_v<typename OtherDerived::Scalar,Interval>) {
        return inverse_correction<LEFT_INV> (A.transpose(),
	(A.mid()).fullPivLu().solve(Matrix::Identity(N,N)).transpose())
			.transpose();
    } else {
        return inverse_correction<LEFT_INV> (A.transpose(),
		A.fullPivLu().solve(Matrix::Identity(N,N)).transpose())
			.transpose();
    }
  }

  /**
   * \brief Compute an upper bound of \f$\left([\mathbf{A}]+[\mathbf{A}]^2+[\mathbf{A}]^3+\dots\right)\f$,
   * with \f$[\mathbf{A}]\f$ a matrix of intervals as an "error term" (uses only bounds on coefficients).
   *  
   * The function also returns ``mrad``, which gives an idea of the *magnification* of
   * the matrix during calculation. In particular, if ``mrad`` = \f$\infty\f$, then the inversion
   * calculation (*e.g.*, performed by Eigen) has somehow failed and some coefficients
   * of the output interval matrix are \f$[-\infty,\infty]\f$.
   *
   * \pre \f$[\mathbf{A}]\f$ is a square matrix.
   *
   * \param A A matrix of intervals (supposed around \f$\mathbf{0}\f$).
   * \param mrad The maximum radius of the result added (output argument).
   * \return The sum enclosure. May be unbounded.
   */
  IntervalMatrix infinite_sum_enclosure(const IntervalMatrix& A, double& mrad);
}
