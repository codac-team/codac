/** 
 *  \file codac2_CtcDist.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class CtcDist
   * 
   * \brief Implements the distance constraint on \f$\mathbf{a}\in\mathbb{R}^2\f$,
   * \f$\mathbf{b}\in\mathbb{R}^2\f$ and \f$d\in\mathbb{R}\f$ such that:
   * 
   * @f[
   * d^2=(a_1-b_1)^2+(a_2-b_2)^2.
   * @f]
   * 
   * The contractor can be applied either on a 5d box or on a set of five intervals.
   * 
   * This contractor is minimal.
   */
  class CtcDist : public Ctc<CtcDist,IntervalVector>
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcDist();

      /**
       * \brief Applies \f$\mathcal{C}_{\textrm{dist}}\big([\mathbf{x}]\big)\f$.
       * 
       * \param x ``IntervalVector`` for \f$(a_1,a_2,b_1,b_2,d)^\intercal\f$
       */
      void contract(IntervalVector& x) const;

      /**
       * \brief Applies \f$\mathcal{C}_{\textrm{dist}}\big([a_1],[a_2],[b_1],[b_2],[d]\big)\f$.
       *
       * \param a1 ``Interval`` for \f$a_1\f$
       * \param a2 ``Interval`` for \f$a_2\f$
       * \param b1 ``Interval`` for \f$b_1\f$
       * \param b2 ``Interval`` for \f$b_2\f$
       * \param d ``Interval`` for \f$d\f$
       */
      void contract(Interval& a1, Interval& a2, Interval& b1, Interval& b2, Interval& d) const;
  };
}