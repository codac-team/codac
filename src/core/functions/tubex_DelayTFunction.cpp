/** 
 *  DelayTFunction class
 * ----------------------------------------------------------------------------
 *  \date       2017
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_DelayTFunction.h"
#include "tubex_TubeVector.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DelayTFunction::DelayTFunction(double delay) : TFnc(1, 1, true), m_delay(delay)
  {

  }

  const Interval DelayTFunction::eval(const Interval& t, const TubeVector& x) const
  {
    assert(x.tdomain().is_superset(t));
    return eval_vector(t, x)[0];
  }

  const IntervalVector DelayTFunction::eval_vector(const Interval& t, const TubeVector& x) const
  {
    assert(x.tdomain().is_superset(t));
    
    if((t + m_delay).is_subset(x.tdomain()))
      return x(t + m_delay);

    else
      return IntervalVector(x.size(), Interval::ALL_REALS);
  }
}