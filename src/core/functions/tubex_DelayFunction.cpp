/* ============================================================================
 *  tubex-lib - DelayFunction class
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

  const Interval DelayFunction::eval(const Interval& t, const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(t, x)[0];
  }

  const IntervalVector DelayFunction::eval_vector(const Interval& t, const TubeVector& x) const
  {
    if((t - m_delay).is_subset(x.domain()))
      return x(t - m_delay);

    else
      return IntervalVector(x.size(), Interval::ALL_REALS);
  }
}