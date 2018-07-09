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

#ifndef __TUBEX_CTCPICARD_H__
#define __TUBEX_CTCPICARD_H__

#include "tubex_Ctc.h"
#include "tubex_Fnc.h"
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
      void contract_fwd(const tubex::Fnc& f, TubeVector& x) const;
      void contract_bwd(const tubex::Fnc& f, TubeVector& x) const;
      bool contract(const tubex::Fnc& f, TubeVector& x, bool fwd = true) const;

      void contract_fwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const;
      void contract_bwd(const tubex::Fnc& f, const TubeVector& tube, TubeSlice& slice) const;
      int picardIterations() const;
      void guessSliceEnvelope(const Fnc& f,
                              const TubeVector& tube,
                              TubeSlice& slice,
                              bool fwd) const;

    protected:

      float m_delta;
      mutable int m_picard_iterations = 0;
  };
}

#endif