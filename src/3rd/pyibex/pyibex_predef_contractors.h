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

#ifndef __PYIBEX_CTCPREDEFINED_H__
#define __PYIBEX_CTCPREDEFINED_H__

#include "pyibex_CtcPolar.h"

namespace pyibex
{
  namespace ctc
  {
    extern pyibex::CtcPolar polar; // polar constraint (px,py,rho,phi)
  }
}

#endif