/** 
 *  Action class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Action.h"
#include "codac2_CtcAction.h"

using namespace std;
using namespace codac;

namespace codac2
{
  Action::Action(const OctaSym& s) : _s(s), __s(s)
  {

  }

  Action::Action(const OctaSym& s, const OctaSym& _s) : _s(s), __s(_s)
  {

  }

  IntervalVector Action::operator()(const IntervalVector& x) const
  {
    assert((size_t)x.size() == _s.size());
    IntervalVector xs(_s.size());
    for(size_t i = 0 ; i < _s.size() ; i++)
      xs[i] = sign(_s[i])*x[abs(_s[i])-1];
    return xs;
  }

  CtcAction Action::operator()(Ctc& ctc) const
  {
    return CtcAction(ctc, _s, __s);
  }
}