/** 
 *  \file codac2_Matrix_addons_Vector.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<Scalar>) && (IsVectorOrRow<R,C>)
Matrix(std::initializer_list<double> l)
  : Matrix<double,R,C>(R == 1 ? 1 : l.size(), C == 1 ? 1 : l.size())
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = li;
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<Scalar>) && (IsVectorOrRow<R,C>)
explicit Matrix(int n, double values[])
  : Matrix<double,R,C>(R == 1 ? 1 : n, C == 1 ? 1 : n, values)
{
  assert_release(n > 0);
}