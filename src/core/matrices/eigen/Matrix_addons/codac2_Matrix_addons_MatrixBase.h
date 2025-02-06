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

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (R == -1 || C == -1)
Matrix() = delete;

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

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> zero(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Zero(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> ones(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Ones(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> eye(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return MatrixBase<Matrix<Scalar,R,C>>::Identity(r,c);
}

template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> constant(Index r, Index c, const Scalar& x)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Constant(r,c,x);
}

// Note that this static function is not called "rand"
// because of ambiguity with the member function "rand"
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (!IsVectorOrRow<R,C>)
inline static Matrix<Scalar,R,C> random(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return DenseBase<Matrix<Scalar,R,C>>::Random(r,c);
}

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