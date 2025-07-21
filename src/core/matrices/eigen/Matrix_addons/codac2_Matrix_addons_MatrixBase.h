/** 
 *  \file codac2_Matrix_addons_MatrixBase.h
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
 * \brief Deleted default constructor to prevent default instantiation
 *        when either the number of rows or columns is dynamic.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (R == -1 || C == -1)
Matrix() = delete;

/**
 * \brief Constructs a matrix with given number of rows and columns.
 * 
 * Initializes the matrix storage with dimensions (``r``,``c``).
 * If ``Scalar`` is not an interval domain, the matrix is initialized with zeros.
 * 
 * \param r Number of rows (runtime).
 * \param c Number of columns (runtime).
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
explicit Matrix(int r, int c)
{
  Base::template _init2<int,int>(r,c);
  if constexpr(!IsIntervalDomain<Scalar>)
    init(0.);
}

/**
 * \brief Constructs a non-vector matrix with given dimensions and initializes
 *        it from a raw array of values.
 * 
 * Copies the values from the \p values array into the matrix elements.
 * If \p values is null, initializes all elements to zero.
 *  
 * \param r Number of rows (runtime).
 * \param c Number of columns (runtime).
 * \param values Pointer to array of values used to initialize matrix elements.
 *               Can be null, in which case the matrix is zero-initialized.
 * 
 * \pre The runtime dimensions \p r and \p c must match the compile-time
 *      dimensions ``R`` and ``C`` if these are fixed (not -1).
 * \pre \p r and \p c must be non-negative.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
explicit Matrix(int r, int c, const Scalar values[])
  : Matrix<Scalar,R,C>(r,c)
{
  assert((R==(int)r || R==-1) && (C==(int)c || C==-1));
  assert(r >= 0 && c >= 0);

  if(values == 0)
    init(Scalar(0.)); // in case the user called Matrix(r,c,0) and 0 is interpreted as NULL!

  else
  {
    Index k = 0;
    for(Index i = 0 ; i < this->rows() ; i++)
      for(Index j = 0 ; j < this->cols() ; j++)
        (*this)(i,j) = values[k++];
    assert(k == this->size());
  }
}

/**
 * \brief Creates a matrix of size (``r``,``c``) filled with zeros.
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \return A matrix of size (``r``,``c``) with all elements initialized to zero.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> zero(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Zero(r,c);
}

/**
 * \brief Creates a matrix of size (``r``,``c``) filled with ones.
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \return A matrix of size (``r``,``c``) with all elements initialized to one.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> ones(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Ones(r,c);
}

/**
 * \brief Creates an identity matrix of size (``r``,``c``).
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \return An identity matrix of size (``r``,``c``).
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> eye(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return MatrixBase<Matrix<Scalar,R,C>>::Identity(r,c);
}

/**
 * \brief Creates a matrix of size (``r``,``c``) with all elements set to \p x.
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \param x The value to assign to every element.
 * \return A matrix of size (``r``,``c``) with all elements equal to \p x.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> constant(Index r, Index c, const Scalar& x)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Constant(r,c,x);
}

/**
 * \brief Creates a matrix of size (``r``,``c``) with random elements.
 * 
 * \note This function is not named "rand" to avoid ambiguity with the member function "rand".
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \return A matrix of size (``r``,``c``) with random elements.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> random(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Random(r,c);
}

/**
 * \brief Resizes the matrix to (``r``,``c``), preserving existing values where possible.
 * 
 * \param r New number of rows.
 * \param c New number of columns.
 * 
 * \details
 * This function resizes the matrix while preserving the data in the overlapping region
 * of the old and new sizes. Unlike Eigen's ``resize()``, which discards old data, this
 * function copies existing values into the resized matrix.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline void resize_save_values(Index r, Index c)
{
  // With resize() of Eigen, the data is reallocated and all previous values are lost.
  auto copy = *this;
  this->resize(r,c);
  for(Index i = 0 ; i < std::min((Index)copy.rows(),r) ; i++)
    for(Index j = 0 ; j < std::min((Index)copy.cols(),c) ; j++)
      (*this)(i,j) = copy(i,j);
}