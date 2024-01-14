/** 
 *  OctaSym class
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
  Interval sign(int a)
  {
    return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
  }

  int isign(int a)
  {
    return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
  }

  OctaSym::OctaSym(const vector<int>& s) : vector<int>(s)
  {
    for(const auto& i : s)
    {
      assert((size_t)abs(i) > 0 && (size_t)abs(i) <= size());
    }
  }

  OctaSym::OctaSym(initializer_list<int> s)
   : OctaSym(vector<int>(s))
  { }

  IntervalVector OctaSym::operator()(const IntervalVector& x) const
  {
    assert((size_t)x.size() == size());
    IntervalVector xs(size());
    for(size_t i = 0 ; i < size() ; i++)
      xs[i] = sign((*this)[i])*x[abs((*this)[i])-1];
    return xs;
  }

  OctaSym OctaSym::invert() const
  {
    OctaSym inv(*this);
    for(size_t i = 0 ; i < size() ; i++)
      inv[abs((int)(*this)[i])-1] = (abs((int)i)+1)*((*this)[i] >= 0 ? 1. : -1.);
    return inv;
  }

  CtcAction OctaSym::operator()(Ctc& ctc) const
  {
    return CtcAction(ctc, *this);
  }

  OctaSym operator*(const OctaSym& s1, const OctaSym& s2)
  {
    OctaSym s3(s1);
    for(size_t i = 0 ; i < s3.size() ; i++)
      s3[i] = isign(s2[i])*s1[abs((int)s2[i])-1];
    return s3;
  }
  
  ostream& operator<<(ostream& str, const OctaSym& s)
  {
    str << "(";
    for(size_t i = 0 ; i < s.size() ; i++)
      str << (i != 0 ? " " : "") << s[i];
    str << ")" << flush;
    return str;
  }
}