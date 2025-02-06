/** 
 *  \file codac2_math.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <numbers>
#include <cmath>

namespace codac2
{
  constexpr double PI = std::numbers::pi; // Need C++20

  inline int sign(double x)
  {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
  }

  inline int integer(double x)
  {
    return (int)x;
  }
  
  inline double pow(double v, double p)
  {
    return v < 0 ? -std::pow(-v,p) : std::pow(v,p);
  }
  
  inline double root(double v, double p)
  {
    return pow(v, 1./p);
  }
}