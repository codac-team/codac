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

#ifndef __TUBEX_CTCPREDEFINED_H__
#define __TUBEX_CTCPREDEFINED_H__

#include "tubex_CtcDeriv.h"
#include "tubex_CtcDistance.h"
#include "tubex_CtcEval.h"

namespace tubex
{
  namespace ctc
  {
    extern CtcDeriv deriv; // derivative constraint (x,v)
    extern CtcEval eval; // evaluation constraint (t,z,y,w)
    extern CtcDistance dist; // distance constraint (a,b,d)
  }
}

#endif