/** 
 *  \file codac2_sympy.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <stdexcept>
#include "codac2_AnalyticFunction.h"

namespace codac2
{
  /**
   * \brief Symbolically simplifies a scalar analytic function.
   *
   * \param f Function to simplify.
   * \return A simplified analytic function.
   */
  AnalyticFunction<ScalarType>
  sympy_simplify(const AnalyticFunction<ScalarType>& f);

  /**
   * \brief Symbolically simplifies a vector analytic function componentwise.
   *
   * \param f Function to simplify.
   * \return A simplified analytic function.
   */
  AnalyticFunction<VectorType>
  sympy_simplify(const AnalyticFunction<VectorType>& f);

  /**
   * \brief Symbolically simplifies a matrix analytic function componentwise.
   *
   * \param f Function to simplify.
   * \return A simplified analytic function.
   */
  AnalyticFunction<MatrixType>
  sympy_simplify(const AnalyticFunction<MatrixType>& f);

  /**
   * \brief Rewrites a scalar analytic function in Horner form when possible.
   *
   * \param f Function to rewrite.
   * \return A rewritten analytic function.
   */
  AnalyticFunction<ScalarType>
  sympy_horner(const AnalyticFunction<ScalarType>& f);

  /**
   * \brief Rewrites a vector analytic function in Horner form componentwise.
   *
   * \param f Function to rewrite.
   * \return A rewritten analytic function.
   */
  AnalyticFunction<VectorType>
  sympy_horner(const AnalyticFunction<VectorType>& f);

  /**
   * \brief Rewrites a matrix analytic function in Horner form componentwise.
   *
   * \param f Function to rewrite.
   * \return A rewritten analytic function.
   */
  AnalyticFunction<MatrixType>
  sympy_horner(const AnalyticFunction<MatrixType>& f);

  /**
   * \brief Returns the symbolic partial derivative of a scalar function.
   *
   * The derivative is taken with respect to the flattened scalar input
   * of index \p flat_input_index.
   *
   * \param f Scalar function.
   * \param flat_input_index Flattened input index.
   * \return The symbolic partial derivative.
   */
  AnalyticFunction<ScalarType>
  sympy_partial_diff(const AnalyticFunction<ScalarType>& f, Index flat_input_index);

  /**
   * \brief Returns the symbolic partial derivative of a scalar function
   *        with respect to a scalar input variable.
   *
   * This overload is restricted to scalar variables that appear directly
   * in the function argument list.
   *
   * \param f Scalar function.
   * \param x Scalar input variable.
   * \return The symbolic partial derivative.
   */
  AnalyticFunction<ScalarType>
  sympy_partial_diff(const AnalyticFunction<ScalarType>& f, const ScalarVar& x);

  /**
   * \brief Returns the symbolic partial derivative of a scalar function
   *        with respect to a scalar input expression.
   *
   * This overload supports either a scalar input variable, or a direct scalar
   * component of a vectorial or matricial input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input expression.
   * \return The symbolic partial derivative.
   */
  AnalyticFunction<ScalarType>
  sympy_partial_diff(const AnalyticFunction<ScalarType>& f, const ScalarExpr& x);

  /**
   * \brief Returns the symbolic derivative of a scalar univariate function.
   *
   * \param f Scalar function.
   * \return The symbolic derivative.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f);

  /**
   * \brief Returns the symbolic first derivative of a scalar function with
   *        respect to a scalar input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input variable.
   * \return The symbolic derivative with respect to \p x.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f, const ScalarVar& x);

  /**
   * \brief Returns the symbolic first derivative of a scalar function with
   *        respect to a scalar input expression.
   *
   * This overload supports either a scalar input variable, or a direct scalar
   * component of a vectorial or matricial input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input expression.
   * \return The symbolic derivative with respect to \p x.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f, const ScalarExpr& x);

  /**
   * \brief Returns the symbolic derivative of given order for a scalar univariate function.
   *
   * \param f Scalar function.
   * \param order Derivation order.
   * \return The symbolic derivative of order \p order.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f, Index order);

  /**
   * \brief Returns the symbolic derivative of given order for a scalar
   *        function with respect to a scalar input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input variable.
   * \param order Derivation order.
   * \return The symbolic derivative of order \p order with respect to \p x.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f, const ScalarVar& x, Index order);

  /**
   * \brief Returns the symbolic derivative of given order for a scalar
   *        function with respect to a scalar input expression.
   *
   * This overload supports either a scalar input variable, or a direct scalar
   * component of a vectorial or matricial input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input expression.
   * \param order Derivation order.
   * \return The symbolic derivative of order \p order with respect to \p x.
   */
  AnalyticFunction<ScalarType>
  sympy_diff(const AnalyticFunction<ScalarType>& f, const ScalarExpr& x, Index order);

  /**
   * \brief Returns the symbolic gradient of a scalar function.
   *
   * \param f Scalar function.
   * \return The symbolic gradient.
   */
  AnalyticFunction<VectorType>
  sympy_gradient(const AnalyticFunction<ScalarType>& f);

  /**
   * \brief Returns the symbolic Hessian matrix of a scalar function.
   *
   * \param f Scalar function.
   * \return The symbolic Hessian matrix.
   */
  AnalyticFunction<MatrixType>
  sympy_hessian(const AnalyticFunction<ScalarType>& f);

  /**
   * \brief Returns the symbolic Jacobian matrix of a vector function.
   *
   * \param f Vector function.
   * \return The symbolic Jacobian matrix.
   */
  AnalyticFunction<MatrixType>
  sympy_diff(const AnalyticFunction<VectorType>& f);

  /**
   * \brief Returns a truncated Taylor series of a scalar univariate function.
   *
   * \param f Scalar function.
   * \param center Expansion center.
   * \param order Truncation order.
   * \return The truncated Taylor series.
   */
  AnalyticFunction<ScalarType>
  sympy_series(const AnalyticFunction<ScalarType>& f, double center, Index order);

  /**
   * \brief Returns a truncated Taylor series of a scalar function with
   *        respect to a scalar input variable.
   *
   * Other input variables are treated as constants.
   *
   * \param f Scalar function.
   * \param x Scalar input variable.
   * \param center Expansion center.
   * \param order Truncation order.
   * \return The truncated Taylor series.
   */
  AnalyticFunction<ScalarType>
  sympy_series(const AnalyticFunction<ScalarType>& f, const ScalarVar& x, double center, Index order);

  /**
   * \brief Returns a truncated Taylor series of a scalar function with
   *        respect to a scalar input expression.
   *
   * Other input variables are treated as constants. This overload supports
   * either a scalar input variable, or a direct scalar component of a
   * vectorial or matricial input variable.
   *
   * \param f Scalar function.
   * \param x Scalar input expression.
   * \param center Expansion center.
   * \param order Truncation order.
   * \return The truncated Taylor series.
   */
  AnalyticFunction<ScalarType>
  sympy_series(const AnalyticFunction<ScalarType>& f, const ScalarExpr& x, double center, Index order);

  /**
   * \brief Tests symbolic equality of two scalar analytic functions through SymPy.
   *
   * The two functions are compared on the same flattened domain: only the
   * order and total number of flattened scalar inputs matter, not the original
   * grouping into scalar, vector or matrix arguments.
   *
   * \param f First scalar function.
   * \param g Second scalar function.
   * \return True if the two functions are symbolically equal.
   */
  bool sympy_equal(const AnalyticFunction<ScalarType>& f, const AnalyticFunction<ScalarType>& g);

  /**
   * \brief Tests symbolic equality of two vector analytic functions through SymPy.
   *
   * Equality is checked componentwise on the same flattened domain.
   *
   * \param f First vector function.
   * \param g Second vector function.
   * \return True if the two functions are symbolically equal.
   */
  bool sympy_equal(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& g);

  /**
   * \brief Tests symbolic equality of two matrix analytic functions through SymPy.
   *
   * Equality is checked entrywise on the same flattened domain.
   *
   * \param f First matrix function.
   * \param g Second matrix function.
   * \return True if the two functions are symbolically equal.
   */
  bool sympy_equal(const AnalyticFunction<MatrixType>& f, const AnalyticFunction<MatrixType>& g);
}
