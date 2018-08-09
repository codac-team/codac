/* ============================================================================
 *  tubex-lib - PeriodicFunction class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include "tubex_PeriodicFunction.h"
#include "tubex_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  PeriodicFunction::PeriodicFunction() : Fnc(1, 1) // todo: change input/output dim
  {

  }

  const Interval PeriodicFunction::eval(const Interval& t, const TubeVector& x) const
  {
    // todo: optimize this?
    return eval_vector(t, x)[0];
  }

  const IntervalVector PeriodicFunction::eval_vector(const Interval& t, const TubeVector& x) const
  {
    double period = x.domain().diam();
    Interval a = Interval(t.lb(), x.domain().lb()) & t;
    Interval b = Interval(x.domain().lb(), t.ub()) & t;

    if(!a.is_empty())
    {
      double a1 = fmod(a.lb() + 4.*period, period);
      double a2 = fmod(a.ub() + 4.*period, period);
      if(a2 == 0.) a2 = period;
      a = Interval(min(a1,a2), max(a1,a2));
    }

    IntervalVector result(x.size(), Interval::EMPTY_SET);
    if(!a.is_empty()) result |= x(a);
    if(!b.is_empty()) result |= x(b);
    return result;
  }
}