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

/**
 * \brief Computes the volume of the interval matrix.
 * 
 * The volume is computed as the sum of the diameters of each interval element.
 * If any interval is unbounded, returns positive infinity.
 * If the matrix is empty or any interval is degenerated, returns 0.
 * 
 * \pre The Scalar type must satisfy the IsIntervalDomain concept.
 * \return The computed volume as a double precision floating point number.
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

/**
 * \brief Checks whether the interval matrix is empty.
 * 
 * An interval matrix is considered empty if any of its elements is empty.
 * 
 * \return ``true`` if any element interval is empty, ``false`` otherwise.
 */
inline bool is_empty() const
{
  for(Index i = 0 ; i < rows() ; i++)
    for(Index j = 0 ; j < cols() ; j++)
      if((*this)(i,j).is_empty())
        return true;
  return false;
}

/**
 * \brief Helper macro to create a matrix from a specific
 *        operation applied to each interval element.
 */
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

/**
 * \brief Returns a matrix containing the lower bounds of each interval element.
 * 
 * \return A matrix of doubles where each element is the lower bound of the corresponding interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto lb() const
{
  degenerate_mat(lb);
}

/**
 * \brief Returns a matrix containing the upper bounds of each interval element.
 * 
 * \return A matrix of doubles where each element is the upper bound of the corresponding interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto ub() const
{
  degenerate_mat(ub);
}

/**
 * \brief Returns a matrix containing the midpoints of each interval element.
 * 
 * The midpoint is the average of the lower and upper bounds.
 *
 * \return A matrix of doubles where each element is the midpoint of the corresponding interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mid() const
{
  degenerate_mat(mid);
}

/**
 * \brief Returns a matrix containing the magnitudes of each interval element.
 * 
 * The magnitude is max(|lower bound|, |upper bound|).
 * 
 * \return A matrix of doubles with the magnitudes of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mag() const
{
  degenerate_mat(mag);
}

/**
 * \brief Returns a matrix containing the mignitudes of each interval element.
 * 
 * \return A matrix of doubles with the mignitudes of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto mig() const
{
  degenerate_mat(mig);
}

/**
 * \brief Returns a matrix containing the signed magnitudes of each interval element.
 * 
 * The signed magnitude is lower bound if |lower bound|>|upper bound|, upper bound otherwise.
 * 
 * \return A matrix of doubles with the signed magnitudes of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto smag() const
{
  degenerate_mat(smag);
}

/**
 * \brief Returns a matrix containing the signed mignitudes of each interval element.
 * 
 * \return A matrix of doubles with the signed mignitudes of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto smig() const
{
  degenerate_mat(smig);
}

/**
 * \brief Returns a matrix with random values chosen inside each interval element.
 * 
 * Each element in the resulting matrix is a random number uniformly sampled inside the interval.
 * 
 * \return A matrix of doubles containing random samples within each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rand() const
{
  degenerate_mat(rand);
}

/**
 * \brief Returns a matrix containing the radii of each interval element.
 * 
 * The radius is half the diameter of the interval.
 * 
 * \return A matrix of doubles with the radii of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto rad() const
{
  degenerate_mat(rad);
}

/**
 * \brief Returns a matrix containing the diameters of each interval element.
 * 
 * The diameter is the difference between upper and lower bounds.
 * 
 * \return A matrix of doubles with the diameters of each interval.
 */
template<typename U=Scalar>
  requires IsIntervalDomain<U>
inline auto diam() const
{
  degenerate_mat(diam);
}

/**
 * \brief Checks if this interval matrix contains the specified matrix \p x.
 * 
 * This means every element of \p x is contained in the corresponding interval element of this matrix.
 * 
 * \param x The matrix to test for containment.
 * \return ``true`` if every element of \p x is contained within the corresponding interval element.
 */
inline bool contains(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _contains(x);
}

/**
 * \brief Template version to check containment of a matrix with arbitrary derived type.
 * 
 * \param x The matrix to test.
 * \return ``true`` if this interval matrix contains \p x.
 */
template<typename OtherDerived>
inline bool contains(const MatrixBase<OtherDerived>& x) const
{
  return _contains(x);
}

/**
 * \brief Internal helper function to check containment.
 */
template<typename T>
inline bool _contains(const T& x) const
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

/**
 * \brief Checks if the interior of this interval matrix contains the specified matrix \p x.
 * 
 * The interior containment means strict containment, not just boundary.
 * 
 * \param x The matrix to test.
 * \return ``true`` if the interior of this matrix contains \p x.
 */
inline bool interior_contains(const Matrix<double,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _interior_contains(x);
}

/**
 * \brief Template version to check interior containment of a matrix with arbitrary derived type.
 * 
 * \param x The matrix to test.
 * \return ``true`` if interior contains \p x.
 */
