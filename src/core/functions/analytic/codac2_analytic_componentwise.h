/** 
 *  \file codac2_analytic_componentwise.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <functional>

#include "codac2_AnalyticFunction.h"

namespace codac2
{
  /**
   * \brief Scalar transformation applied to the entries of an analytic function output.
   *
   * This callback takes one scalar analytic expression and returns the
   * transformed scalar expression.
   */
  using ScalarExprTransform = std::function<ScalarExpr(const ScalarExpr&)>;

  /**
   * \brief Applies a scalar transformation to a scalar analytic function.
   *
   * \param f Scalar function.
   * \param transform Scalar transformation.
   * \return The transformed scalar function.
   */
  AnalyticFunction<ScalarType>
  map_scalar_entries(const AnalyticFunction<ScalarType>& f, const ScalarExprTransform& transform);

  /**
   * \brief Applies a scalar transformation componentwise to a vector analytic function.
   *
   * The output shape of \p f is preserved.
   *
   * \param f Vector function.
   * \param transform Scalar transformation.
   * \return The transformed vector function.
   */
  AnalyticFunction<VectorType>
  map_scalar_entries(const AnalyticFunction<VectorType>& f, const ScalarExprTransform& transform);

  /**
   * \brief Applies a scalar transformation entrywise to a matrix analytic function.
   *
   * The output shape of \p f is preserved.
   *
   * \param f Matrix function.
   * \param transform Scalar transformation.
   * \return The transformed matrix function.
   */
  AnalyticFunction<MatrixType>
  map_scalar_entries(const AnalyticFunction<MatrixType>& f, const ScalarExprTransform& transform);
}