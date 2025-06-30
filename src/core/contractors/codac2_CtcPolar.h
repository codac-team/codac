/** 
 *  \file codac2_CtcPolar.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoit Desrochers, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class CtcPolar
   * 
   * \brief Implements the polar constraint on \f$x\in\mathbb{R}\f$, \f$y\in\mathbb{R}\f$,
   * \f$\rho\in\mathbb{R}^+\f$ and \f$\theta\in\mathbb{R}\f$ such that:
   * 
   * @f[
   * \left\{~\begin{array}{lll}x&=&\rho\cos\theta\\y&=&\rho\sin\theta\end{array}\right.
   * @f]
   * 
   * The contractor can be applied either on a 4d box or on a set of four intervals.
   * 
   * This contractor is minimal.
   * 
   * In robotics, it can be used to express a range-and-bearing constraint.
   * It can be seen as an extension of \f$\mathcal{C}_{\textrm{dist}}\f$ enriched with the bearing information. 
   */
  class CtcPolar : public Ctc<CtcPolar,IntervalVector>
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcPolar()
        : Ctc<CtcPolar,IntervalVector>(4)
      { }

      /**
       * \brief Applies \f$\mathcal{C}_{\textrm{polar}}\big([\mathbf{x}]\big)\f$.
       * 
       * \param x ``IntervalVector`` for \f$(x,y,\rho,\theta)^\intercal\f$
       */
      void contract(IntervalVector& x) const;

      /**
       * \brief Applies \f$\mathcal{C}_{\textrm{polar}}\big([x],[y],[\rho],[\theta]\big)\f$.
       *
       * \param x ``Interval`` for \f$x\f$
       * \param y ``Interval`` for \f$y\f$
       * \param rho ``Interval`` for \f$\rho\f$
       * \param theta ``Interval`` for \f$\theta\f$
       */
      void contract(Interval& x, Interval& y, Interval& rho, Interval& theta) const;
  };
}