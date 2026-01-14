/** 
 *  codac2_trunc.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_trunc.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  double trunc(double x)
  {
    if(x == oo)
      return prev_float(oo);
    else if(x == -oo)
      return next_float(-oo);
    return x;
  }

  Interval trunc(const Interval& x)
  {
    return { trunc(x.lb()), trunc(x.ub()) };
  }

  double untrunc(double x)
  {
    if(x == prev_float(oo))
      return oo;
    else if(x == next_float(-oo))
      return -oo;
    return x;
  }

  Interval untrunc(const Interval& x)
  {
    return { untrunc(x.lb()), untrunc(x.ub()) };
  }

  double graphic_trunc(double x)
  {
    double max = 9e15;
    if(x < -max)
      return -max;
    else if(x > max)
      return max;
    return x;
  }
}