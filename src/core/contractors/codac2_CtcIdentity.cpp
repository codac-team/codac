/** 
 *  codac2_CtcIdentity.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcIdentity.h"

using namespace std;
using namespace codac2;

CtcIdentity::CtcIdentity(size_t n)
  : Ctc<CtcIdentity,IntervalVector>(n)
{ }

void CtcIdentity::contract(IntervalVector& x) const
{ }