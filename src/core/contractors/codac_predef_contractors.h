/** 
 *  \file
 *  Predefined contractors (shortcuts for ease of use)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_PREDEF_CTC_H__
#define __CODAC_PREDEF_CTC_H__

#include "codac_CtcDeriv.h"
#include "codac_CtcDist.h"
#include "codac_CtcEval.h"
#include "codac_CtcDelay.h"
#include "codac_CtcChain.h"
#include "codac_CtcPolar.h"

namespace codac
{
  namespace ctc
  {
    extern CtcDelay delay; // delay constraint (a,x,y)
    extern CtcDeriv deriv; // derivative constraint (x,v)
    extern CtcDist dist; // distance constraint (a,b,d)
    extern CtcEval eval; // evaluation constraint (t,z,y,w)
    extern CtcChain chain; // double integration constraint (x,xdot,xddot)
    extern CtcPolar polar; // polar constraint (px,py,rho,phi)
  }
}

#endif