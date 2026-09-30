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
#include <iostream>
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_IntervalVector.h"

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


  /**
   * \brief compute the interval cofactor matrix, as well as the 
   * determinant, of a square matrix 
   * \param A A matrix expression, possibly interval.
   * \return The enclosure of the cofactor matrix (first) 
   * and determinant (second)
   */
  template<typename OtherDerived>
  inline std::pair<IntervalMatrix,Interval>
	cofactor_matrix_enclosure(const Eigen::MatrixBase<OtherDerived>& A) {
      assert_release(A.is_squared());

      Index N = A.cols();
      auto A_ = A.template cast<Interval>();

      Eigen::FullPivLU<Matrix> lu(A_.mid());
        /* FullPiv is used to get the ``best'' pivoting strategy */
      auto P = lu.permutationP();
      auto Q = lu.permutationQ();

      IntervalMatrix pMq = P*A_*Q;
      /* preconditioning ? 
         we use L and U to compute
         L-1 pMq U-1
         and include L-1 and U-1 in the computation of the cofactor matrix
         */
      Matrix mLU = lu.matrixLU();
      for (int i=0;i<N;i++) {
          if (std::fabs(mLU(i,i))<1e-5) { /* FIXME : value of "nonzero" ? */
             mLU(i,i)=(mLU(i,i)<0.0 ? -1e-5 : 1e-5); 
          }
      }
      IntervalMatrix ImLU 
                = IntervalMatrix::Zero(N,N);
      for (int c=0;c<N;c++) {
        for (int r=c+1;r<N;r++) {
           Interval s(mLU(r,c));
           for (int k=c+1;k<r;k++) {
              s += mLU(r,k)*ImLU(k,c);
           }
           ImLU(r,c)=-s;
        }
      }
      for (int c=0;c<N;c++) {
        int r;
        ImLU(c,c)=Interval(1.0)/mLU(c,c);
        r=c-1;
        for (;r>=0;r--) {
           Interval s(mLU(r,c));
           s/=mLU(c,c);
           for (int k=r+1;k<c;k++) {
               s += mLU(r,k)*ImLU(k,c);
           }
           ImLU(r,c)=-s/mLU(r,r);
        }
      }
      IntervalMatrix InvL = 
                IntervalMatrix::Identity(N,N);
      InvL.triangularView<Eigen::StrictlyLower>() = ImLU;
      IntervalMatrix InvU = 
                IntervalMatrix::Identity(N,N);
      InvU.triangularView<Eigen::Upper>() = ImLU;
      pMq = InvL * pMq * InvU;



      IntervalMatrix Linv = IntervalMatrix::Identity(N,N);
      IntervalMatrix Uinv = IntervalMatrix::Identity(N,N);
      /* partial inversion of pMq is done, i.e. 
        we build a such that Linv * pMq * Uinv is equal
         ( D 0  )   ( Id     0    )
         ( 0 Id ) * ( 0  <eps ). The comatrix of the second part is :
        is      ( d 0 )
                ( 0 w ) with d = (max nrm)^a   (a = size of eps block)
                             w = (max nrm)^(a-1) */
      Index c=0;
      while (c<N-1) {
         Interval pivot = pMq(c,c);
         if (pivot.contains(0)) break; /* or if the mig is too low */
         Linv.block(c+1,0,N-1-c,c+1) -= 
         pMq.block(c+1,c,N-1-c,1)*Linv.block(c,0,1,c+1)/pivot;
         Uinv.block(0,c+1,c+1,N-1-c) -= 
             Uinv.block(0,c,c+1,1)*pMq.block(c,c+1,1,N-1-c)/pivot;
         pMq.bottomRightCorner(N-1-c,N-1-c) -=
            pMq.block(c+1,c,N-1-c,1)*pMq.block(c,c+1,1,N-1-c)/pivot;
         pMq.block(c+1,c,N-1-c,1).fill(0.0);
         pMq.block(c,c+1,1,N-1-c).fill(0.0);
         c++;
      }
      Index lastcol=c;
      Interval det=1.0;
      /* build the comatrix for the diagonal part of pMq 
         (note: built in place) */
      if (lastcol==N-1) lastcol=N; /*  the inversion succeeded completely, 
                    pMq is diagonal  (but the last coef may contains 0) */
      Index a = N-lastcol;
      IntervalVector tmp = IntervalVector::Zero(lastcol);
      tmp[0]=1.0;
      for (Index c=0;c<lastcol-1;c++) {
         tmp[c+1] = tmp[c]*pMq(c,c);
      }
      det = tmp[lastcol-1]*pMq(lastcol-1,lastcol-1);
      for (Index c=lastcol-1;c>=1;c--) {
         std::swap(tmp[c],pMq(c,c));
         pMq(c,c)*=tmp[0];
         tmp[0]*=tmp[c];
      }
      pMq(0,0)=tmp[0];
      /* special case: a=2 */
      if (a==2) {
         Interval det2 = pMq(N-1,N-1)*pMq(N-2,N-2)-pMq(N-1,N-2)*pMq(N-2,N-1);
         for (Index l=0;l<lastcol;l++) { 
           pMq(l,l)*=det2;
         }
         pMq.bottomRightCorner(2,2) *= det;
         std::swap(pMq(N-1,N-1),pMq(N-2,N-2));
         std::swap(pMq(N-1,N-2),pMq(N-2,N-1));
         pMq(N-1,N-2)=-pMq(N-1,N-2);
         pMq(N-2,N-1)=-pMq(N-2,N-1);
         det *= det2;
      } else if (a>2) {
         /* build the cofactor matrix */
          Interval eps=0.0;
          for (Index c=lastcol;c<N;c++) {
             Interval nrm = pMq.block(lastcol,c,a,1).norm();
             if (nrm.ub()>eps.ub()) eps=nrm;
          }
          Interval v1 = pow(eps,(int)a)*Interval(-1.0,1.0);
          Interval d1 = v1*eps;
          for (Index l=0;l<lastcol;l++) { 
             pMq(l,l)*=d1;
          }
          pMq.bottomRightCorner(a,a).fill(v1*det);
          det *= d1;
      }
      pMq = InvL.transpose()*Linv.transpose()*pMq*Uinv.transpose()*InvU.transpose();
      return {
        P.determinant()*Q.determinant()*
 	((IntervalMatrix)(P.transpose())*pMq*(IntervalMatrix(Q.transpose()))),
        P.determinant()*Q.determinant()*det
      };
  }

}
