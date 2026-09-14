/** 
 *  \file codac2_SlicedTube_operations.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>

#include "codac2_SlicedTube.h"
#include "codac2_math.h"

namespace codac2
{
  #define macro_unary_tube(f) \
  { \
    auto y = x1; \
    for(auto it = y.tdomain()->begin() ; it != y.tdomain()->end() ; it++) \
    { \
      auto sy = y.slice(it); \
      sy->codomain() = f(sy->codomain()); \
    } \
    return y; \
  } \

  #define macro_binary_tube_tube(f,output_type) \
  { \
    assert_release(x1.tdomain() == x2.tdomain()); \
    auto y = output_type; \
    for(auto it = y.tdomain()->begin() ; it != y.tdomain()->end() ; it++) \
    { \
      auto sy = y.slice(it); \
      sy->codomain() = f(x1.slice(it)->codomain(), x2.slice(it)->codomain()); \
    } \
    return y; \
  } \

  #define macro_binary_real_tube(f) \
  { \
    auto y = x2; \
    for(auto it = y.tdomain()->begin() ; it != y.tdomain()->end() ; it++) \
    { \
      auto sy = y.slice(it); \
      sy->codomain() = f(x1, sy->codomain()); \
    } \
    return y; \
  } \

  #define macro_binary_tube_real(f) \
  { \
    auto y = x1; \
    for(auto it = y.tdomain()->begin() ; it != y.tdomain()->end() ; it++) \
    { \
      auto sy = y.slice(it); \
      sy->codomain() = f(sy->codomain(), x2); \
    } \
    return y; \
  } \

  #define macro_member_binary_tube_tube(f) \
  { \
    assert_release(x1.tdomain() == x2.tdomain()); \
    for(auto it = x1.tdomain()->begin() ; it != x1.tdomain()->end() ; it++) \
    { \
      auto sx1 = x1.slice(it); \
      sx1->codomain() = f(sx1->codomain(), x2.slice(it)->codomain()); \
    } \
    return x1; \
  } \

  #define macro_member_binary_tube_real(f) \
  { \
    for(auto it = x1.tdomain()->begin() ; it != x1.tdomain()->end() ; it++) \
    { \
      auto sx1 = x1.slice(it); \
      sx1->codomain() = f(sx1->codomain(), x2); \
    } \
    return x1; \
  } \

  template<typename U>
  struct is_slicedtube : std::false_type {};

  template<typename U>
  struct is_slicedtube<SlicedTube<U>> : std::true_type {};

  template<typename U>
  inline constexpr bool is_slicedtube_v = is_slicedtube<std::remove_cvref_t<U>>::value;

  template<typename Q>
  concept NonSlicedTube = !is_slicedtube_v<Q>;

  template<typename T>
  concept NonScalarTubeCodomain =
    !std::is_same_v<std::remove_cvref_t<T>, double> &&
    !std::is_same_v<std::remove_cvref_t<T>, Interval>;

  template<typename T>
  inline T operator_tube_union(const T& x1, const T& x2) { return x1 | x2; }

  template<typename T>
  inline T operator_tube_intersection(const T& x1, const T& x2) { return x1 & x2; }

  template<typename T, typename X1, typename X2>
  inline T operator_tube_add(const X1& x1, const X2& x2) { return x1 + x2; }

  template<typename T, typename X1, typename X2>
  inline T operator_tube_sub(const X1& x1, const X2& x2) { return x1 - x2; }

  template<typename T, typename X1, typename X2>
  inline T operator_tube_mul(const X1& x1, const X2& x2) { return x1 * x2; }

  template<typename T>
  inline T operator_tube_scal_mul(const Interval& x1, const T& x2) { return x1 * x2; }

  template<typename T>
  inline T operator_tube_mul_scal(const T& x1, const Interval& x2) { return x1 * x2; }

  inline IntervalVector operator_tube_mul_vec(const IntervalMatrix& x1, const IntervalVector& x2) { return x1 * x2; }

  template<typename T, typename X1, typename X2>
  inline T operator_tube_div(const X1& x1, const X2& x2) { return x1 / x2; }

  template<typename T, typename X1>
  inline T operator_tube_div_scal(const X1& x1, const Interval& x2) { return x1 / x2; }

  /**
   * \brief Pointwise hull-union assignment of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\sqcup[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)\sqcup[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\sqcup[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand, updated in place.
   * \param x2 right operand.
   * \return \p x1 after the pointwise hull-union.
   */
  template<typename T>
  inline SlicedTube<T>& operator|=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_union<T>);

  /**
   * \brief Pointwise hull-union of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\sqcup[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)\sqcup[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\sqcup[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The pointwise hull-union tube.
   */
  template<typename T>
  inline SlicedTube<T> operator|(const SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_binary_tube_tube(operator_tube_union<T>,x1);

  /**
   * \brief Pointwise intersection assignment of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\cap[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)\cap[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\cap[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand, updated in place.
   * \param x2 right operand.
   * \return \p x1 after the pointwise intersection.
   */
  template<typename T>
  inline SlicedTube<T>& operator&=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_intersection<T>);

  /**
   * \brief Pointwise intersection of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\cap[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)\cap[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\cap[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The pointwise intersection tube.
   */
  template<typename T>
  inline SlicedTube<T> operator&(const SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_binary_tube_tube(operator_tube_intersection<T>,x1);

  /**
   * \brief Unary plus.
   *
   * According to \p T, the operand is respectively denoted
   * \f$[x_1](\cdot)\f$, \f$[\mathbf{x}_1](\cdot)\f$ or \f$[\mathbf{A}_1](\cdot)\f$.
   *
   * \param x1 operand.
   * \return \p x1 unchanged.
   */
  template<typename T>
  inline const SlicedTube<T>& operator+(const SlicedTube<T>& x1)
  {
    return x1;
  }

  /**
   * \brief Pointwise sum of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)+[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)+[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)+[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The pointwise sum tube.
   */
  template<typename T>
  inline SlicedTube<T> operator+(const SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_binary_tube_tube(operator_tube_add<T>,x1);

  /**
   * \brief Pointwise sum of a tube and a constant object with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)+x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)+\mathbf{x}_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)+\mathbf{A}_2\f$.
   *
   * \param x1 left tube operand.
   * \param x2 right constant operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator+(const SlicedTube<T>& x1, const Q& x2)
    macro_binary_tube_real(operator_tube_add<T>);

  /**
   * \brief Pointwise sum of a constant object and a tube with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$x_1+[x_2](\cdot)\f$,
   * \f$\mathbf{x}_1+[\mathbf{x}_2](\cdot)\f$ or
   * \f$\mathbf{A}_1+[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left constant operand.
   * \param x2 right tube operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator+(const Q& x1, const SlicedTube<T>& x2)
    macro_binary_real_tube(operator_tube_add<T>);

  /**
   * \brief Pointwise addition assignment with a constant object of matching codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right constant operand.
   * \return \p x1 after the pointwise addition.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T>& operator+=(SlicedTube<T>& x1, const Q& x2)
    macro_member_binary_tube_real(operator_tube_add<T>);

  /**
   * \brief Pointwise addition assignment with a tube of the same codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right tube operand.
   * \return \p x1 after the pointwise addition.
   */
  template<typename T>
  inline SlicedTube<T>& operator+=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_add<T>);

  /**
   * \brief Unary minus.
   *
   * According to \p T, the operand is respectively denoted
   * \f$[x_1](\cdot)\f$, \f$[\mathbf{x}_1](\cdot)\f$ or \f$[\mathbf{A}_1](\cdot)\f$.
   *
   * \param x1 operand.
   * \return The pointwise opposite tube.
   */
  template<typename T>
  inline SlicedTube<T> operator-(const SlicedTube<T>& x1)
  {
    return -1. * x1;
  }

  /**
   * \brief Pointwise difference of two tubes with the same codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)-[x_2](\cdot)\f$,
   * \f$[\mathbf{x}_1](\cdot)-[\mathbf{x}_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)-[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The pointwise difference tube.
   */
  template<typename T>
  inline SlicedTube<T> operator-(const SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_binary_tube_tube(operator_tube_sub<T>,x1);

  /**
   * \brief Pointwise difference between a tube and a constant object with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)-x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)-\mathbf{x}_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)-\mathbf{A}_2\f$.
   *
   * \param x1 left tube operand.
   * \param x2 right constant operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator-(const SlicedTube<T>& x1, const Q& x2)
    macro_binary_tube_real(operator_tube_sub<T>);

  /**
   * \brief Pointwise difference between a constant object and a tube with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$x_1-[x_2](\cdot)\f$,
   * \f$\mathbf{x}_1-[\mathbf{x}_2](\cdot)\f$ or
   * \f$\mathbf{A}_1-[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left constant operand.
   * \param x2 right tube operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator-(const Q& x1, const SlicedTube<T>& x2)
    macro_binary_real_tube(operator_tube_sub<T>);

  /**
   * \brief Pointwise subtraction assignment with a constant object of matching codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right constant operand.
   * \return \p x1 after the pointwise subtraction.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T>& operator-=(SlicedTube<T>& x1, const Q& x2)
    macro_member_binary_tube_real(operator_tube_sub<T>);

  /**
   * \brief Pointwise subtraction assignment with a tube of the same codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right tube operand.
   * \return \p x1 after the pointwise subtraction.
   */
  template<typename T>
  inline SlicedTube<T>& operator-=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_sub<T>);

  /**
   * \brief Pointwise multiplication of a scalar interval and a tube.
   *
   * According to \p T, the operation is respectively
   * \f$x_1\,[x_2](\cdot)\f$,
   * \f$x_1\,[\mathbf{x}_2](\cdot)\f$ or
   * \f$x_1\,[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 scalar interval.
   * \param x2 right tube operand.
   * \return The resulting tube.
   */
  template<typename T>
    requires (!std::is_same_v<T, double>)
  inline SlicedTube<T> operator*(const Interval& x1, const SlicedTube<T>& x2)
    macro_binary_real_tube(operator_tube_scal_mul<T>);

  /**
   * \brief Pointwise multiplication of a tube by a scalar interval.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\,x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)\,x_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)\,x_2\f$.
   *
   * \param x1 left tube operand.
   * \param x2 scalar interval.
   * \return The resulting tube.
   */
  template<typename T>
    requires (!std::is_same_v<T, double>)
  inline SlicedTube<T> operator*(const SlicedTube<T>& x1, const Interval& x2)
    macro_binary_tube_real(operator_tube_mul_scal<T>);

  /**
   * \brief Pointwise multiplication of two scalar interval tubes.
   *
   * \f$[x_1](\cdot)\,[x_2](\cdot)\f$
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The resulting scalar interval tube.
   */
  inline SlicedTube<Interval> operator*(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(operator_tube_mul<Interval>,x1);

  /**
   * \brief Pointwise multiplication of two interval-vector tubes.
   *
   * \f$[\mathbf{x}_1](\cdot)\,[\mathbf{x}_2](\cdot)\f$
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The resulting interval-vector tube.
   */
  inline SlicedTube<IntervalVector> operator*(const SlicedTube<IntervalVector>& x1, const SlicedTube<IntervalVector>& x2)
    macro_binary_tube_tube(operator_tube_mul<IntervalVector>,x1);

  /**
   * \brief Pointwise multiplication of two interval-matrix tubes.
   *
   * \f$[\mathbf{A}_1](\cdot)\,[\mathbf{A}_2](\cdot)\f$
   *
   * \param x1 left operand.
   * \param x2 right operand.
   * \return The resulting interval-matrix tube.
   */
  inline SlicedTube<IntervalMatrix> operator*(const SlicedTube<IntervalMatrix>& x1, const SlicedTube<IntervalMatrix>& x2)
    macro_binary_tube_tube(operator_tube_mul<IntervalMatrix>,x1);

  /**
   * \brief Pointwise multiplication of a scalar interval tube and a non-scalar tube.
   *
   * Depending on \p T, the operation is
   * \f$[x_1](\cdot)\,[\mathbf{x}_2](\cdot)\f$ or
   * \f$[x_1](\cdot)\,[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 scalar interval tube.
   * \param x2 non-scalar tube operand.
   * \return The resulting tube.
   */
  template<typename T>
    requires NonScalarTubeCodomain<T>
  inline SlicedTube<T> operator*(const SlicedTube<Interval>& x1, const SlicedTube<T>& x2)
    macro_binary_tube_tube(operator_tube_mul<T>,x2);

  /**
   * \brief Pointwise multiplication of a non-scalar tube and a scalar interval tube.
   *
   * Depending on \p T, the operation is
   * \f$[\mathbf{x}_1](\cdot)\,[x_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\,[x_2](\cdot)\f$.
   *
   * \param x1 non-scalar tube operand.
   * \param x2 scalar interval tube.
   * \return The resulting tube.
   */
  template<typename T>
    requires NonScalarTubeCodomain<T>
  inline SlicedTube<T> operator*(const SlicedTube<T>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(operator_tube_mul<T>,x1);

  /**
   * \brief Pointwise multiplication of a tube and a constant object with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)\,x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)\,\mathbf{x}_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)\,\mathbf{A}_2\f$.
   *
   * \param x1 left tube operand.
   * \param x2 right constant operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator*(const SlicedTube<T>& x1, const Q& x2)
    macro_binary_tube_real(operator_tube_mul<T>);

  /**
   * \brief Pointwise multiplication of a constant object and a tube with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$x_1\,[x_2](\cdot)\f$,
   * \f$\mathbf{x}_1\,[\mathbf{x}_2](\cdot)\f$ or
   * \f$\mathbf{A}_1\,[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 left constant operand.
   * \param x2 right tube operand.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator*(const Q& x1, const SlicedTube<T>& x2)
    macro_binary_real_tube(operator_tube_mul<T>);

  /**
   * \brief Pointwise multiplication of an interval-matrix tube by an interval-vector tube.
   *
   * \f$[\mathbf{A}_1](\cdot)\,[\mathbf{x}_2](\cdot)\f$
   *
   * \param x1 interval-matrix tube.
   * \param x2 interval-vector tube.
   * \return The resulting interval-vector tube.
   */
  inline SlicedTube<IntervalVector> operator*(const SlicedTube<IntervalMatrix>& x1, const SlicedTube<IntervalVector>& x2)
    macro_binary_tube_tube(operator_tube_mul_vec,x2);

  /**
   * \brief Pointwise multiplication assignment with a constant object of matching codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right constant operand.
   * \return \p x1 after the pointwise multiplication.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T>& operator*=(SlicedTube<T>& x1, const Q& x2)
    macro_member_binary_tube_real(operator_tube_mul<T>);

  /**
   * \brief Pointwise multiplication assignment with a tube of the same codomain type.
   *
   * \param x1 left tube operand, updated in place.
   * \param x2 right tube operand.
   * \return \p x1 after the pointwise multiplication.
   */
  template<typename T>
  inline SlicedTube<T>& operator*=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_mul<T>);

  /**
   * \brief Pointwise multiplication assignment by a scalar interval tube.
   *
   * Depending on \p T, the operation is
   * \f$[\mathbf{x}_1](\cdot)\,[x_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)\,[x_2](\cdot)\f$.
   *
   * \param x1 non-scalar tube operand, updated in place.
   * \param x2 scalar interval tube.
   * \return \p x1 after the pointwise multiplication.
   */
  template<typename T>
    requires NonScalarTubeCodomain<T>
  inline SlicedTube<T>& operator*=(SlicedTube<T>& x1, const SlicedTube<Interval>& x2)
    macro_member_binary_tube_tube(operator_tube_mul<T>);

  /**
   * \brief Pointwise division of a tube by a scalar interval.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)/x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)/x_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)/x_2\f$.
   *
   * \param x1 numerator tube.
   * \param x2 scalar interval denominator.
   * \return The resulting tube.
   */
  template<typename T>
    requires (!std::is_same_v<T, double>)
  inline SlicedTube<T> operator/(const SlicedTube<T>& x1, const Interval& x2)
    macro_binary_tube_real(operator_tube_div_scal<T>);

  /**
   * \brief Pointwise division of two scalar interval tubes.
   *
   * \f$[x_1](\cdot)/[x_2](\cdot)\f$
   *
   * \param x1 numerator tube.
   * \param x2 denominator tube.
   * \return The resulting scalar interval tube.
   */
  inline SlicedTube<Interval> operator/(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(operator_tube_div<Interval>,x1);

  /**
   * \brief Pointwise division of a non-scalar tube by a scalar interval tube.
   *
   * Depending on \p T, the operation is
   * \f$[\mathbf{x}_1](\cdot)/[x_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)/[x_2](\cdot)\f$.
   *
   * \param x1 non-scalar numerator tube.
   * \param x2 scalar interval denominator tube.
   * \return The resulting tube.
   */
  template<typename T>
    requires NonScalarTubeCodomain<T>
  inline SlicedTube<T> operator/(const SlicedTube<T>& x1, const SlicedTube<Interval>& x2)
    macro_binary_tube_tube(operator_tube_div<T>,x1);

  /**
   * \brief Pointwise division of a tube by a constant object with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$[x_1](\cdot)/x_2\f$,
   * \f$[\mathbf{x}_1](\cdot)/\mathbf{x}_2\f$ or
   * \f$[\mathbf{A}_1](\cdot)/\mathbf{A}_2\f$.
   *
   * \param x1 numerator tube.
   * \param x2 denominator constant object.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator/(const SlicedTube<T>& x1, const Q& x2)
    macro_binary_tube_real(operator_tube_div<T>);

  /**
   * \brief Pointwise division of a constant object by a tube with matching codomain type.
   *
   * According to \p T, the operation is respectively
   * \f$x_1/[x_2](\cdot)\f$,
   * \f$\mathbf{x}_1/[\mathbf{x}_2](\cdot)\f$ or
   * \f$\mathbf{A}_1/[\mathbf{A}_2](\cdot)\f$.
   *
   * \param x1 numerator constant object.
   * \param x2 denominator tube.
   * \return The resulting tube.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T> operator/(const Q& x1, const SlicedTube<T>& x2)
    macro_binary_real_tube(operator_tube_div<T>);

  /**
   * \brief Pointwise division assignment with a constant object of matching codomain type.
   *
   * \param x1 numerator tube, updated in place.
   * \param x2 denominator constant object.
   * \return \p x1 after the pointwise division.
   */
  template<typename T, typename Q>
    requires NonSlicedTube<Q>
  inline SlicedTube<T>& operator/=(SlicedTube<T>& x1, const Q& x2)
    macro_member_binary_tube_real(operator_tube_div<T>);

  /**
   * \brief Pointwise division assignment with a tube of the same codomain type.
   *
   * \param x1 numerator tube, updated in place.
   * \param x2 denominator tube.
   * \return \p x1 after the pointwise division.
   */
  template<typename T>
  inline SlicedTube<T>& operator/=(SlicedTube<T>& x1, const SlicedTube<T>& x2)
    macro_member_binary_tube_tube(operator_tube_div<T>);

  /**
   * \brief Pointwise division assignment by a scalar interval tube.
   *
   * Depending on \p T, the operation is
   * \f$[\mathbf{x}_1](\cdot)/[x_2](\cdot)\f$ or
   * \f$[\mathbf{A}_1](\cdot)/[x_2](\cdot)\f$.
   *
   * \param x1 non-scalar numerator tube, updated in place.
   * \param x2 scalar interval denominator tube.
   * \return \p x1 after the pointwise division.
   */
  template<typename T>
    requires NonScalarTubeCodomain<T>
  inline SlicedTube<T>& operator/=(SlicedTube<T>& x1, const SlicedTube<Interval>& x2)
    macro_member_binary_tube_tube(operator_tube_div<T>);

  /**
   * \brief Pointwise square of a scalar interval tube.
   *
   * \f$\big([x_1](\cdot)\big)^2\f$
   *
   * \param x1 scalar interval tube.
   * \return The squared tube.
   */
  SlicedTube<Interval> sqr(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise square root of a scalar interval tube.
   *
   * \f$\sqrt{[x_1](\cdot)}\f$
   *
   * \param x1 scalar interval tube.
   * \return The square-root tube.
   */
  SlicedTube<Interval> sqrt(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise power of a scalar interval tube with an integer exponent.
   *
   * \f$\big([x_1](\cdot)\big)^{x_2}\f$
   *
   * \param x1 scalar interval tube.
   * \param x2 integer exponent.
   * \return The resulting tube.
   */
  SlicedTube<Interval> pow(const SlicedTube<Interval>& x1, int x2);

  /**
   * \brief Pointwise power of a scalar interval tube with an interval exponent.
   *
   * \f$\big([x_1](\cdot)\big)^{x_2}\f$
   *
   * \param x1 scalar interval tube.
   * \param x2 interval exponent.
   * \return The resulting tube.
   */
  SlicedTube<Interval> pow(const SlicedTube<Interval>& x1, const Interval& x2);

  /**
   * \brief Pointwise exponential of a scalar interval tube.
   *
   * \f$\exp\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> exp(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise natural logarithm of a scalar interval tube.
   *
   * \f$\log\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> log(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise cosine of a scalar interval tube.
   *
   * \f$\cos\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> cos(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise sine of a scalar interval tube.
   *
   * \f$\sin\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> sin(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise tangent of a scalar interval tube.
   *
   * \f$\tan\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> tan(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise arc-cosine of a scalar interval tube.
   *
   * \f$\arccos\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> acos(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise arc-sine of a scalar interval tube.
   *
   * \f$\arcsin\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> asin(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise arc-tangent of a scalar interval tube.
   *
   * \f$\arctan\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> atan(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise two-argument arc-tangent of two scalar interval tubes.
   *
   * \f$\operatorname{atan2}\!\big([x_1](\cdot),[x_2](\cdot)\big)\f$
   *
   * \param x1 first scalar interval tube.
   * \param x2 second scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> atan2(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise two-argument arc-tangent of a scalar interval tube and an interval.
   *
   * \f$\operatorname{atan2}\!\big([x_1](\cdot),x_2\big)\f$
   *
   * \param x1 scalar interval tube.
   * \param x2 scalar interval.
   * \return The resulting tube.
   */
  SlicedTube<Interval> atan2(const SlicedTube<Interval>& x1, const Interval& x2);

  /**
   * \brief Pointwise two-argument arc-tangent of an interval and a scalar interval tube.
   *
   * \f$\operatorname{atan2}\!\big(x_1,[x_2](\cdot)\big)\f$
   *
   * \param x1 scalar interval.
   * \param x2 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> atan2(const Interval& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise hyperbolic cosine of a scalar interval tube.
   *
   * \f$\cosh\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> cosh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise hyperbolic sine of a scalar interval tube.
   *
   * \f$\sinh\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> sinh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise hyperbolic tangent of a scalar interval tube.
   *
   * \f$\tanh\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> tanh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise inverse hyperbolic cosine of a scalar interval tube.
   *
   * \f$\operatorname{arccosh}\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> acosh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise inverse hyperbolic sine of a scalar interval tube.
   *
   * \f$\operatorname{arcsinh}\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> asinh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise inverse hyperbolic tangent of a scalar interval tube.
   *
   * \f$\operatorname{arctanh}\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> atanh(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise absolute value of a scalar interval tube.
   *
   * \f$\left|[x_1](\cdot)\right|\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> abs(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise minimum of two scalar interval tubes.
   *
   * \f$\min\!\big([x_1](\cdot),[x_2](\cdot)\big)\f$
   *
   * \param x1 first scalar interval tube.
   * \param x2 second scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> min(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise minimum of a scalar interval tube and an interval.
   *
   * \f$\min\!\big([x_1](\cdot),x_2\big)\f$
   *
   * \param x1 scalar interval tube.
   * \param x2 scalar interval.
   * \return The resulting tube.
   */
  SlicedTube<Interval> min(const SlicedTube<Interval>& x1, const Interval& x2);

  /**
   * \brief Pointwise minimum of an interval and a scalar interval tube.
   *
   * \f$\min\!\big(x_1,[x_2](\cdot)\big)\f$
   *
   * \param x1 scalar interval.
   * \param x2 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> min(const Interval& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise maximum of two scalar interval tubes.
   *
   * \f$\max\!\big([x_1](\cdot),[x_2](\cdot)\big)\f$
   *
   * \param x1 first scalar interval tube.
   * \param x2 second scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> max(const SlicedTube<Interval>& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise maximum of a scalar interval tube and an interval.
   *
   * \f$\max\!\big([x_1](\cdot),x_2\big)\f$
   *
   * \param x1 scalar interval tube.
   * \param x2 scalar interval.
   * \return The resulting tube.
   */
  SlicedTube<Interval> max(const SlicedTube<Interval>& x1, const Interval& x2);

  /**
   * \brief Pointwise maximum of an interval and a scalar interval tube.
   *
   * \f$\max\!\big(x_1,[x_2](\cdot)\big)\f$
   *
   * \param x1 scalar interval.
   * \param x2 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> max(const Interval& x1, const SlicedTube<Interval>& x2);

  /**
   * \brief Pointwise sign of a scalar interval tube.
   *
   * \f$\operatorname{sign}\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> sign(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise integer-part operator applied to a scalar interval tube.
   *
   * \f$\operatorname{integer}\!\big([x_1](\cdot)\big)\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> integer(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise floor of a scalar interval tube.
   *
   * \f$\lfloor [x_1](\cdot) \rfloor\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> floor(const SlicedTube<Interval>& x1);

  /**
   * \brief Pointwise ceiling of a scalar interval tube.
   *
   * \f$\lceil [x_1](\cdot) \rceil\f$
   *
   * \param x1 scalar interval tube.
   * \return The resulting tube.
   */
  SlicedTube<Interval> ceil(const SlicedTube<Interval>& x1);
}