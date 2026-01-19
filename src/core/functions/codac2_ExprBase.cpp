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

std::atomic<Index> ExprID::_id_counter{0};

ExprID::ExprID()
  : _id(ExprID::_id_counter.fetch_add(1, std::memory_order_relaxed))
{
  ExprID::_id_counter ++;
}

Index ExprID::id() const
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