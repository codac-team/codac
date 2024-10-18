/** 
 *  OctaSym class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_assert.h"
#include "codac2_OctaSym.h"
#include "codac2_CtcAction.h"
#include "codac2_SepAction.h"

using namespace std;
using namespace codac2;

Interval sign(int a)
{
  return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
}

int isign(int a)
{
  return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
}

OctaSym::OctaSym(const vector<int>& s)
  : std::vector<int>(s)
{
  for(const auto& i : s)
  {
    assert_release((size_t)std::abs(i) > 0 && (size_t)std::abs(i) <= size());
  }
}

OctaSym::OctaSym(initializer_list<int> s)
 : std::vector<int>(s)
{ }

OctaSym OctaSym::invert() const
{
  OctaSym inv(*this);
  for(size_t i = 0 ; i < size() ; i++)
    inv[std::abs((int)(*this)[i])-1] = (std::abs((int)i)+1)*((*this)[i] >= 0 ? 1. : -1.);
  return inv;
}

OctaSym OctaSym::operator*(const OctaSym& s) const
{
  assert_release(size() == s.size());
  OctaSym a(*this);
  for(size_t i = 0 ; i < a.size() ; i++)
    a[i] = isign(s[i])*(*this)[std::abs((int)s[i])-1];
  return a;
}

IntervalVector OctaSym::operator()(const IntervalVector& x) const
{
  assert_release((size_t)x.size() == size());
  IntervalVector x_(size());
  for(size_t i = 0 ; i < size() ; i++)
    x_[i] = sign((*this)[i])*x[std::abs((*this)[i])-1];
  return x_;
}