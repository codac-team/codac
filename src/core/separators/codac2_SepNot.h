/** 
 *  \file
 *  SepNot class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEPNOT__
#define __CODAC2_SEPNOT__

#include <map>
#include "codac2_Sep.h"

namespace codac2
{
  class SepNot : public Sep
  {
    public:

      SepNot(const Sep& s)
        : _s(s)
      { }

      BoxPair separate(const IntervalVector& x) const
      {
        auto x_ = _s.separate(x);
        return { x_.out , x_.in };
      }

    protected:

      const Sep& _s;
  };
}

#endif