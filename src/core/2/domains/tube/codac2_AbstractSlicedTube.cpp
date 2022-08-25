/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_AbstractSlicedTube.h"

using namespace std;

namespace codac2
{
  AbstractSlicedTube::AbstractSlicedTube(const shared_ptr<TDomain>& tdomain) :
    _tdomain(tdomain)
  {

  }

  const shared_ptr<TDomain>& AbstractSlicedTube::tdomain() const
  {
    return _tdomain;
  }

  Interval AbstractSlicedTube::t0_tf() const
  {
    return _tdomain->t0_tf();
  }
} // namespace codac