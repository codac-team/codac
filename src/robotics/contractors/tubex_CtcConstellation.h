/* ============================================================================
 *  tubex-lib - CtcConstellation class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : -
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCCONSTELLATION_H__
#define __TUBEX_CTCCONSTELLATION_H__

#include "tubex_Ctc.h"
#include "tubex_Beacon.h"

namespace tubex
{
  /**
   * \brief CtcConstellation class.
   *
   */
  class CtcConstellation // todo: derive from ibex::Ctc
  {
    public:

      CtcConstellation();

      bool contract(ibex::IntervalVector &beacon_box, const std::vector<Beacon>& map) const;

    protected:

  };
}

#endif