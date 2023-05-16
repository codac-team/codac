/** 
 *  CtcAction class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcAction.h"

using namespace std;
using namespace codac;

namespace codac2
{
  CtcAction::CtcAction(Ctc& ctc, const Action& s, const Action& _s)
   : Ctc(2), _ctc(ctc), _s(s), __s(_s)
  {

  }

  void CtcAction::contract(IntervalVector& x)
  {
    IntervalVector _x(_s(x));
    _ctc.contract(_x);
    x &= __s(_x);
  }
}