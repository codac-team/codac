/** 
 *  \file codac2_Matrix_addons_IntervalMatrixBase.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrixBase. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrixBase (IBEX lib, author: Gilles Chabert)
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIX_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

// does not work.. template<typename OtherDerived1,typename OtherDerived2,typename U=Scalar,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
// does not work..   requires (
// does not work..        IsIntervalDomain<U>
// does not work..     && (!IsIntervalDomain<typename OtherDerived1::Scalar>)
// does not work..     && (!IsIntervalDomain<typename OtherDerived2::Scalar>)
// does not work..     && OtherDerived1::RowsAtCompileTime == R
// does not work..     && OtherDerived2::RowsAtCompileTime == R
// does not work..     && OtherDerived1::ColsAtCompileTime == C
// does not work..     && OtherDerived2::ColsAtCompileTime == C)
// does not work.. Matrix(MatrixBase<OtherDerived1> lb, MatrixBase<OtherDerived2> ub)
// does not work..   : Matrix<U,R,C>(lb)

/**
 * \brief Constructs an interval matrix from lower and upper bound matrices.
 * 
 * Initializes the interval matrix with the given lower bound matrix \p lb and
 * upper bound matrix \p ub.
 * 
 * Each element of the resulting interval matrix is constructed from corresponding
 * elements in \p lb and \p ub. If any lower bound element is greater than its
 * corresponding upper bound element, the matrix is set to empty.
 * 
 * \param lb Matrix of lower bounds.
 * \param ub Matrix of upper bounds.
 * 
 * \pre \p lb and \p ub must have the same size.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar>
Matrix(const Matrix<double,R,C>& lb, const Matrix<double,R,C>& ub)
  : Matrix<Scalar,R,C>(lb)
{
  assert_release(lb.size() == ub.size());

  for(Index i = 0 ; i < this->size() ; i++)
  {
    auto& lbi = *(this->data()+i);
    const auto& ubi = *(ub.data()+i);

    if(lbi.lb() > ubi)
    {
      set_empty();
      break;
    }

    else
      lbi |= ubi;
  }
}

/**
 * \brief Constructs an interval matrix from a 2D array of bounds.
 * 
 * Initializes the matrix with dimensions (``r``,``c``) where each element is set
 * to an interval constructed from the corresponding pair of lower and upper bounds
 * in the \p bounds array.
 * 
 * \param r Number of rows in the matrix.
 * \param c Number of columns in the matrix.
 * \param bounds 2D array of size ``[r*c][2]``, where each sub-array contains
 *               the lower and upper bound for one element.
 * 
 * \pre \p r and \p c must be positive.
 * \pre The \p bounds array size must exactly match the total number of elements.
 * 
 * \post Each matrix element ``(i,j)`` is initialized to ``codac2::Interval(bounds[k][0], bounds[k][1])``
 *       with `k` mapping row-major order index.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar>
Matrix(int r, int c, const double bounds[][2])
  : Matrix<Scalar,R,C>(r,c)
{
  assert_release(r > 0 && c > 0);

  Index k = 0;
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
    {
      (*this)(i,j) = codac2::Interval(bounds[k][0],bounds[k][1]);
      k++;
    }
  assert_release(k == this->size() && "incorrect array size");
}

/**
 * \brief Initializes all elements of the matrix with default intervals.
 * 
 * Sets every element in the matrix to a default-constructed ``codac2::Interval``.
 * 
 * \return Reference to the current matrix object (enables method chaining).
 */
inline auto& init()
{
  for(Index i = 0 ; i < this->size() ; i++)
    (this->data()+i)->init();
  return *this;
}

/**
 * \brief Compares this interval matrix with another matrix for equality.
 * 
 * Evaluates and casts the other matrix to an interval matrix, then
 * performs an element-wise equality comparison.
 * 
 * \param x The other matrix (of arbitrary derived type) to compare.
 * \return ``true`` if the matrices are equal after casting; ``false`` otherwise.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime,typename OtherDerived>
  requires IsIntervalDomain<Scalar>
inline bool operator==(const MatrixBase<OtherDerived>& x) const
{
  return operator==(x.eval().template cast<codac2::Interval>());
}

/**
 * \brief Returns the minimum radius among the interval elements.
 * 
 * \return The smallest radius value in the matrix.
 */
inline double min_rad() const
  requires IsIntervalDomain<Scalar>
{
  return (this->data()+extr_diam_index(true))->rad();
}

/**
 * \brief Returns the maximum radius among the interval elements.
 * 
 * \return The largest radius value in the matrix.
 */
inline double max_rad() const
  requires IsIntervalDomain<Scalar>
{
  return (this->data()+extr_diam_index(false))->rad();
}

/**
 * \brief Returns the minimum diameter among the interval elements.
 * 
 * \return The smallest diameter value in the matrix.
 */
