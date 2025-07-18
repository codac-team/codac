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

/**
 * \brief Constructs a vector or row matrix from a list of double values.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param l Initializer list of double values to populate the matrix.
 * 
 * \details
 * If the compile-time size is fixed to 1 (vector or row), the size is 1;
 * otherwise, the size matches the initializer list size.
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

/**
 * \brief Constructs a vector or row matrix of size \p n from an array of double values.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Number of elements (must be greater than 0).
 * \param values Pointer to an array of double values to initialize the matrix.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsIntervalDomain<Scalar>) && (IsVectorOrRow<R,C>)
explicit Matrix(int n, double values[])
  : Matrix<double,R,C>(R == 1 ? 1 : n, C == 1 ? 1 : n, values)
{
  assert_release(n > 0);
}