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
  class CtcEval : public Ctc
  {
    public:

      CtcEval();
      void enable_temporal_propagation(bool enable_propagation);

      // Tube
      void contract(double t, ibex::Interval& z, Tube& y, Tube& w) const;
      void contract(ibex::Interval& t, ibex::Interval& z, Tube& y, Tube& w) const;
      void contract(const ibex::Interval& t, const ibex::Interval& z, Tube& y, const Tube& w) const;

      // TubeVector
      void contract(double t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const;
      void contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const;
      void contract(const ibex::Interval& t, const ibex::IntervalVector& z, TubeVector& y, const TubeVector& w) const;

    protected:

      bool m_propagation_enabled = true;
  };
}

#endif