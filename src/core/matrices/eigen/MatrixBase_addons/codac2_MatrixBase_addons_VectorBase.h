/** 
 *  \file codac2_MatrixBase_addons_VectorBase.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

/**
 * \brief Converts a vector into a diagonal matrix.
 *
 * Creates a square matrix whose diagonal entries are taken from the vector.
 * Off-diagonal entries are set to zero. The matrix returned has size equal to the
 * size of the vector.
 *
 * \return A dense matrix with this vector on its diagonal.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline auto diag_matrix() const
{
  return this->asDiagonal().toDenseMatrix();
}

/**
 * \brief Extracts a subvector from the current vector.
 *
 * Returns a new vector containing elements from index \p start_id to \p end_id.
 *
 * \param start_id The starting index of the subvector.
 * \param end_id   The ending index of the subvector.
 * \return A subvector containing elements in the range [start_id, end_id].
 *
 * \pre `0 <= start_id <= end_id < this->size()`
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline auto subvector(Index start_id, Index end_id) const
{
  assert_release(end_id >= 0 && start_id >= 0);
  assert_release(end_id < this->size() && start_id <= end_id);
  return this->segment(start_id,end_id-start_id+1);
}