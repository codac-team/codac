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
    assert((size_t)std::abs(i) > 0 && (size_t)std::abs(i) <= size());
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

IntervalVector OctaSym::operator()(const IntervalVector& x) const
{
  assert_release((size_t)x.size() == size());
  IntervalVector x_(size());
  for(size_t i = 0 ; i < size() ; i++)
    x_[i] = sign((*this)[i])*x[std::abs((*this)[i])-1];
  return x_;
}

ostream& operator<<(ostream& str, const OctaSym& s)
{
  str << "(";
  for(size_t i = 0 ; i < s.size() ; i++)
    str << (i != 0 ? " " : "") << s[i];
  str << ")" << flush;
  return str;
}

OctaSym operator*(const OctaSym& s1, const OctaSym& s2)
{
  assert_release(s1.size() == s2.size());
  OctaSym s3(s1);
  for(size_t i = 0 ; i < s3.size() ; i++)
    s3[i] = isign(s2[i])*s1[std::abs((int)s2[i])-1];
  return s3;
}