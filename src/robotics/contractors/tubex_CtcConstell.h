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
#include <vector>
#include <list>

namespace tubex
{
  /**
   * \brief CtcConstell class.
   *
   */
  class CtcConstell : public ibex::Ctc
  {
    public:

      CtcConstell(const std::vector<ibex::IntervalVector>& map);
      CtcConstell(const std::list<ibex::IntervalVector>& map);
      ~CtcConstell();
      void contract(ibex::IntervalVector &beacon_box);

    protected:

      std::vector<ibex::IntervalVector> m_map;
  };
}

#endif