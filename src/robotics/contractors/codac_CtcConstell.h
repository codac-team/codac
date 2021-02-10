/** 
 *  CtcConstell class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCCONSTELL_H__
#define __CODAC_CTCCONSTELL_H__

#include <list>
#include <vector>
#include "codac_Ctc.h"
#include <codac_IntervalVector.h>

namespace codac
{
  /**
   * \brief CtcConstell class.
   *
   */
  class CtcConstell : public Ctc
  {
    public:

      CtcConstell(const std::vector<IntervalVector>& map);
      CtcConstell(const std::list<IntervalVector>& map);
      ~CtcConstell();
      void contract(IntervalVector &beacon_box);

    protected:

      std::vector<IntervalVector> m_map;
  };
}

#endif