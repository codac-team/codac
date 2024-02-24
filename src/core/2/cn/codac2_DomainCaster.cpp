/** 
 *  DomainCaster class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_DomainCaster.h"

using namespace std;
using namespace codac2;


DomainCaster<Paving&>::DomainCaster(Paving& x)
  : _x(x)
{ }

DomainCaster<Paving&>::operator Paving&()
{
  return _x;
}

std::shared_ptr<PropagationSpanBase> DomainCaster<Paving&>::update_and_propag()
{
  double new_volume = _x.volume();
  size_t new_nb_leaves = _x.nb_leaves();

  if(new_volume != _prev_volume || new_nb_leaves != _prev_nb_leaves)
  {
    _prev_volume = new_volume;
    _prev_nb_leaves = new_nb_leaves;
    return std::make_shared<PropagationSpan<Paving>>(&_x);
  }

  else
    return nullptr;
}

bool DomainCaster<Paving&>::operator==(const DomainCaster<Paving&>& x) const
{
  return &_x == &x._x;
}

std::string DomainCaster<Paving&>::to_string() const
{
  return _x.name();
}