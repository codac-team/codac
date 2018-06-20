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
#include "tubex_ConvexPolygon.h"

// todo: contract v if x degenerated

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
      bool contract(const Tube& x, const Tube& v, ibex::Interval& t, ibex::IntervalVector& y);

      // Tube slice
      bool contract(TubeSlice& x, const TubeSlice& v);
      bool contract(const TubeSlice& x, const TubeSlice& v, ibex::Interval& t, ibex::IntervalVector& y);

      ConvexPolygon getPolygon(const TubeSlice& x, const TubeSlice& v);

    protected:

      bool contractGates(TubeSlice& x, const TubeSlice& v);
      bool contractEnvelope(const TubeSlice& x, const TubeSlice& v, ibex::Interval& t, ibex::IntervalVector& y, ConvexPolygon& p);
  };
}

#endif