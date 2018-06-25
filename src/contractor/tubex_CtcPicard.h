/* ============================================================================
 *  tubex-lib - CtcPicard class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef CtcPicard_HEADER
#define CtcPicard_HEADER

#include "tubex_Ctc.h"
#include "tubex_TubeVector.h"
#include "tubex_TubeSlice.h"

namespace tubex
{
  /**
   * \brief CtcPicard class.
   */
  class CtcPicard : Ctc
  {
    public:

      CtcPicard(float delta = 1.1);
      bool contract(const ibex::Function& f, TubeVector& x, bool preserve_sampling = false);
      bool contract_fwd(const ibex::Function& f, TubeSlice& x);
      bool contract_bwd(const ibex::Function& f, TubeSlice& x);

    protected:

      bool contract(const ibex::Function& f,
                    ibex::IntervalVector& x, const ibex::IntervalVector& x0,
                    const ibex::Interval& h);

      float m_delta;
  };
}

#endif