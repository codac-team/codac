/* ============================================================================
 *  tubex-lib - CtcDiff class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef CtcDiff_HEADER
#define CtcDiff_HEADER

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcDiff class.
   */
  class CtcDiff : Ctc
  {
    public:

      CtcDiff(Tube *x, const Tube *v);
      bool contract();

    protected:

      Tube *m_x;
      const Tube *m_v;
  };
}

#endif