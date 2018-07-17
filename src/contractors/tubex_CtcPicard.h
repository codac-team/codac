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
      bool contract(const tubex::Fnc& f,
                    TubeVector& x,
                    TPropagation t_propa = FORWARD | BACKWARD) const;
      int picard_iterations() const;

    protected:

      void contract(const tubex::Fnc& f,
                    const TubeVector& tube,
                    TubeSlice& slice,
                    TPropagation t_propa) const;
      void guess_slice_envelope(const Fnc& f,
                                const TubeVector& tube,
                                TubeSlice& slice,
                                TPropagation t_propa) const;

      float m_delta;
      mutable int m_picard_iterations = 0;
  };
}

#endif