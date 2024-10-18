/** 
 *  \file codac2_geometry.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_BoolInterval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  BoolInterval orientation(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3);
}