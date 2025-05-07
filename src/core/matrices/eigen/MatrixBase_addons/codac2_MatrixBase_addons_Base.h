/** 
 *  \file codac2_MatrixBase_addons_Base.h
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

inline bool is_squared() const
{
  return this->rows() == this->cols();
}

inline auto squared_norm() const
{
  return this->squaredNorm();
}

#define minmax_item(op) \
  Scalar m = (*this)(0,0); /* first element */ \
  for(Index i = 0 ; i < this->rows() ; i++) \
    for(Index j = 0 ; j < this->cols() ; j++) \
    { \
      if constexpr(std::is_same_v<Scalar,codac2::Interval>) \
        m = codac2::op(m,(*this)(i,j)); \
      else \
        m = std::op(m,(*this)(i,j)); \
    } \
  return m; \

inline Scalar min_coeff() const
{
  minmax_item(min);
}

inline Scalar max_coeff() const
{
  minmax_item(max);
}

inline bool is_nan() const
{
  return this->array().isNaN().any();
}