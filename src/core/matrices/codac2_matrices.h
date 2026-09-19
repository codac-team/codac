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

/* Eigen's blocked product kernel accumulates in two different ways. When
 * EIGEN_HAS_SINGLE_INSTRUCTION_MADD is defined it writes c = pmadd(a,b,c),
 * which is exactly the fused multiply-add the accumulator expects. Otherwise
 * it multiplies into a temporary declared with the type of the right-hand
 * operand, tmp = pmul(a,tmp), then adds it with padd(c,tmp). That second path
 * is a register-allocation workaround for compilers without a fused
 * multiply-add, and it silently assumes that the product has the type of the
 * right-hand operand. Codac breaks that assumption everywhere it multiplies
 * matrices of two different scalar types -- a Matrix by an IntervalMatrix,
 * for instance, whose product is an IntervalMatrix. The kernel then either
 * fails to compile (no padd overload accepts the two types) or, worse,
 * narrows the product down to the type of the right-hand operand.
 *
 * Every architecture Eigen vectorizes defines the macro in its packet-math
 * header, so this path is normally unreachable. MSVC on arm64 is the exception:
 * Eigen only enables NEON on __ARM_NEON, which that compiler does not define,
 * so the whole library falls back on scalar packets and the arm64 builds fail
 * to compile those mixed-type products. Defining the macro here selects the
 * pmadd path unconditionally, which is the one all the other targets already
 * take, and keeps the same results everywhere. It must be defined before Eigen
 * is included, hence its position at the very top of this file.
 */
#ifndef EIGEN_HAS_SINGLE_INSTRUCTION_MADD
#define EIGEN_HAS_SINGLE_INSTRUCTION_MADD
#endif

/* Eigen's own EIGEN_ASSUME_ALIGNED(PTR, ALIGN_BYTES), defined in
 * Eigen/src/Core/util/Memory.h, hints the compiler that a packet load/store
 * is aligned by calling std::assume_aligned<8 * (ALIGN_BYTES)>(PTR) when the
 * standard library provides it (as libc++ does). ALIGN_BYTES is always
 * called with an already-byte-valued Alignment enumerator (Aligned16 = 16,
 * Aligned32 = 32...), so that extra "8 *" claims eight times the alignment
 * Eigen itself computed -- e.g. a NEON Packet2d, whose unpacket_traits
 * report Aligned16, ends up asserted as 128-byte aligned. libc++'s
 * std::assume_aligned() is, among major standard libraries, the one that
 * actually verifies such a claim at run time under -fsanitize=alignment
 * (GCC/libstdc++'s __builtin_assume_aligned() treats it as a silent
 * optimizer hint instead), which is why every heap-allocated Eigen::Matrix
 * op trips it under UBSan on macOS/Clang -- e.g. in codac2::gauss_jordan()
 * or any matrix product -- while the same code is silent on
 * Linux/GCC. The buffers themselves are never actually misaligned for the
 * SIMD width Eigen emits; only this hint's arithmetic is wrong. Memory.h
 * guards its own definition with #ifndef EIGEN_ASSUME_ALIGNED, so defining
 * it here first -- before Eigen is included, like EIGEN_HAS_SINGLE_
 * INSTRUCTION_MADD above -- replaces the miscomputed hint with a no-op
 * (Eigen's own fallback for compilers lacking std::assume_aligned/
 * __builtin_assume_aligned) rather than trying to patch the multiplier,
 * so this keeps working even if a future Eigen release changes it again.
 * It does not change which aligned/unaligned load or store instruction
 * Eigen selects for a given expression, only this compiler-hint call.
 */
#ifndef EIGEN_ASSUME_ALIGNED
#define EIGEN_ASSUME_ALIGNED(PTR, ALIGN_BYTES)
#endif

#include <type_traits>
#include "codac2_Interval.h"
#include "codac2_Interval_operations.h"
#include "codac2_assert.h"

namespace Eigen
{
  template<int R,int C>
  concept IsVectorOrRow = (C == 1 || R == 1);

  template<typename Scalar>
  concept IsIntervalDomain = codac2::is_interval_based<Scalar>::value;
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