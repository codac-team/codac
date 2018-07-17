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

#ifndef __TUBEX_CTCDERIV_H__
#define __TUBEX_CTCDERIV_H__

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

      CtcDeriv(bool preserve_slicing = false);
      bool contract(TubeVector& x, const TubeVector& v, TPropagation t_propa = FORWARD | BACKWARD) const;
      bool contract(TubeSlice& x, const TubeSlice& v) const;

    protected:

      bool contract_gates(TubeSlice& x, const TubeSlice& v) const;
      bool contract_codomain(int i, TubeSlice& x, const TubeSlice& v) const;

      friend class CtcEval; // contract_gates used by CtcEval
  };
}

#endif