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
      bool contract(ibex::Interval& t, ibex::Interval& z, Tube& y, const Tube& w, bool propagation = true);

      bool tContracted();
      bool zContracted();
      bool yContracted();
      bool wContracted();

    protected:

      void computeIndex(const ibex::Interval& t, const ibex::Interval& z, const Tube& y, int& index_lb, int& index_ub);

      ibex::Interval *m_t, *m_z;
      Tube *m_y;
      const Tube *m_w;
      bool m_y_contracted, m_t_contracted, m_z_contracted;
      bool m_bisection_required;
  };
}

#endif