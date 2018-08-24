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

#include "ibex_Ctc.h"
#include "tubex_Beacon.h"

namespace tubex
{
  /**
   * \brief CtcConstellation class.
   *
   */
  class CtcConstellation : public ibex::Ctc
  {
    public:

      CtcConstellation(const std::vector<Beacon>& map);
      ~CtcConstellation();
      void contract(ibex::IntervalVector &beacon_box);

    protected:

      const std::vector<Beacon> m_map;
  };
}

#endif