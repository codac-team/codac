/** 
 *  SepFunction class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_SepFunction.h"
#include "ibex_Function.h"

using namespace std;

namespace codac
{
  SepFunction::SepFunction(Function& f)
    : SepFunction(f, Interval(0.))
  {

  }

  SepFunction::SepFunction(Function& f, const Interval& y)
    : ibex::SepFwdBwd(f, y)
  {

  }

  SepFunction::SepFunction(Function& f, const IntervalVector& y)
    : ibex::SepFwdBwd(f, y)
  {
    
  }

  void SepFunction::separate(IntervalVector& x_in, IntervalVector& x_out)
  {
    ibex::SepFwdBwd::separate(x_in, x_out);
  }
}