template<typename OtherDerived>
inline bool interior_contains(const MatrixBase<OtherDerived>& x) const
{
  return _interior_contains(x);
}

/**
 * \brief Internal helper function to check interior containment.
 */
template<typename T>
inline bool _interior_contains(const T& x) const
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

/**
 * \brief Checks if the interval matrix contains any unbounded intervals.
 * 
 * \return ``true`` if any interval element is unbounded, ``false`` otherwise.
 */
inline bool is_unbounded() const
{
  if(this->is_empty()) return false;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_unbounded())
        return true;
  return false;
}

/**
 * \brief Checks if the interval matrix is degenerated.
 * 
 * An interval matrix is degenerated if all its elements are degenerated intervals.
 * 
 * \return ``true`` if all interval elements are degenerated.
 */
inline bool is_degenerated() const
{
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).is_degenerated())
        return false;
  return true;
}

/**
 * \brief Checks if the interval matrix is flat.
 *
 * A matrix is considered flat if at least one of its intervals is degenerated (i.e., has zero diameter),
 * or if the matrix is empty.
 *
 * \return ``true`` if the matrix is empty or contains at least one degenerated interval.
 */
inline bool is_flat() const
{
  if(this->is_empty()) return true;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_degenerated()) // don't use diam() because of roundoff
        return true;
  return false;
}

/**
 * \brief Checks whether this matrix intersects with another matrix of intervals.
 *
 * Intersection means that every corresponding interval pair from both matrices has a non-empty intersection.
 *
 * \param x A matrix of intervals of the same size.
 * \return ``true`` if all corresponding elements intersect, ``false`` otherwise.
 */
inline bool intersects(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _intersects(x);
}

/**
 * \brief Checks whether this matrix intersects with another Eigen-compatible matrix.
 *
 * \param x The matrix to test for intersection.
 * \return ``true`` if all corresponding interval elements intersect, ``false`` otherwise.
 */
template<typename OtherDerived>
inline bool intersects(const MatrixBase<OtherDerived>& x) const
{
  return _intersects(x);
}

/**
 * \brief Internal helper that performs intersection checking.
 */
template<typename OtherDerived>
inline bool _intersects(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks if this matrix is disjoint with another matrix of intervals.
 *
 * Disjoint means there exists at least one interval pair that does not intersect.
 *
 * \param x A matrix of intervals.
 * \return ``true`` if at least one pair of intervals is disjoint, ``false`` otherwise.
 */
inline bool is_disjoint(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_disjoint(x);
}

/**
 * \brief Checks if this matrix is disjoint with another matrix of compatible type.
 *
 * \param x The matrix to test.
 * \return ``true`` if at least one pair of corresponding intervals is disjoint, ``false`` otherwise.
 */
template<typename OtherDerived>
inline bool is_disjoint(const MatrixBase<OtherDerived>& x) const
{
  return _is_disjoint(x);
}

/**
 * \brief Internal helper for disjointness checking.
 */
template<typename OtherDerived>
inline bool _is_disjoint(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether this matrix overlaps with another.
 *
 * Overlap requires that all interval pairs overlap.
 *
 * \param x The matrix of intervals to test overlap against.
 * \return ``true`` if all corresponding intervals overlap.
 */
inline bool overlaps(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _overlaps(x);
}

/**
 * \brief Checks whether this matrix overlaps with another matrix of compatible type.
 *
 * \param x The matrix to test overlap against.
 * \return ``true`` if all interval pairs overlap.
 */
template<typename OtherDerived>
inline bool overlaps(const MatrixBase<OtherDerived>& x) const
{
  return _overlaps(x);
}

/**
 * \brief Internal helper to check overlap.
 */
template<typename OtherDerived>
inline bool _overlaps(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether this matrix is a subset of another interval matrix.
 *
 * Each interval element of this matrix must be a subset of the corresponding interval in \p x.
 * An empty matrix is considered a subset of any matrix of the same size.
 *
 * \param x A matrix of intervals.
 * \return ``true`` if all elements of this matrix are subsets of those in \p x.
 */
inline bool is_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_subset(x);
}

/**
 * \brief Checks whether this matrix is a subset of another matrix (Eigen-compatible).
 *
 * \param x The matrix to check against.
 * \return ``true`` if all elements are subsets.
 */
template<typename OtherDerived>
inline bool is_subset(const MatrixBase<OtherDerived>& x) const
{
  return _is_subset(x);
}

/**
 * \brief Internal helper for subset check.
 */
template<typename T>
inline bool _is_subset(const T& x) const
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

/**
 * \brief Checks whether this matrix is a strict subset of another matrix.
 *
 * A strict subset means it is a subset and at least one element is strictly contained (not equal).
 *
 * \param x The matrix to compare to.
 * \return ``true`` if this matrix is strictly contained in \p x.
 */
inline bool is_strict_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_strict_subset(x);
}

/**
 * \brief Checks whether this matrix is a strict subset of another matrix (Eigen-compatible).
 *
 * \param x The matrix to compare to.
 * \return ``true`` if strictly contained.
 */
template<typename OtherDerived>
inline bool is_strict_subset(const MatrixBase<OtherDerived>& x) const
{
  return _is_strict_subset(x);
}

/**
 * \brief Internal helper for strict subset check.
 */
template<typename T>
inline bool _is_strict_subset(const T& x) const
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

/**
 * \brief Checks whether this matrix is an interior subset of another.
 *
 * Every interval in this matrix must be strictly inside (in the interior of) the corresponding one in \p x.
 *
 * \param x The matrix to check against.
 * \return ``true`` if this matrix is an interior subset of \p x.
 */
inline bool is_interior_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_interior_subset(x);
}

