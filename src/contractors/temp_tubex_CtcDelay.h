/* ============================================================================
 *  tubex-lib - CtcDelay class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef CtcDelay_HEADER
#define CtcDelay_HEADER

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcDelay class.
   *
   * Constraint: x(t)=y(t+a)
   */
  class CtcDelay : Ctc
  {
    public:

      CtcDelay();
      bool contract(ibex::Interval& a, Tube& x, Tube& y);

    protected:

  };
}

#endif