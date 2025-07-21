/** 
 *  \file codac2_Matrix_addons_Base.h
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
 * \brief Accesses a matrix element (modifiable) by its row and column indices.
 * 
 * Provides mutable access to the element at the specified row \p i and column \p j.
 * This operator allows both reading and writing of matrix elements.
 * 
 * \param i Row index of the element.
 * \param j Column index of the element.
 * \return Reference to the element at position (i,j).
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
inline Scalar& operator()(Index i, Index j)
{
  return const_cast<Scalar&>(static_cast<const Matrix<Scalar,R,C>&>(*this).operator()(i,j));
}

/**
 * \brief Accesses a matrix element (non modifiable) by its row and column indices.
 * 
 * Provides reading access to the element at the specified row \p i and column \p j.
 * This operator allows only reading of matrix elements.
 * 
 * \param i Row index of the element.
 * \param j Column index of the element.
 * \return Const reference to the element at position (i,j).
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
inline const Scalar& operator()(Index i, Index j) const
{
  assert_release(i >= 0 && i < this->rows() && j >= 0 && j < this->cols());

  if constexpr(!IsVectorOrRow<R,C>)
    return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator()(i,j);

  else
  {
    if constexpr(R == 1)
      return (*this)[j];
    return (*this)[i];
  }
}

/**
 * \brief Initializes all elements of the matrix with the scalar value \p x.
 * 
 * \param x The scalar value to assign to each element.
 * \return Reference to the current matrix object (allows method chaining).
 */
inline auto& init(const Scalar& x)
{
  for(Index i = 0 ; i < this->size() ; i++)
    *(this->data()+i) = x;
  return *this;
}

/**
 * \brief Compares this matrix with another matrix for equality.
 * 
 * \param x The other matrix to compare against.
 * \return ``true`` if matrices have the same size, shape, and identical elements; ``false`` otherwise.
 */
template<typename U_,int R_,int C_>
inline bool operator==(const Matrix<U_,R_,C_>& x) const
{
  // This operator should be related to Eigen::MatrixBase,
  // for allowing a comparison between two matrix expressions.
  // However, it is not possible to overwrite Eigen::MatrixBase::operator==.
  // Therefore, only comparisons between Eigen::Matrix types is possible.
  // This must be corrected...

  if(this->size() != x.size())
    return false;

  if constexpr(std::is_same_v<Scalar,codac2::Interval> && std::is_same_v<U_,codac2::Interval>)
  {
    if(this->is_empty() || x.is_empty())
      return this->is_empty() && x.is_empty();
  }

  if(this->rows() != x.rows() || this->cols() != x.cols())
    return false;

  for(Index i = 0 ; i < this->size() ; i++)
    if(*(this->data()+i) != *(x.data()+i))
      return false;

  return true;
}