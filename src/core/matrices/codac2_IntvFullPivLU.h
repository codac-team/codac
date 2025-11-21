/** 
 * \file codac2_IntvFullPivLU.h
 * ----------------------------------------------------------------------------
 * \date       2024
 * \author     Damien Massé
 * \copyright  Copyright 2024 Codac Team
 * \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_Matrix.h"
#include "codac2_Row.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  /** 
   * \class IntvFullPivLU
   *
   * \brief Full pivot LU decomposition for a matrix of intervals, 
   * based on Eigen decomposition. The decomposition is of the form
   * \f$\mathbf{M} = \mathbf{P}^{-1} [\mathbf{L}][\mathbf{U}] \mathbf{Q}^{-1}\f$
   * where \f$\mathbf{P}\f$ and \f$\mathbf{Q}\f$ are permutation matrices, and
   * \f$[\mathbf{L}]\f$ and \f$[\mathbf{U}]\f$ are lower and upper interval matrices
   * (*i.e.* \f$[\mathbf{L}]\f$'s diagonal is 1).
   */
  class IntvFullPivLU
  {
     public:

        /**
         * \brief Constructor from ``Matrix`` of double 
         * 
         * \param M the matrix for which the decomposition is computed
         */
        explicit IntvFullPivLU(const Matrix& M);

        /**
         * \brief Constructor from ``Matrix`` of intervals. Eigen 
         * decomposition is done on ``M.mid()``.
         * 
         * \param M the matrix of intervals.
         */
        explicit IntvFullPivLU(const IntervalMatrix& M);

        /**
         * \brief Check if the matrix is injective,
         * *i.e.* its rank is equal to its number of rows.
         * 
         * \return a ``BoolInterval``
         */
        BoolInterval is_injective() const;

        /**
         * \brief Check if the initial matrix is invertible 
         * *i.e.* it is square and full rank
         * 
         * \return a ``BoolInterval``
         */
        BoolInterval is_invertible() const;

        /**
         * \brief Check if the matrix is surjective
         * *i.e.* its rank is equal to its number of cols.
         * 
         * \return a ``BoolInterval``
         */
        BoolInterval is_surjective() const;

        /**
         * \brief Return an interval enclosing the determinant
         * 
         * \pre The matrix must be square.
         * 
         * \return the product of the diagonal elements of \f$[\mathbf{U}]\f$
         */
        Interval determinant() const;

        /**
         * \brief Return an interval enclosing the rank. Quite precise
         * for square matrix (number of diagonal elements of \f$[\mathbf{U}]\f$ not
         * containing 0). Less precise for non-square matrices, where each row/column
         * outside the top-left part of \f$[\mathbf{U}]\f$ can change the rank.
         * However, if no diagonal element contains 0, the return is 
         * unambiguous.
         *
         * \return an interval enclosing the possible ranks.
         */
        Interval rank() const;

        /**
         * \brief Approximation of the size of the kernel space,
         * based on the result of ``rank()`` (number of cols-rank()).
         * As such, this is not the exact size of the kernel space
         * as built by ``kernel()``.
         * 
         * \return an interval enclosing the possible dimensions.
         */
        Interval dimension_of_kernel() const;

        /**
         * \brief Overapproximation of the kernel space as 
         * a matrix of column vectors.
         * Any vector \f$\mathbf{V}\f$ which is not a linear combination of the column
         * vectors is guaranteed to be outside the kernel
         * (*i.e.*, \f$\mathbf{M}\mathbf{V} \neq 0\f$).
         *
         * \return a matrix of column vectors, may be empty
         */
        IntervalMatrix kernel() const;

        /**
         * \brief Overapproximation of the left-null ("cokernel") space as 
         * a matrix of row vectors.
         * Any vector \f$\mathbf{V}\f$ which is not a linear combination of the row
         * vectors is guaranteed to be outside the kernel
         * (*i.e.*, \f$\mathbf{V}\mathbf{M} \neq 0\f$).
         *
         * \return a matrix of row vectors, may be empty
         */
        IntervalMatrix cokernel() const;

        /**
         * \brief "Underapproximation" of the column space of the matrix,
         * *i.e.* return a set of independant columns of the original matrix
         * which is possibly maximal. As for Eigen, you must 
         * provide the original matrix used for the decomposition.
         *
         * \pre \f$\mathbf{M}\f$ is the matrix used to build the decomposition
         *
         * \param M the matrix used to build the decomposition.
         * \return a matrix of columns of \f$\mathbf{M}\f$, or one vector of 0
         * if the rank of \f$\mathbf{M}\f$ may be 0.
         */
        template<typename Derived>
        Derived image(const Eigen::MatrixBase<Derived> &M) const;

        /**
         * \brief "Underapproximation" of the row space of the matrix,
         * *i.e.* return a set of independant rows of the original matrix
         * which is possibly maximal. As for Eigen, you must 
         * provide the original matrix used for the decomposition.
         *
         * \pre M is the matrix used to build the decomposition
         *
         * \param M the matrix used to build the decomposition.
         * \return a matrix of rows of \f$\mathbf{M}\f$, or one row of 0
         * if the rank of \f$\mathbf{M}\f$ may be 0.
         */
        template<typename Derived>
        Derived coimage(const Eigen::MatrixBase<Derived> &M) const;

        /**
         * \brief Equation solving \f$\mathbf{M}\mathbf{X}=\mathbf{rhs}\f$
         *
         * Precisely look for solutions where the only non-zero
         * values are those on non-zero pivots.
         * If the matrix is full-rank and surjective (cols >= rows),
         * it gives an overapproximation of the solutions (for each 
         * possible values of rhs).
         * 
         * If the matrix is full-rank and injective (rows >= cols),
         * it returns empty if no solution is possible, and a
         * possible overapproximation of the solutions otherwise
         * (but there *may* still be no solution).
         * 
         * If the matrix is not full-rank, 
         *   - empty means that no solution is possible with the
         *       initial precondition (non-zero values for non-zero pivots)
         *       if the goal is to prove the absence of solution, see
         *       ``solve`` with a bounding box.
         *   - non empty presents the possible solutions found.
         * 
         * \param rhs right-hand side of the equation 
         * \return a potential solution of the equation \f$\mathbf{M}\mathbf{X}=\mathbf{rhs}\f$
         */
        IntervalMatrix solve(const IntervalMatrix& rhs) const;

        /**
         * \brief Equation solving \f$\mathbf{M}\mathbf{X}=\mathbf{rhs}\f$
         * with bounding matrix \f$[\mathbf{B}]\f$ for
         * the solution, *i.e.* contraction of the matrix on the solutions
         * on \f$\mathbf{M}\mathbf{X}\f$.
         *
         * Especially useful where the matrix is not
         * full-rank, where solve without bounding box may return empty
         * even if solutions exist.
         *
         * If the matrix is full-rank and surjective (cols >= rows),
         * it gives an overapproximation of the solutions (for each 
         * possible values of rhs).
         * 
         * If the matrix is full-rank and injective (rows >= cols),
         * it returns empty if no solution is possible, and a
         * possible overapproximation of the solutions otherwise
         * (but there *may* still be no solution).
         * 
         * If the matrix is not full-rank, 
         *   - empty means that no solution is possible inside the bounding matrix.
         *   - non empty presents the possible solutions found.
         * 
         * \param rhs right-hand side of the equation 
         * \param B bounding-box of the left hand-side, contracted so that
         * \f$\mathbf{M}\mathbf{B}=\mathbf{rhs}\f$
         */
        void solve(const IntervalMatrix& rhs, IntervalMatrix& B) const;

        /**
         * \brief Rebuilding of the matrix, *i.e.* compute
         * \f$\mathbf{P}^{-1}[\mathbf{L}][\mathbf{U}]\mathbf{Q}^{-1}\f$.
         *
         * Can be used to evaluate the precision of the decomposition.
         *
         * \return the reconstructed matrix
         */
        IntervalMatrix reconstructed_matrix() const;

        /**
         * \brief Maximum magnitude of the diagonal elements of \f$[\mathbf{U}]\f$
         *
         * \return the maximum
         */
        double max_pivot() const;
        
        /**
         * \brief The permutation \f$\mathbf{P}\f$ in the decomposition
         * \f$\mathbf{P}^{-1}\mathbf{L}\mathbf{U}\mathbf{Q}^{-1}\f$.
         *
         * \return the permutation \f$\mathbf{P}\f$, as defined by Eigen
         */
        const Eigen::FullPivLU<Matrix>::PermutationPType& permutation_P() const;

        /**
         * \brief The permutation \f$\mathbf{Q}\f$ in the decomposition
         * \f$\mathbf{P}^{-1}\mathbf{L}\mathbf{U}\mathbf{Q}^{-1}\f$.
         *
         * \return the permutation \f$\mathbf{Q}\f$, as defined by Eigen
         */
        const Eigen::FullPivLU<Matrix>::PermutationQType& permutation_Q() const;

        /**
         * \brief The Eigen decomposition of ``M.mid()``
         *
         * \return the Eigen decomposition
         */
        const Eigen::FullPivLU<Matrix>& eigen_LU() const;

        /**
         * \brief Return the column-wise transformation done on ``M.mid()``
         * before the Eigen LU decomposition, as a Row.
         *
         * \return the transformation
         */ 
        const Row& transformation() const;

        /**
         * \brief Returns the matrix storing \f$[\mathbf{L}]\f$ and \f$[\mathbf{U}]\f$
         * (*i.e.* \f$[\mathbf{L}]\f$ for strictly lower part, \f$[\mathbf{U}]\f$ for upper part).
         *
         * \return the interval matrix
         */
        const IntervalMatrix& matrix_LU() const;


    private:

        Eigen::FullPivLU<Matrix> _LU;
        Row transform; ///< column-wise transformation done on ``M.mid()`` to homogeneise the diameters
        IntervalMatrix matrixLU_; ///< LU matrix
     
        void compute_matrix_LU(const IntervalMatrix& M, double nonzero);
        static IntervalMatrix build_LU_bounds(const IntervalMatrix& E);
  };

