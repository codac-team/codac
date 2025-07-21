/** 
 *  \file codac2_Matrix_addons_VectorBase.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
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
 * \brief Constructs a vector or row matrix with size \p n.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Number of elements.
 * 
 * \details
 * Initializes the matrix to zero if ``Scalar`` is not an interval domain.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
explicit Matrix(int n)
{
  Base::template _init2<int,int>(R == 1 ? 1 : n, C == 1 ? 1 : n);
  if constexpr(!IsIntervalDomain<Scalar>)
    init(0.);
}

/**
 * \brief Constructs a vector or row matrix from a vector of doubles.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param v Vector of doubles to initialize the matrix.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
explicit Matrix(const std::vector<double>& v)
  : Matrix<Scalar,R,C>(R == 1 ? 1 : v.size(), C == 1 ? 1 : v.size())
{
  for(size_t i = 0 ; i < v.size() ; i++)
    (*this)[i] = v[i];
}

/**
 * \brief Access element at index \p i (mutable).
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param i Index of the element.
 * \return Reference to the element at index \p i.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline Scalar& operator()(Index i)
{
  return const_cast<Scalar&>(const_cast<const Matrix<Scalar,R,C>*>(this)->operator()(i));
}

/**
 * \brief Access element at index \p i (const).
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param i Index of the element.
 * \return Const reference to the element at index \p i.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline const Scalar& operator()(Index i) const
{
  assert_release(i >= 0 && i < this->size());
  return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator()(i);
}

/**
 * \brief Access element at index \p i (mutable) via operator[].
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param i Index of the element.
 * \return Reference to the element at index \p i.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline Scalar& operator[](Index i)
{
  return const_cast<Scalar&>(const_cast<const Matrix<Scalar,R,C>*>(this)->operator[](i));
}

/**
 * \brief Access element at index \p i (const) via operator[].
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param i Index of the element.
 * \return Const reference to the element at index \p i.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline const Scalar& operator[](Index i) const
{
  assert_release(i >= 0 && i < this->size());
  return this->PlainObjectBase<Matrix<Scalar,R,C>>::operator[](i);
}

/**
 * \brief Creates a zero vector or row matrix of size \p n.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Size of the vector.
 * \return A zero-initialized matrix of size \p n.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> zero(Index n)
{
  assert_release(n >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Zero(n);
}

/**
 * \brief Creates a ones vector or row matrix of size \p n.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Size of the vector.
 * \return A matrix filled with ones of size \p n.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> ones(Index n)
{
  assert_release(n >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Ones(n);
}

/**
 * \brief Creates a constant vector or row matrix of size \p n filled with \p x.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Size of the vector.
 * \param x The constant value to fill.
 * \return A matrix filled with \p x of size \p n.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> constant(Index n, const Scalar& x)
{
  assert_release(n >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Constant(n,x);
}

/**
 * \brief Creates a random vector or row matrix of size \p n.
 * 
 * \pre The matrix is a vector or row vector (\c IsVectorOrRow<R,C>).
 * 
 * \param n Size of the vector.
 * \return A matrix filled with random values of size \p n.
 * 
 * \note Named 'random' to avoid ambiguity with any member function named 'rand'.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline static Matrix<Scalar,R,C> random(Index n)
{
  assert_release(n >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Random(n);
}

/**
 * \brief Inserts values from matrix \p x starting at index \p start_id.
 * 
 * \pre The matrix and \p x are both vectors or row vectors.
 * 
 * \param start_id Start index for insertion.
 * \param x The matrix whose values will be inserted.
 */
template<typename OtherDerived,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C> && IsVectorOrRow<MatrixBase<OtherDerived>::RowsAtCompileTime,MatrixBase<OtherDerived>::ColsAtCompileTime>
inline void put(Index start_id, const MatrixBase<OtherDerived>& x)
{
  assert_release(start_id >= 0 && start_id < this->size());
  assert_release(start_id+x.size() <= this->size());

  this->segment(start_id,x.size()) << x;
}

/**
 * \brief Resizes the vector or row matrix to size \p n, preserving existing values.
 * 
 * \pre The matrix is a vector or row vector.
 * 
 * \param n The new size.
 * 
 * \details
 * Eigen's ``resize()`` discards existing data, so this function copies existing
 * values before resizing and restores them afterward.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsVectorOrRow<R,C>
inline void resize_save_values(Index n)
{
  // With resize() of Eigen, the data is reallocated and all previous values are lost.
  auto copy = *this;
  this->resize(n);
  for(Index i = 0 ; i < std::min((Index)copy.size(),n) ; i++)
    (*this)[i] = copy[i];
}