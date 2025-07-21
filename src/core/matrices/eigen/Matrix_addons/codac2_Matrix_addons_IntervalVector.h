/** 
 *  \file codac2_Matrix_addons_IntervalVector.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
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

/**
 * \brief Constructs an interval vector or interval row from an initializer list of floating point values.
 * 
 * Converts each value in the list into a degenerate interval and stores it in the structure.
 * 
 * \pre The structure must be a vector or row vector (\c IsVectorOrRow).
 * 
 * \param l Initializer list of values to convert into intervals.
 * 
 * \pre The initializer list \p l must not be empty.
 */
template<typename T,int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires (std::is_arithmetic_v<T> && IsIntervalDomain<Scalar> && IsVectorOrRow<R,C>)
Matrix(std::initializer_list<T> l)
  : Matrix<codac2::Interval,R,C>(R == 1 ? 1 : l.size(), C == 1 ? 1 : l.size())
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

/**
 * \brief Constructs an interval vector or interval row from an initializer list of initializer lists of doubles.
 * 
 * Converts each inner list into an interval and stores it in the structure.
 * 
 * \pre The structure must be a vector or row vector (\c IsVectorOrRow).
 * 
 * \param l Initializer list of initializer lists of doubles representing intervals.
 * 
 * \pre The initializer list \p l must not be empty.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar> && IsVectorOrRow<R,C>
Matrix(std::initializer_list<std::initializer_list<double>> l)
  : Matrix<codac2::Interval,R,C>(R == 1 ? 1 : l.size(), C == 1 ? 1 : l.size())
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = codac2::Interval(li);
}

/**
 * \brief Constructs an interval vector or interval row from an initializer list of intervals.
 * 
 * Copies each interval in the list into the structure.
 * 
 * \pre The structure must be a vector or row vector (\c IsVectorOrRow).
 * 
 * \param l Initializer list of intervals.
 * 
 * \pre The initializer list \p l must not be empty.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar> && IsVectorOrRow<R,C>
Matrix(std::initializer_list<codac2::Interval> l)
  : Matrix<codac2::Interval,R,C>(R == 1 ? 1 : l.size(), C == 1 ? 1 : l.size())
{
  assert_release(!std::empty(l));
  Index i = 0;
  for(const auto& li : l)
    (*this)[i++] = li;
}

/**
 * \brief Constructs an interval vector or interval row from a size and array of bounds.
 * 
 * Initializes the structure intervals using the given array of bounds.
 *
 * \pre The structure must be a vector or row vector (\c IsVectorOrRow).
 * 
 * \param n Number of components (must be positive).
 * \param bounds Array of bounds, each element containing lower and upper bound as double.
 * 
 * \pre \p n must be greater than zero.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar> && IsVectorOrRow<R,C>
Matrix(int n, const double bounds[][2])
  : Matrix<codac2::Interval,R,C>(R == 1 ? 1 : n, C == 1 ? 1 : n, bounds)
{
  assert_release(n > 0);
}

/**
 * \brief Creates an interval vector (or interval row) of size \p n initialized with empty intervals.
 * 
 * \pre The structure must be a vector or row vector (\c IsVectorOrRow).
 * 
 * \param n Size of the vector (must be non-negative).
 * \return A vector or row matrix of size \p n with all elements set to empty intervals.
 */
template<int R=RowsAtCompileTime,int C=ColsAtCompileTime>
  requires IsIntervalDomain<Scalar> && IsVectorOrRow<R,C>
inline static auto empty(Index n)
{
  assert_release(n >= 0);
  return Matrix<codac2::Interval,R,C>::constant((int)n,codac2::Interval::empty());
}