inline const IntervalMatrix& IntvFullPivLU::matrix_LU() const {
     return this->matrixLU_;
}
inline const Eigen::FullPivLU<Matrix>::PermutationPType 
      &IntvFullPivLU::permutation_P() const {
    return this->_LU.permutationP();
}
inline const Eigen::FullPivLU<Matrix>::PermutationQType 
      &IntvFullPivLU::permutation_Q() const {
    return this->_LU.permutationQ();
}
inline const Eigen::FullPivLU<Matrix> &IntvFullPivLU::eigen_LU() const {
    return this->_LU;
}
inline const Row &IntvFullPivLU::transformation() const {
     return this->transform;
}

template<typename Derived>
inline  Derived IntvFullPivLU::image
  (const Eigen::MatrixBase<Derived> &M) const
{
   int rk = this->rank().lb();
   if (rk==0) {
      return Derived::Zero(M.rows(),1); 
    /* NdDamien : où est-ce qu'on dit que Derived est une
       matrice ??? je crois que je hais le C++ ;)  */
   }
   Derived ret = 
    Derived::Zero(M.rows(),rk);
   Index p = 0;
   Index dim = std::min(matrixLU_.rows(),matrixLU_.cols());
   auto Q = this->_LU.permutationQ();
   for (Index i = 0; i<dim; i++) {
      if (!matrixLU_(i,i).contains(0.0)) {
         ret.col(p) = M.col(Q.indices().coeff(i));
         p++;
      }
   }
   return ret;
}

template<typename Derived>
inline  Derived IntvFullPivLU::coimage
  (const Eigen::MatrixBase<Derived> &M) const
{
   int rk = this->rank().lb();
   if (rk==0) {
      return Derived::Zero(1,M.cols()); 
   }
   Derived ret = 
    Derived::Zero(rk,M.cols());
   Index p = 0;
   Index dim = std::min(matrixLU_.rows(),matrixLU_.cols());
   auto P = this->_LU.permutationP();
   for (Index i = 0; i<dim; i++) {
      if (!matrixLU_(i,i).contains(0.0)) {
         ret.row(p) = M.row(P.indices().coeff(i));
         p++;
      }
   }
   return ret;
}

}