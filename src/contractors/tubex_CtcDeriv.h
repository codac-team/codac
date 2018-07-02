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
#include "tubex_TubeVector.h"
#include "tubex_ConvexPolygon.h"
#include "tubex_CtcEval.h"

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
      bool contract(TubeVector& x, const TubeVector& v);
      bool contractFwd(TubeVector& x, const TubeVector& v);
      bool contractBwd(TubeVector& x, const TubeVector& v);
      bool contract(TubeSlice& x, const TubeSlice& v);

    protected:

      bool contractGates(TubeSlice& x, const TubeSlice& v);
      bool contractCodomain(int i, TubeSlice& x, const TubeSlice& v);

      friend class CtcEval;
  };
}

#endif