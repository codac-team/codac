/** 
 *  codac2_ExprBase.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_ExprBase.h"

using namespace std;
using namespace codac2;

size_t ExprID::_id_counter = 0;

ExprID::ExprID()
  : _id(ExprID::_id_counter)
{
  ExprID::_id_counter ++;
}

size_t ExprID::id() const
{
  return _id;
}

bool ExprID::operator==(const ExprID& i) const
{
  return _id == i._id;
}

bool ExprID::operator<(const ExprID& i) const
{
  return _id < i._id;
}


ExprBase::ExprBase()
  : _unique_id(/* creating new id from object address */)
{ }

const ExprID& ExprBase::unique_id() const
{
  return _unique_id;
}

bool ExprBase::operator==(const ExprBase& e) const
{
  return _unique_id == e._unique_id;
}