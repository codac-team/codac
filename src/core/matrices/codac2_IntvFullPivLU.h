/** 
 *  \file codac2_IntvFullPivLU.h
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
#include "codac2_BoolInterval.h"

namespace codac2
{
  /** \brief Full pivot LU decomposition for matrix of interval, 
   *  based on Eigen decomposition. The decomposition is of the form
   *  M = P{-1} [L][U] Q{-1} where P and Q are permutation matrices, and
   *  [L] and [U] are lower and upper interval matrices ([L] diagonal is 1).
   */
  class IntvFullPivLU
  {
     public:
     /** \brief constructor from Matrix of double 
      *  \param M the matrix for which the decomposition is computed
      */
     explicit IntvFullPivLU(const Matrix &M);

     /** \brief constructor from Matrix of intervals. Eigen 
      *  decomposition is done on M.mid().
      *
      *  \param M the matrix of intervals.
      */
     explicit IntvFullPivLU(const IntervalMatrix &M);

     /** \brief check if the matrix is injective,
      *  i.e. its rank is equal to its number of rows.
      *  \return TRUE, FALSE or UNKNOWN 
      */
     BoolInterval is_injective() const;
     /** \brief check if the initial matrix is invertible 
      *  i.e. it is square and full rank
      * 
      *  \return TRUE, FALSE or UNKNOWN */
     BoolInterval is_invertible() const;
     /** \brief check if the matrix is surjective
      *  i.e. its rank is equal to its number of cols.
      *  \return TRUE, FALSE or UNKNOWN 
      */
     BoolInterval is_surjective() const;
     /** \brief return an interval enclosing the determinant
      *  
      *  \pre the matrix is square
      * 
      *  \return the product of the diagonal elements of [U]
      */
     Interval determinant() const;
     /** \brief return a interval enclosing the rank. Quite precise
      *  for square matrix (number of diagonal elements of [U] not
      *  containing 0). Less for non-square matrices, where each row/column
      *  outside the top-left part of [U] can change the rank.
      *  However, if no diagonal element contains 0, the return is 
      *  unambiguous.
      *
      *  \return an interval enclosing the possible ranks.
      */
     Interval rank() const;
     /** \brief approximation of the size of the kernel space.
      *  based on the result of rank() (number of cols-rank()).
      *  As such, this is not
      *  the exact size of the kernel space as build by kernel() 
      * 
      *  \return an interval enclosing the possible dimensions.
      */
     Interval dimension_of_kernel() const;
     /** \brief overapproximation of the kernel space as 
      *  a matrix of column vectors.
      *  any vector V which is not a linear combination of the column
      *  vectors is guaranteed to be outside the kernel ( \f$MV \neq 0\f$ )
      *
      *  \return a matrix of column vectors, may be empty
      */
     IntervalMatrix kernel() const;
     /** \brief ``underapproximation'' of the column space of the matrix,
      *  ie return a set of independant columns of the original matrix
      *  which is possibly maximal. As for Eigen, you must 
      *  provide the original matrix used for the decomposition.
      *
      *  \pre M is the matrix used to build the decomposition
      *
      *  \param M the matrix used to build the decomposition.
      *  \return a matrix of columns of M, or one vector of 0
      *  if the rank of M may be 0.
      */
     template <typename Derived> Derived
	image(const Eigen::MatrixBase<Derived> &M) const;
     /** \brief equation solving M X = rhs 
      *  precisely look for solutions where the only non-zero
      *  values are those on non-zero pivots 
      *  if the matrix is full-rank and surjective (cols >= rows),
      *  it gives an overapproximation of the solutions (for each 
      *  possible values of rhs)
      *  if the matrix is full-rank and injective (rows >= cols),
      *  it returns empty if no solution is possible, and a
      *  possible overapproximation of the solutions otherwise
      *  (but there _may_ still be no solution)
      *  if the matrix is not full-rank, 
      *     empty means that no solution is possible with the
      *        initial precondition (non-zero values for non-zero pivots)
      *     non empty presents the possible solutions found 
      *  
      *  \param rhs right-hand side of the equation 
      *  \return a potential solution of the equation M X = rhs
      */
     IntervalMatrix solve(const IntervalMatrix &rhs) const;


     /** \brief rebuilding of the matrix, ie compute P^{-1}[L][U]Q^{-1}
      *  can be used to evaluate the precision of the decomposition
      *
      *  \return the reconstructed matrix
      */
     IntervalMatrix reconstructed_matrix() const;
     /** \brief maximum magnitude of the diagonal elements of [U]
      *
      *  \return the maximum
      */
     double max_pivot() const;
     
     /** \brief the permutation P in the decomposition P{-1}LUQ{-1}
      *
      * \return the permutation P, as defined by Eigen
      */
     const Eigen::FullPivLU<Matrix>::PermutationPType 
			&permutation_P () const;
     /** \brief the permutation Q in the decomposition P{-1}LUQ{-1}
      *
      * \return the permutation Q, as defined by Eigen
      */
     const Eigen::FullPivLU<Matrix>::PermutationQType 
			&permutation_Q () const;
     /** \brief the Eigen decomposition of M.mid()
      *
      * \return the Eigen decomposition
      */
     const Eigen::FullPivLU<Matrix> &eigen_LU() const;

     /** \brief returns the matrix storing [L] and [U]
      *  ([L] for strictly lower part, [U] for upper part)
      *
      * \return the interval matrix
      */
     const IntervalMatrix &matrix_LU() const;


     private:
        Eigen::FullPivLU<Matrix> _LU;

        IntervalMatrix matrixLU_;   /* LU matrix */
     
        void compute_matrix_LU(const IntervalMatrix &M, double nonzero);
        static IntervalMatrix build_LU_bounds(const IntervalMatrix &E);
  };

inline const IntervalMatrix &IntvFullPivLU::matrix_LU() const {
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


}


