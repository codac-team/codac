/* ============================================================================
 *  tubex-lib - CtcOut class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef CtcOut_HEADER
#define CtcOut_HEADER

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcOut class.
   */
  class CtcOut : Ctc
  {
    public:

      CtcOut();
      bool contract(const ibex::Interval& t, const ibex::Interval& y);

    protected:

  };
}

#endif