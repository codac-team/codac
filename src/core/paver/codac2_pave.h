/** 
 *  \file codac2_pave.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Paving.h"
#include "codac2_Ctc.h"
#include "codac2_Sep.h"

namespace codac2
{
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps);
  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps);
}