/** 
 *  \file codac2_math.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <gaol/gaol_interval.h>
#include <numbers>
#include <cmath>

namespace codac2
{
  const double oo = []() {

    // (from IBEX lib, main author: Gilles Chabert)
    // We use Gaol not in PRESERVE_ROUNDING mode, thus
    // assuming the rounding mode is always set upward.
    // Calling this function in the initialization of
    // the 'oo' constant should be enough as this constant
    // is initialized before the first Codac function call occurs.
    gaol::round_upward();

    return std::numeric_limits<double>::infinity();
  }();

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

  template<typename T>
  inline bool is_nan(const T& x)
  {
    if constexpr(std::is_arithmetic_v<T>)
      return std::isnan(x);
    else
      return x.is_nan();
  }
}