/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <iostream>

inline void deprecated_xor()
{
  std::cout
    << "Operator '^' is intentionally disabled in Codac. Use pow(x,y), sqr(x), or '**' instead."
    << std::endl;
}