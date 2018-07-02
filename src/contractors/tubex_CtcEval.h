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

#ifndef CtcEval_HEADER
#define CtcEval_HEADER

#include "tubex_Ctc.h"
#include "tubex_Tube.h"

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

      CtcEval(bool enable_propagation = true, bool enable_slicing = true);

      bool contract(double t, ibex::Interval& z, Tube& y, Tube& w) const;
      bool contract(ibex::Interval& t, ibex::Interval& z, Tube& y, Tube& w) const;

    protected:

      void computeIndex(const ibex::Interval& t, const ibex::Interval& z, const Tube& y, int& index_lb, int& index_ub) const;

      bool m_propagation_enabled = true;
      bool m_slicing_enabled = true;

      mutable bool m_y_contracted = false, m_t_contracted = false, m_z_contracted = false;
      mutable bool m_bisection_required = false;
  };
}

#endif