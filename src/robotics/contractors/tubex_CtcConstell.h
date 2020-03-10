/** 
 *  CtcConstell class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCCONSTELL_H__
#define __TUBEX_CTCCONSTELL_H__

#include "ibex_Ctc.h"
#include "tubex_Beacon.h"

namespace tubex
{
  /**
   * \brief CtcConstell class.
   *
   */
  class CtcConstell : public ibex::Ctc
  {
    public:

      CtcConstell(const std::vector<Beacon>& map);
      ~CtcConstell();
      void contract(ibex::IntervalVector &beacon_box);

    protected:

      const std::vector<Beacon> m_map;
  };
}

#endif