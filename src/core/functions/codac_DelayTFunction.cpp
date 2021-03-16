/** 
 *  DelayTFunction class
 * ----------------------------------------------------------------------------
 *  \date       2017
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_DelayTFunction.h"
#include "codac_TubeVector.h"

using namespace std;
using namespace ibex;

namespace codac
{
  DelayTFunction::DelayTFunction(double delay) : TFnc(1, 1, true), m_delay(delay)
  {

  }
  
  DelayTFunction::~DelayTFunction()
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