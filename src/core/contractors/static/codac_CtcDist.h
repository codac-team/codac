/** 
 *  \file
 *  CtcDist class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCDIST_H__
#define __CODAC_CTCDIST_H__

#include "codac_Ctc.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class CtcDist
   * \brief Distance constraint between two 2d vectors.
   */
  class CtcDist : public Ctc
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcDist();

      /**
       * \brief \f$\mathcal{C}_{\textrm{dist}}\big([\mathbf{x}]\big)\f$
       *
       * \param x the 5d box of domains: (x1,x2,b1,b2,d)
       */
      void contract(IntervalVector& x);

      /**
       * \brief \f$\mathcal{C}_{\textrm{dist}}\big([\mathbf{x}]\big)\f$
       *
       * \param a the 2d box of the first 2d vector
       * \param b the 2d box of the second 2d vector
       * \param d the interval distance
       */
      void contract(IntervalVector& a, IntervalVector& b, Interval& d);

      /**
       * \brief \f$\mathcal{C}_{\textrm{dist}}\big([\mathbf{x}]\big)\f$
       *
       * \param ax the first component of the first 2d vector
       * \param ay the second component of the first 2d vector
       * \param bx the first component of the second 2d vector
       * \param by the second component of the second 2d vector
       * \param d the interval distance
       */
      void contract(Interval& ax, Interval& ay, Interval& bx, Interval& by, Interval& d);
  };
}

#endif