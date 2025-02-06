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

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline auto diag_matrix() const
{
  return this->asDiagonal().toDenseMatrix();
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline auto subvector(Index start_id, Index end_id) const
{
  assert_release(end_id >= 0 && start_id >= 0);
  assert_release(end_id < this->size() && start_id <= end_id);
  return this->segment(start_id,end_id-start_id+1);
}