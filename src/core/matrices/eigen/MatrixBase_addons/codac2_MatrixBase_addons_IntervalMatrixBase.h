/** 
 *  \file codac2_MatrixBase_addons_IntervalMatrixBase.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrixBase. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrixBase (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline double volume() const
{
  if(this->is_empty())
    return 0.;

  double v = 0.;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
    {
      if((*this)(i,j).is_unbounded()) return codac2::oo;
      if((*this)(i,j).is_degenerated()) return 0.;
      v += std::log((*this)(i,j).diam());
    }
  return std::exp(v);
}

inline bool is_empty() const
{
  for(Index i = 0 ; i < rows() ; i++)
    for(Index j = 0 ; j < cols() ; j++)
      if((*this)(i,j).is_empty())
        return true;
  return false;
}

#define degenerate_mat(op) \
  Matrix<double,RowsAtCompileTime,ColsAtCompileTime> m(this->rows(),this->cols()); \
  \
  if(this->is_empty()) \
    m.init(std::numeric_limits<double>::quiet_NaN()); \
  \
  else \
  { \
    for(Index i = 0 ; i < this->rows() ; i++) \
      for(Index j = 0 ; j < this->cols() ; j++) \
        m(i,j) = (*this)(i,j).op(); \
  } \
  \
  return m; \

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto lb() const
{
  degenerate_mat(lb);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto ub() const
{
  degenerate_mat(ub);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mid() const
{
  degenerate_mat(mid);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mag() const
{
  degenerate_mat(mag);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mig() const
{
  degenerate_mat(mig);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rand() const
{
  degenerate_mat(rand);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rad() const
{
  degenerate_mat(rad);
}

template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto diam() const
{
  degenerate_mat(diam);
}

template<typename OtherDerived>
inline bool contains(const MatrixBase<OtherDerived>& x) const
{
  assert_release(x.size() == this->size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).contains(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool interior_contains(const MatrixBase<OtherDerived>& x) const
{
  assert_release(x.size() == this->size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).interior_contains(x(i,j)))
        return false;

  return true;
}

inline bool is_unbounded() const
{
  if(this->is_empty()) return false;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_unbounded())
        return true;
  return false;
}

inline bool is_degenerated() const
{
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).is_degenerated())
        return false;
  return true;
}

inline bool is_flat() const
{
  if(this->is_empty()) return true;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_degenerated()) // don't use diam() because of roundoff
        return true;
  return false;
}

template<typename OtherDerived>
inline bool intersects(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).intersects(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_disjoint(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_disjoint(x(i,j)))
        return true;

  return false;
}

template<typename OtherDerived>
inline bool overlaps(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).overlaps(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_subset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).is_subset(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_strict_subset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  if(!is_subset(x))
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_strict_subset(x(i,j)))
        return true;

  return false;
}

template<typename OtherDerived>
inline bool is_interior_subset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).is_interior_subset(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_strict_interior_subset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return true;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).is_strict_interior_subset(x(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_superset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!x(i,j).is_subset((*this)(i,j)))
        return false;

  return true;
}

template<typename OtherDerived>
inline bool is_strict_superset(const MatrixBase<OtherDerived>& x) const
{
  assert_release(this->size() == x.size());

  if(this->is_empty())
    return false;

  if(!is_superset(x))
    return false;

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(x(i,j).is_strict_subset((*this)(i,j)))
        return true;

  return false;
}

inline bool is_bisectable() const
{
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_bisectable())
        return true;
  return false;
}