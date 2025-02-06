/** 
 *  codac2_CtcEmpty.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcEmpty.h"

using namespace std;
using namespace codac2;

CtcEmpty::CtcEmpty(Index n)
  : Ctc<CtcEmpty,IntervalVector>(n)
{ }

void CtcEmpty::contract(IntervalVector& x) const
{
  x.set_empty();
}