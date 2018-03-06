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
   */
  class CtcEval : Ctc
  {
    public:

      CtcEval(ibex::Interval *t, ibex::Interval *z, Tube *y, const Tube *w);
      bool contract();

    protected:

      ibex::Interval *m_t, *m_z;
      Tube *m_y;
      const Tube *m_w;
  };
}

#endif