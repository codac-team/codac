/* ============================================================================
 *  tubex-lib - CtcCompo class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef CtcCompo_HEADER
#define CtcCompo_HEADER

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \brief CtcCompo class.
   */
  class CtcCompo : Ctc
  {
    public:

      CtcCompo();
      bool contract(Tube& x, Tube& y, Tube& z);

    protected:
      
  };
}

#endif