inline double min_diam() const
  requires IsIntervalDomain<Scalar>
{
  return (this->data()+extr_diam_index(true))->diam();
}

/**
 * \brief Returns the maximum diameter among the interval elements.
 * 
 * \return The largest diameter value in the matrix.
 */
inline double max_diam() const
  requires IsIntervalDomain<Scalar>
{
  return (this->data()+extr_diam_index(false))->diam();
}

/**
 * \brief Returns the index of the element with the minimum diameter.
 * 
 * \return The index of the matrix element with the smallest diameter.
 */
inline Index min_diam_index() const
  requires IsIntervalDomain<Scalar>
{
  return extr_diam_index(true);
}

/**
 * \brief Returns the index of the element with the maximum diameter.
 * 
 * \return The index of the matrix element with the largest diameter.
 */
inline Index max_diam_index() const
  requires IsIntervalDomain<Scalar>
{
  return extr_diam_index(false);
}

/**
 * \brief Returns the index of the element with the minimum or maximum diameter.
 * 
 * Searches the matrix elements for the interval with either the smallest or largest diameter,
 * depending on the \p min flag.
 * 
 * This method handles unbounded intervals specially:
 * - If looking for minimum diameter, unbounded intervals are ignored unless no bounded intervals exist.
 * - If looking for maximum diameter, unbounded intervals are considered.
 * 
 * \param min If ``true``, returns the index of the element with minimum diameter; otherwise maximum diameter.
 * 
 * \return Index of the element with the minimum or maximum diameter.
 * 
 * \pre The matrix must not be empty (diameter undefined otherwise).
 */
inline Index extr_diam_index(bool min) const
  requires IsIntervalDomain<Scalar>
{
  // This code originates from the ibex-lib
  // See: ibex_TemplateVector.h
  // Author: Gilles Chabert

  double d = min ? codac2::oo : -1; // -1 to be sure that even a 0-diameter interval can be selected
  int selected_index = -1;
  bool unbounded = false;
  assert_release(!this->is_empty() && "Diameter of an empty IntervalVector is undefined");

  Index i;

  for(i = 0 ; i < this->size() ; i++) 
  {
    if((this->data()+i)->is_unbounded()) 
    {
      unbounded = true;
      if(!min) break;
    }
    else
    {
      double w = (this->data()+i)->diam();
      if(min ? w<d : w>d)
      {
        selected_index = i;
        d = w;
      }
    }
  }

  if(min && selected_index == -1)
  {
    assert(unbounded);
    // the selected interval is the first one.
    i = 0;
  }

  // The unbounded intervals are not considered if we look for the minimal diameter
  // and some bounded intervals have been found (selected_index!=-1)
  if(unbounded && (!min || selected_index == -1))
  {
    double pt = min ? -codac2::oo : codac2::oo; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
    selected_index = i;

    for(; i < this->size() ; i++)
    {
      if((this->data()+i)->lb() == -codac2::oo)
      {
        if((this->data()+i)->ub() == codac2::oo)
          if(!min)
          {
            selected_index = i;
            break;
          }

        if((min && (-(this->data()+i)->ub() > pt)) || (!min && (-(this->data()+i)->ub() < pt)))
        {
          selected_index = i;
          pt = -(this->data()+i)->ub();
        }
      }

      else if((this->data()+i)->ub() == codac2::oo)
        if((min && ((this->data()+i)->lb() > pt)) || (!min && ((this->data()+i)->lb() < pt)))
        {
          selected_index = i;
          pt = (this->data()+i)->lb();
        }
    }
  }

  return selected_index;
}

/**
 * \brief Marks the interval matrix as empty.
 * 
 * Sets all elements to an empty interval.
 */
inline void set_empty()
  requires IsIntervalDomain<Scalar>
{
  this->init(codac2::Interval::empty());
}

/**
 * \brief Inflates all intervals in the matrix by a fixed radius.
 * 
 * Adds radius \p r to each interval element, expanding their bounds.
 * 
 * \param r The non-negative radius by which to inflate each interval.
 * \return Reference to the current matrix object (for chaining).
 * 
 * \pre `r` must be greater than or equal to zero.
 */
inline auto& inflate(double r)
  requires IsIntervalDomain<Scalar>
{
  assert_release(r >= 0.);

  for(Index i = 0 ; i < this->size() ; i++)
    (this->data()+i)->inflate(r);
  return *this;
}

/**
 * \brief Inflates each interval in the matrix by corresponding values from another matrix.
 * 
 * Inflates each element by the radius specified at the same position in matrix \p r.
 * 
 * \param r Matrix containing non-negative inflation radii.
 * \return Reference to the current matrix object (for chaining).
 * 
 * \pre The size of this matrix and \p r must be the same.
 * \pre All elements in \p r must be greater than or equal to zero.
 */
