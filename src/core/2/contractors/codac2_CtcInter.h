/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCINTER_H__
#define __CODAC2_CTCINTER_H__

#include "codac2_Contractor.h"

namespace codac2
{
  template<int N>
  class CtcInter : public Contractor
  {
    public:

      CtcInter()
      {

      }

      void contract(Paving<N>& x, IntervalVector_<N>& y)
      {
        IntervalVector_<N> y_ = IntervalVector_<N>::empty_set();
        for(auto& li : x.leaves_list())
        {
          y_ |= li->_x & y;
          //li->_x &= y;
        }
        y = y_;
      }

      make_available_in_cn__templated(CtcInter<N>)

    protected:

  };
}

#endif