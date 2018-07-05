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

      CtcPicard(float delta = 1.1, bool preserve_sampling = false);
      bool contract_fwd(const tubex::Fnc& f, TubeVector& x) const;
      bool contract_bwd(const tubex::Fnc& f, TubeVector& x) const;
      bool contract(const tubex::Fnc& f, TubeVector& x, bool fwd = true) const;
      bool contract_fwd(const tubex::Fnc& f, TubeSlice& x) const;
      bool contract_bwd(const tubex::Fnc& f, TubeSlice& x) const;
      int picardIterations() const;

    protected:

      bool contract(const tubex::Fnc& f,
                    const ibex::Interval& t,
                    ibex::IntervalVector& x,
                    double t0,
                    const ibex::IntervalVector& x0) const;

      const ibex::IntervalVector eval(int order,
                                      const tubex::Fnc& f,
                                      const ibex::Interval& t,
                                      const ibex::IntervalVector& x,
                                      double t0,
                                      const ibex::IntervalVector& x0) const;

      float m_delta;
      bool m_preserve_sampling = false;
      mutable int m_picard_iterations = 0;
  };
}

#endif