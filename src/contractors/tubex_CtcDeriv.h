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

      // TubeVector
      bool contract(TubeVector& x, const TubeVector& v) const;
      bool contractFwd(TubeVector& x, const TubeVector& v) const;
      bool contractBwd(TubeVector& x, const TubeVector& v) const;

      // TubeSlice
      bool contract(TubeSlice& x, const TubeSlice& v) const;

    protected:

      bool contractGates(TubeSlice& x, const TubeSlice& v) const;
      bool contractCodomain(int i, TubeSlice& x, const TubeSlice& v) const;

      friend class CtcEval; // contractGates used by CtcEval
  };
}

#endif