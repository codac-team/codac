/** 
 *  \file codac2_MatrixBase_addons_Vector.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

/**
 * \brief Returns the index of the minimum coefficient in the vector.
 *
 * \return The index (0-based) of the minimum coefficient.
 *
 * \note For a column vector, the returned index is the row index.
 *       For a row vector, it is the column index.
 */
template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
inline Index min_coeff_index() const
{
  Index r,c;
  this->minCoeff(&r,&c);
  assert((C == 1 && c == 0) || (R == 1 && r == 0));
  return r;
}

/**
 * \brief Returns the index of the maximum coefficient in the vector.
 *
 * \return The index (0-based) of the maximum coefficient.
 *
 * \note For a column vector, the returned index is the row index.
 *       For a row vector, it is the column index.
 */
template<typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<U>) && (IsVectorOrRow<R,C>)
inline Index max_coeff_index() const
{
  Index r,c;
  this->maxCoeff(&r,&c);
  assert((C == 1 && c == 0) || (R == 1 && r == 0));
  return r;
}