/* ============================================================================
 *  tubex-lib - CtcDeriv class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef CtcDeriv_HEADER
#define CtcDeriv_HEADER

#include "tubex_Ctc.h"
#include "tubex_TubeSlice.h"

namespace tubex
{
  /**
   * \brief CtcDeriv class.
   *
   * Constraint: \dot{x}=v
   */
  class CtcDeriv : Ctc
  {
    public:

      CtcDeriv();

      // Tube
      bool contract(Tube& x, const Tube& v);
      bool contractFwd(Tube& x, const Tube& v);
      bool contractBwd(Tube& x, const Tube& v);
      bool contract(const Tube& x, const Tube& v, const ibex::Interval& t, ibex::Interval& y);

      // Tube slice
      bool contract(TubeSlice& x, const TubeSlice& v);
      bool contract(const TubeSlice& x, const TubeSlice& v, double t, ibex::Interval& y);
      bool contract(const TubeSlice& x, const TubeSlice& v, const ibex::Interval& t, ibex::Interval& y);

    protected:

      bool contractGates(TubeSlice& x, const TubeSlice& v);
      bool contractEnvelope(const TubeSlice& x, const TubeSlice& v, const ibex::Interval& t, ibex::Interval& y);
  };
}

#endif