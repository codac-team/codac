/** 
 *  \file
 *  Sep class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEP__
#define __CODAC2_SEP__

#include <map>
#include "codac2_IntervalVector.h"

namespace codac2
{
  struct BoxPair
  {
    IntervalVector in;
    IntervalVector out;
  };

  class Sep
  {
    public:

      virtual BoxPair separate(const IntervalVector& x) const = 0;
  };
}

#endif