template<typename OtherDerived>
  requires IsIntervalDomain<Scalar>
inline auto& inflate(const MatrixBase<OtherDerived>& r)
{
  assert_release(this->size() == r.size());
  assert_release(r.min_coeff() >= 0.);

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j).inflate(r(i,j));
  return *this;
}

/**
 * \brief Performs element-wise intersection assignment with another matrix.
 * 
 * Updates each element of this matrix by intersecting it with the corresponding element
 * of matrix \p x.
 * 
 * If \p x is empty, this matrix is set empty.
 * 
 * \param x The matrix to intersect with.
 * \return Reference to this matrix after intersection.
 * 
 * \pre The size of this matrix and \p x must be the same.
 */
template<typename OtherDerived>
inline auto& operator&=(const MatrixBase<OtherDerived>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<typename MatrixBase<OtherDerived>::Scalar,codac2::Interval>)
  {
    if(x.is_empty())
    {
      set_empty();
      return *this;
    }
  }
  
  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j) &= x(i,j);
  return *this;
}

/**
 * \brief Performs element-wise union assignment with another matrix.
 * 
 * Updates each element of this matrix by taking the union with the corresponding element
 * of matrix \p x.
 * 
 * If `x` is empty, this matrix remains unchanged.
 * 
 * \param x The matrix to union with.
 * \return Reference to this matrix after union.
 * 
 * \pre The size of this matrix and \p x must be the same.
 */
template<typename OtherDerived>
inline auto& operator|=(const MatrixBase<OtherDerived>& x)
{
  assert_release(this->size() == x.size());

  if constexpr(std::is_same_v<typename MatrixBase<OtherDerived>::Scalar,codac2::Interval>)
  {
    if(x.is_empty())
      return *this;
  }

  for(Index i = 0 ; i < this->rows() ; i++)
    for(Index j = 0 ; j < this->cols() ; j++)
      (*this)(i,j) |= x(i,j);
  return *this;
}

/**
 * \brief Returns the element-wise intersection of this matrix with another.
 * 
 * Creates a copy of this matrix and performs element-wise intersection assignment
 * with matrix \p x, then returns the result.
 * 
 * \param x The matrix to intersect with.
 * \return A new matrix representing the intersection.
 */
template<typename OtherDerived>
inline auto operator&(const MatrixBase<OtherDerived>& x) const
{
  auto y = *this;
  return y &= x;
}

/**
 * \brief Returns the element-wise union of this matrix with another.
 * 
 * Creates a copy of this matrix and performs element-wise union assignment
 * with the interval-casted matrix \p x, then returns the result.
 * 
 * \param x The matrix to union with.
 * \return A new matrix representing the union.
 */
template<typename OtherDerived>
inline auto operator|(const MatrixBase<OtherDerived>& x) const
{
  auto y = *this;
  return y |= x.template cast<codac2::Interval>();
}

/**
 * \brief Creates an interval matrix of given dimensions initialized with empty intervals.
 * 
 * \param r Number of rows (must be non-negative).
 * \param c Number of columns (must be non-negative).
 * \return A matrix of size (``r``,``c``) with all elements set to empty intervals.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar>
inline static auto empty(Index r, Index c)
{
  assert_release(r >= 0 && c >= 0);
  return Matrix<Scalar,R,C>::constant(r,c,codac2::Interval::empty());
}

/**
 * \brief Bisects the interval at the given index into two sub-interval matrices.
 * 
 * Splits the interval at element index \p i into two parts according to the \p ratio
 * and returns a pair of matrices having the two resulting sub-intervals.
 * 
 * \param i Index of the element to bisect (must be within valid range).
 * \param ratio Ratio to determine the split point within the interval (default 0.49, must lie in [0,1]).
 * \return A pair of matrices resulting from the bisection.
 * 
 * \pre The element at index \p i must be bisectable.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar>
inline auto bisect(Index i, float ratio = 0.49) const
{
  assert_release(i >= 0 && i < this->size());
  assert_release((this->data()+i)->is_bisectable());
  assert_release(codac2::Interval(0,1).interior_contains(ratio));

  auto p = std::make_pair(*this,*this);
  auto pi = (this->data()+i)->bisect(ratio);
  *(p.first.data()+i) = pi.first;
  *(p.second.data()+i) = pi.second;
  return p;
}

/**
 * \brief Bisects the interval with the largest diameter in the matrix.
 * 
 * Uses ``max_diam_index()`` to find the element with the largest diameter and bisects
 * it using the given ratio.
 * 
 * \param ratio Ratio to determine the split point within the interval (default 0.49).
 * \return A pair of matrices resulting from the bisection of the largest diameter interval.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar>
inline auto bisect_largest(float ratio = 0.49) const
{
  return bisect(max_diam_index(), ratio);
}