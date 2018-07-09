/* ============================================================================
 *  tubex-lib - Function class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include "tubex_DelayFunction.h"
#include "tubex_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DelayFunction::DelayFunction(double delay) : Fnc(1, 1), m_delay(delay)
  {

  }

  const IntervalVector DelayFunction::eval(const Interval& t, const TubeVector& x) const
  {
    if((t - m_delay).is_subset(x.domain()))
      return x[t - m_delay];

    else
      return IntervalVector(x.dim(), Interval::ALL_REALS);
  }
}