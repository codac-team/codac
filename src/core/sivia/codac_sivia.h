/** 
 *  \file
 *  SIVIA
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SIVIA_H__
#define __CODAC_SIVIA_H__

#include <ibex_Sep.h>
#include "codac_Ctc.h"
#include "codac_VIBesFigPaving.h"
#include "codac_IntervalVector.h"

namespace codac
{
  void SIVIA(const IntervalVector& x, Ctc* ctc, float precision,
    const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);
  
  void SIVIA(const IntervalVector& x, ibex::Sep* sep, float precision,
    const SetColorMap& color_map = DEFAULT_SET_COLOR_MAP);
}

#endif