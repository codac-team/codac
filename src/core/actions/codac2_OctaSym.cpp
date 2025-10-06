/** 
 *  OctaSym class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_assert.h"
#include "codac2_OctaSym.h"
#include "codac2_CtcAction.h"
#include "codac2_SepAction.h"

using namespace std;
using namespace codac2;

OctaSym::OctaSym(const vector<int>& s)
  : std::vector<int>(s)
{
  for([[maybe_unused]] const auto& i : s)
  {
    assert_release(std::abs(i) > 0 && std::abs(i) <= (int)size());
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
    a[i] = sign(s[i])*(*this)[std::abs((int)s[i])-1];
  return a;
}

Matrix OctaSym::permutation_matrix() const
{ 
  Matrix m = Matrix::zero(size(),size());
  for(size_t i = 0 ; i < size() ; i++)
    m(i,std::abs((*this)[i])-1) = sign((*this)[i]);
  return m;
}