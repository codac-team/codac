/** 
 *  \file
 *  Predefined contractors (shortcuts for ease of use)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_PREDEF_CTC_H__
#define __TUBEX_PREDEF_CTC_H__

#include "tubex_CtcDeriv.h"
#include "tubex_CtcDist.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcDelay.h"

namespace tubex
{
  namespace ctc
  {
    extern CtcDeriv deriv; // derivative constraint (x,v)
    extern CtcEval eval; // evaluation constraint (t,z,y,w)
    extern CtcDist dist; // distance constraint (a,b,d)
    extern CtcDelay delay; // delay constraint (a,x,y)
  }
}

#endif