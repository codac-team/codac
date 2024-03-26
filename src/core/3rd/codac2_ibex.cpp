/** 
 *  \file
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, (Simon Rohou)
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_ibex.h"

using namespace std;

namespace codac2
{
  ibex::Interval& to_ibex(Interval& x)
  {
    return dynamic_cast<ibex::Interval&>(x);
  }
  
  const ibex::Interval& to_ibex(const Interval& x)
  {
    return dynamic_cast<const ibex::Interval&>(x);
  }

  Interval to_codac(const ibex::Interval& x)
  {
    return Interval(x);
  }

} // namespace codac