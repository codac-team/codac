/* ============================================================================
 *  tubex-lib - CtcHC4 class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef CtcHC4_HEADER
#define CtcHC4_HEADER

#include "tubex_Ctc.h"
#include "ibex_CtcHC4.h"
#include "tubex_TubeSlice.h"

namespace tubex
{
  /**
   * \brief CtcHC4 class.
   */
  class CtcHC4 : Ctc
  {
    public:

      CtcHC4();
      bool contract(ibex::CtcHC4& hc4, TubeVector& x);
      bool contract(ibex::CtcHC4& hc4, TubeSlice& x);

    protected:

  };
}

#endif