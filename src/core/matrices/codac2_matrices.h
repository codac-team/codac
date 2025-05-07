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
}

namespace codac2
{
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

  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto floor(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().floor().matrix();
  }

  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto ceil(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().ceil().matrix();
  }

  template<typename OtherDerived>
    requires (!Eigen::IsIntervalDomain<typename OtherDerived::Scalar>)
  inline auto round(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.array().round().matrix();
  }

  inline Eigen::IOFormat codac_row_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "", "", "", "[ ", " ]");
  }

  inline Eigen::IOFormat codac_vector_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, "", " ; ", "", "", "[ ", " ]");
  }

  inline Eigen::IOFormat codac_matrix_fmt()
  {
    return Eigen::IOFormat(Eigen::StreamPrecision, 0, " , ", "\n", "[ ", " ]", "[", "]");
  }
}