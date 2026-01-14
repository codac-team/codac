/** 
 *  \file codac2_matrices.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
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

#pragma once

#include <type_traits>
#include "codac2_Interval.h"
#include "codac2_Interval_operations.h"
#include "codac2_assert.h"

namespace Eigen
{
  template<int R,int C>
  concept IsVectorOrRow = (C == 1 || R == 1);

  template<typename Scalar>
  concept IsIntervalDomain = std::is_same_v<Scalar,codac2::Interval>;
}

#define EIGEN_MATRIXBASE_PLUGIN "codac2_MatrixBase_addons_include.h"
#define EIGEN_MATRIX_PLUGIN "codac2_Matrix_addons_include.h"
// The option #define EIGEN_INITIALIZE_MATRICES_BY_ZERO is not used because
// of the interval cases: an IntervalMatrix would have its components initialized
// to 0 instead of [-oo,oo].

#ifndef EIGEN_NO_DEBUG
/* Disables Eigen's assertions if defined.
 * Not defined by default, unless the NDEBUG macro is defined
 * (this is a standard C++ macro which disables all asserts).
 * https://eigen.tuxfamily.org/dox/TopicPreprocessorDirectives.html
 */
//#define EIGEN_NO_DEBUG // uncomment to disable Eigen's assertions
#endif

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Eigen
{
  template<> struct NumTraits<codac2::Interval>
   : NumTraits<double> // permits to get the epsilon, dummy_precision, lowest, highest functions
  {
    typedef codac2::Interval Real;
    typedef codac2::Interval NonInteger;
    typedef codac2::Interval Nested;
    typedef codac2::Interval Scalar;
    typedef double RealScalar;
   
    enum {
      IsComplex = 0,
      IsInteger = 0,
      IsSigned = 1,
      RequireInitialization = 1,
      ReadCost = 1,
      AddCost = 3,
      MulCost = 3
    };
  };

  template<typename BinOp>
  struct ScalarBinaryOpTraits<codac2::Interval,double,BinOp>
  { typedef codac2::Interval ReturnType; };

  template<typename BinOp>
  struct ScalarBinaryOpTraits<double,codac2::Interval,BinOp>
  { typedef codac2::Interval ReturnType; };
}

namespace codac2
{
  using Eigen::Dynamic;
  
  inline const Interval& conj(const Interval& x) { return x; }
  inline const Interval& real(const Interval& x) { return x; }
  inline Interval imag(const Interval&)          { return 0.; }
  //inline Interval abs(const Interval& x)         { return codac2::abs(x); }
  inline Interval abs2(const Interval& x)        { return codac2::sqr(x); }

  template<typename Scalar,int R,int C>
  using Mat = Eigen::Matrix<Scalar,R,C>;

  template<int R,int C>
  struct is_interval_based<Eigen::Matrix<double,R,C>> : std::false_type {};

  template<int R,int C>
  struct is_interval_based<Eigen::Matrix<Interval,R,C>> : std::true_type {};

  template<typename T,int R,int C>
  struct is_ctc<Eigen::Matrix<T,R,C>> : std::false_type {};

  template<typename T,int R,int C>
  struct is_sep<Eigen::Matrix<T,R,C>> : std::false_type {};

  template<typename T>
  concept is_matrix_base_double =
    requires {
      typename Eigen::internal::traits<std::decay_t<T>>::Scalar;
      requires std::same_as<
        typename Eigen::internal::traits<std::decay_t<T>>::Scalar,
        double
      >;
    };
}

namespace codac2
{
  /**
   * \brief Compute the element-wise absolute value of a matrix.
   *
   * This function takes an Eigen matrix expression and returns a matrix
   * where each element is replaced by its absolute value.
   * 
   * For scalar type double, the standard library fabs() is used.
   * For other scalar types, it uses the generic abs() function.
   *
   * \param x Input matrix expression.
   * \return A new Eigen matrix with the absolute values of the elements of \p x.
   */
  template<typename OtherDerived>
  inline auto abs(const Eigen::MatrixBase<OtherDerived>& x)
  {
    using M = Eigen::MatrixBase<OtherDerived>;
    Eigen::Matrix<typename M::Scalar,M::RowsAtCompileTime,M::ColsAtCompileTime> a(x.rows(),x.cols());

    for(Index i = 0 ; i < x.rows() ; i++)
      for(Index j = 0 ; j < x.cols() ; j++)
      {
        if constexpr(std::is_same_v<typename M::Scalar,double>)
          a(i,j) = fabs(x(i,j));
        else
          a(i,j) = abs(x(i,j));
      }

    return a;
  }

  /**
   * \brief Compute the element-wise floor of a matrix.
   *
   * This function returns a matrix where each element is replaced by
   * the largest integer not greater than that element.
   * 
   * Disabled for interval matrices.
   *
   * \param x Input matrix expression.
   * \return A new Eigen matrix with floored elements.
   */
  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto floor(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().floor().matrix();
  }

  /**
   * \brief Compute the element-wise ceiling of a matrix.
   *
   * This function returns a matrix where each element is replaced by
   * the smallest integer not less than that element.
   * 
   * Disabled for interval matrices.
   *
   * \param x Input matrix expression.
   * \return A new Eigen matrix with ceiled elements.
   */
  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto ceil(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().ceil().matrix();
  }

  /**
   * \brief Compute the element-wise rounding of a matrix.
   *
   * This function returns a matrix where each element is replaced by
   * the nearest integer to that element.
   * 
   * Disabled for interval matrices.
   *
   * \param x Input matrix expression.
   * \return A new Eigen matrix with rounded elements.
   */
  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto round(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().round().matrix();
  }

  /**
   * \brief Provides an Eigen IOFormat for formatting row vectors.
   *
   * This format prints elements separated by spaces,
   * with brackets around the entire row vector.
   *
   * \return An Eigen::IOFormat configured for row vector formatting.
   */
  inline Eigen::IOFormat codac_row_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "", "", "", "[ ", " ]");
  }

  /**
   * \brief Provides an Eigen IOFormat for formatting column vectors.
   *
   * This format prints elements separated by semicolons,
   * with brackets around the entire vector.
   *
   * \return An Eigen::IOFormat configured for column vector formatting.
   */
  inline Eigen::IOFormat codac_vector_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, "", " ; ", "", "", "[ ", " ]");
  }
    
  /**
   * \brief Provides an Eigen IOFormat for formatting matrices.
   *
   * This format prints elements separated by commas,
   * rows separated by new lines,
   * and brackets around the entire matrix.
   *
   * \return An Eigen::IOFormat configured for matrix formatting.
   */
  inline Eigen::IOFormat codac_matrix_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, 0, " , ", "\n", "[ ", " ]", "[", "]");
  }
}