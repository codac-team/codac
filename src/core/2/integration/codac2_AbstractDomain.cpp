/** 
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_AbstractDomain.h"

using namespace std;
using namespace codac;

namespace codac2
{
  AbstractDomain::AbstractDomain()
  {

  }
  
  IntervalVector AbstractDomain::box() const
  {
    return IntervalVector(0); // todo
  }
}