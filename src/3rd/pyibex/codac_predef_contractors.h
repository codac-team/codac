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

#ifndef __PYIBEX_PREDEF_CTC_H__
#define __PYIBEX_PREDEF_CTC_H__

#include "codac_CtcPolar.h"

namespace codac
{
  namespace ctc
  {
    extern CtcPolar polar; // polar constraint (px,py,rho,phi)
  }
}

#endif