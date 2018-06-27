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

      CtcEval();
      bool contract(double t, ibex::Interval& z, Tube& y);
      bool contract(double t, ibex::Interval& z, Tube& y, const Tube& w, bool propagate = true);
      bool contract(ibex::Interval& t, ibex::Interval& z, Tube& y, const Tube& w, bool propagate = true);

      bool tContracted();
      bool zContracted();
      bool yContracted();
      bool wContracted();

      // Returns a boolean: 'true' if a bisection of [t] should be considered to improve the contraction
      bool tBisectionRequired();

    protected:

      bool contract_timeUncertainty(ibex::Interval& t, ibex::Interval& z, Tube& y, const Tube& w, bool propagate);
      bool contract_knownTime(double t, ibex::Interval& z, Tube& y);
      bool contract_knownTime(double t, ibex::Interval& z, Tube& y, const Tube& w, bool propagate);
      void computeIndex(const ibex::Interval& t, const ibex::Interval& z, const Tube& y, int& index_lb, int& index_ub);

      bool m_y_contracted = false, m_t_contracted = false, m_z_contracted = false;
      bool m_bisection_required = false;
  };
}

#endif