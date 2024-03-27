/** 
 *  \file
 *  BoxCtc class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_BOXCTC__
#define __CODAC2_BOXCTC__

#include <map>
#include "codac2_IntervalVector.h"

namespace codac2
{
  class BoxCtc
  {
    public:

      virtual void contract(IntervalVector& x) const = 0;
  };
}

#endif