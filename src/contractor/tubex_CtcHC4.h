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
#include "tubex_Tube.h"
#include "tubex_TubeSlice.h"
#include "ibex_CtcHC4.h"

namespace tubex
{
  /**
   * \brief CtcHC4 class.
   *
   * Constraint: \dot{x}=v
   */
  class CtcHC4 : Ctc
  {
    public:

      CtcHC4();

      // Tube
      bool contract(Tube& x1, ibex::CtcHC4& hc4);
      bool contract(Tube& x1, Tube& x2, ibex::CtcHC4& hc4);
      bool contract(Tube& x1, Tube& x2, Tube& x3, ibex::CtcHC4& hc4);
      bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, ibex::CtcHC4& hc4);

      // TubeSlice
      bool contract(TubeSlice& x1, ibex::CtcHC4& hc4);
      bool contract(TubeSlice& x1, TubeSlice& x2, ibex::CtcHC4& hc4);
      bool contract(TubeSlice& x1, TubeSlice& x2, TubeSlice& x3, ibex::CtcHC4& hc4);
      bool contract(TubeSlice& x1, TubeSlice& x2, TubeSlice& x3, TubeSlice& x4, ibex::CtcHC4& hc4);

    protected:

  };
}

#endif