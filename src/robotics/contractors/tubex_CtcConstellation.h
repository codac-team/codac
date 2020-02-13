/** 
 *  CtcConstellation class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

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