/* ============================================================================
 *  tubex-lib - CtcDelay class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCDELAY_H__
#define __TUBEX_CTCDELAY_H__

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcDelay class.
   *
   * Constraint: y=x(t-a)
   */
  class CtcDelay : Ctc
  {
    public:

      CtcDelay(bool preserve_slicing = false);

      // Tube
      bool contract(const ibex::Interval& a, const TubeVector& x, TubeVector& y) const;

    protected:

  };
}

#endif