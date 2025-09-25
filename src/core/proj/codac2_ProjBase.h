/** 
 *  \file codac2_CtcProj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \brief Base class for projection-related operations.
   *
   * ``ProjBase`` provides common utilities for projection operators such as ``CtcProj`` and ``SepProj``.
   * It handles the decomposition and reconstruction of boxes based on a 
   * projection index set \f$x_i\in\{0, \dots, n-1\}\f$.
   *
   * Used internally by projection contractors and separators to manage subspaces efficiently.
   */
  class ProjBase
  {
    protected:

      /**
       * \brief Constructs a ``ProjBase`` object.
       *
       * Initializes the projection index set \f$x_i\f$ and the complementary vector \f$\mathbf{y}\f$
       * in a space of dimension \f$n\f$.
       *
       * \param proj_indices Indices \f$x_i\f$ of the projected components (kept in \f$\mathbf{x}\f$).
       * \param y Interval vector for the non-projected components.
       * \param default_eps Default epsilon used to explore along the y-column to be projected.
       */
      ProjBase(const std::vector<Index>& proj_indices, const IntervalVector& y, double default_eps = 0.01);

      /**
       * \brief Extracts the projected subvector \f$\mathbf{x}\f$ from
       * a full vector \f$\mathbf{w}\in\mathbb{R}^n\f$.
       *
       * Given a full vector \f$\mathbf{w}\in\mathbb{R}^n\f$ and a set of projection indices 
       * \f$x_i\in\subset\{0, \dots, n-1\}\f$, this function extracts the components of \f$\mathbf{w}\f$ 
       * whose indices belong to \f$x_i\f$.
       * 
       * \param w The full input vector \f$\mathbf{w}\in\mathbb{R}^n\f$.
       * \return IntervalVector The extracted vector \f$\mathbf{x}\f$, containing the components of \f$\mathbf{w}\f$
       *                        at indices defined in \f$x_i\f$.
       *
       * \note It complements ``extract_y()``.
       */
      IntervalVector extract_x(const IntervalVector& w) const;

      /**
       * \brief Extracts the complementary subvector \f$\mathbf{y}\f$ from
       * a full vector \f$\mathbf{w}\in\mathbb{R}^n\f$.
       *
       * Given a full vector \f$\mathbf{w}\in\mathbb{R}^n\f$ and a set of projection indices
       * \f$x_i\in\{0, \dots, n-1\}\f$, this function extracts the components of \f$\mathbf{w}\f$
       * that are **not** in the projection (i.e., whose indices are in the complement of \f$x_i\f$).
       *
       * \param w The full input vector \f$\mathbf{w}\in\mathbb{R}^n\f$.
       * \return IntervalVector The extracted vector containing the components of \f$\mathbf{w}\f$
       *                        at indices not in \f$x_i\f$.
       *
       * \note It complements ``extract_x()``.
       */
      IntervalVector extract_y(const IntervalVector& w) const;

      /**
       * \brief Returns a full vector of dimension \f$n\f$ by merging the two
       * input sub-vectors ``x`` and ``y``, according to the projection index sets
       * ``_xi`` and ``_y``. The values from ``x`` are placed at the positions
       * specified by ``_xi``, and the values from ``y`` fill the remaining positions.
       * 
       * \pre ``x.size() == _xi.size()``
       * \pre ``y.size() == _y.size()``
       *
       * \param x IntervalVector containing the components corresponding to ``_xi``.
       * \param y IntervalVector containing the remaining components.
       * \return ``IntervalVector`` the full reconstructed vector of dimension \f$n\f$, combining ``x`` and ``y``.
       */
      IntervalVector cart_prod_xy(const IntervalVector& x, const IntervalVector& y) const;

      /**
       * \brief Returns the component in \f$\{0, \dots, n-1\}\f$ with the largest diameter,
       * among the components of the complementary projection.
       *
       * \param y The subvector \f$\mathbf{y}\f$ (complement of the projection).
       * \return Index The \f$\mathbf{w}\f$-index of the complement components having the largest diameter.
       */
      Index y_max_diam_index(const IntervalVector& y) const;

    protected:

      const Index _n;
      const std::vector<Index> _xi;
      const IntervalVector _y;
      const double _default_eps;
  };
}