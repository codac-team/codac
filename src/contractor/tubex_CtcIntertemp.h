/* ============================================================================
 *  tubex-lib - CtcIntertemp class
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
   * \brief CtcIntertemp class.
   */
  class CtcIntertemp : Ctc
  {
    public:

      CtcIntertemp(ibex::Interval *t1, ibex::Interval *t2, Tube *y, const Tube *w);
      bool contract();

    protected:

      ibex::Interval *m_t1, *m_t2;
      Tube *m_y;
      const Tube *m_w;
  };
}

#endif