/**
 * \brief Checks whether this matrix is an interior subset of another matrix (Eigen-compatible).
 *
 * \param x The matrix to compare with.
 * \return ``true`` if each element is in the interior of the corresponding one in \p x.
 */
template<typename OtherDerived>
inline bool is_interior_subset(const MatrixBase<OtherDerived>& x) const
{
  return _is_interior_subset(x);
}

/**
 * \brief Internal helper for interior subset checking.
 */
template<typename OtherDerived>
inline bool _is_interior_subset(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether this matrix is a strict interior subset of another matrix.
 *
 * A strict interior subset means that each interval in this matrix lies strictly within the interior
 * of the corresponding interval in \p x.
 *
 * \param x The matrix to compare against.
 * \return ``true`` if every interval in this matrix is strictly inside the corresponding interval in \p x.
 */
inline bool is_strict_interior_subset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_strict_interior_subset(x);
}

/**
 * \brief Checks whether this matrix is a strict interior subset of another Eigen-compatible matrix.
 *
 * \param x The matrix to compare with.
 * \return ``true`` if all intervals are strictly inside the corresponding intervals in \p x.
 */
template<typename OtherDerived>
inline bool is_strict_interior_subset(const MatrixBase<OtherDerived>& x) const
{
  return _is_strict_interior_subset(x);
}

/**
 * \brief Internal helper for strict interior subset relation.
 */
template<typename OtherDerived>
inline bool _is_strict_interior_subset(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether this matrix is a superset of another interval matrix.
 *
 * This means that each interval in this matrix fully contains the corresponding interval in \p x.
 *
 * \param x The matrix to compare with.
 * \return ``true`` if every interval in this matrix is a superset of the corresponding one in \p x.
 */
inline bool is_superset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_superset(x);
}

/**
 * \brief Checks whether this matrix is a superset of another Eigen-compatible matrix.
 *
 * \param x The matrix to compare to.
 * \return ``true`` if this matrix fully contains all intervals of \p x.
 */
template<typename OtherDerived>
inline bool is_superset(const MatrixBase<OtherDerived>& x) const
{
  return _is_superset(x);
}

/**
 * \brief Internal helper for superset check.
 */
template<typename OtherDerived>
inline bool _is_superset(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether this matrix is a strict superset of another matrix.
 *
 * A strict superset means that each interval contains the corresponding one in \p x,
 * and at least one interval strictly contains its counterpart.
 *
 * \param x The matrix to compare against.
 * \return ``true`` if this matrix strictly contains the other matrix.
 */
inline bool is_strict_superset(const Matrix<codac2::Interval,RowsAtCompileTime,ColsAtCompileTime>& x) const
{
  return _is_strict_superset(x);
}

/**
 * \brief Checks whether this matrix is a strict superset of an Eigen-compatible matrix.
 *
 * \param x The matrix to compare with.
 * \return ``true`` if this matrix strictly contains the other matrix.
 */
template<typename OtherDerived>
inline bool is_strict_superset(const MatrixBase<OtherDerived>& x) const
{
  return _is_strict_superset(x);
}

/**
 * \brief Internal helper for strict superset check.
 */
template<typename OtherDerived>
inline bool _is_strict_superset(const MatrixBase<OtherDerived>& x) const
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

/**
 * \brief Checks whether at least one interval in the matrix is bisectable.
 *
 * \note Examples of non bisectable intervals are [0,next_float(0)] or [DBL_MAX,+oo).
 *
 * \return ``true`` if at least one element in the matrix satisfies that condition
 */
inline bool is_bisectable() const
{
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if((*this)(i,j).is_bisectable())
        return true;
  return false;
}

/**
 * \brief Checks whether all intervals in the matrix have integer lower and upper bounds.
 *
 * \return ``true`` if every interval in the matrix has integer lower and upper bounds; ``false`` otherwise.
 */
inline bool has_integer_bounds() const
{
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      if(!(*this)(i,j).has_integer_bounds())
        return false;
  return true;
}