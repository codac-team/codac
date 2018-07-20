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

#ifndef __TUBEX_CTC_H__
#define __TUBEX_CTC_H__

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  enum TPropagation
  {
    FORWARD = 0x01,
    BACKWARD = 0x02
  };
  
  inline TPropagation operator|(TPropagation a, TPropagation b)
  { return static_cast<TPropagation>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \brief Ctc class.
   */
  class Ctc
  {
    public:

      Ctc(bool preserve_slicing = false);
      virtual bool contract() {};

    protected:

      bool m_preserve_slicing;
  };
}

#endif