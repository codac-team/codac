/* ============================================================================
 *  tubex-lib - CtcEval class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCEVAL_H__
#define __TUBEX_CTCEVAL_H__

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcEval class.
   *
   * Constraint: z=y(t)
   */
  class CtcEval : Ctc
  {
    public:

      CtcEval(bool preserve_slicing = false, bool enable_propagation = true);

      // Tube
      bool contract(double t, ibex::Interval& z, Tube& y, Tube& w) const;
      bool contract(ibex::Interval& t, ibex::Interval& z, Tube& y, Tube& w) const;

      // TubeVector
      bool contract(double t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const;
      bool contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const;

    protected:

      bool m_propagation_enabled = true;
  };
}

#endif