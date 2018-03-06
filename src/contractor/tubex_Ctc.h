/* ============================================================================
 *  tubex-lib - Ctc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef Ctc_HEADER
#define Ctc_HEADER

#include "tubex_Tube.h"

namespace tubex
{
  /**
   * \brief Ctc class.
   */
  class Ctc
  {
    public:

      Ctc(){};
      virtual bool contract();
  };
}

#endif