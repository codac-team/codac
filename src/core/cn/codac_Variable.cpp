/** 
 *  Variable class
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Variable.h"

using namespace std;

namespace codac
{
  Variable::Variable(const Interval& i)
    : Domain(i)
  {
    
  }

  Variable::Variable(const IntervalVector& iv)
    : Domain(iv)
  {
    
  }

  Variable::Variable(const Tube& t)
    : Domain(t)
  {
    
  }

  Variable::Variable(const TubeVector& tv)
    : Domain(tv)
  {
    
  }
}