/** 
 *  \file
 *  CtcDist class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCDIST_H__
#define __TUBEX_CTCDIST_H__

#include "tubex_CtcFunction.h"

namespace tubex
{
  /**
   * \class CtcDist
   * \brief Distance constraint between two 2d vectors.
   */
  class CtcDist : public CtcFunction
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcDist();

      /**
       * \brief \f$\mathcal{C}_{\textrm{dist}}\big([\mathbf{x}]\big)\f$
       *
       * \param a the 2d box of the first 2d vector
       * \param b the 2d box of the second 2d vector
       * \param d the interval distance
       */
      void contract(ibex::IntervalVector& a, ibex::IntervalVector& b, ibex::Interval& d);
  };
}

#endif