/** 
 *  PropagationSpan class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_PropagationSpan.h"

using namespace std;
using namespace codac2;

//template<>
//bool PropagationSpan<Interval>::intersects(const std::shared_ptr<PropagationSpanBase>& view) const
//{std::cout << "ici" << std::endl;
//  return false;
//}


// ==================================================================================================
// Specialization PropagationSpan<IntervalVector>

PropagationSpan<IntervalVector>::PropagationSpan(const IntervalVector* x)
  : _x(x), _indices(x->size())
{
  std::iota(_indices.begin(), _indices.end(), 0);
}

PropagationSpan<IntervalVector>::PropagationSpan(const IntervalVector* x, const std::vector<size_t>& indices)
  : _x(x), _indices(indices)
{

}

bool PropagationSpan<IntervalVector>::is_empty() const
{
  return _indices.empty();
}

bool PropagationSpan<IntervalVector>::is_non_contractible() const
{
  return _x->volume() == 0.;
}

bool PropagationSpan<IntervalVector>::intersects(const std::shared_ptr<PropagationSpanBase>& view) const
{
  auto view_cast_iv = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  if(view_cast_iv)
  {
    if(view_cast_iv->_x == _x)
      return true;

    for(const auto& i : _indices)
      for(const auto& i_view : view_cast_iv->_indices)
      {
        if(&(*_x)[i] == &(*view_cast_iv->_x)[i_view])
          return true;
        if(i_view > i)
          break; // i is not in the ordered vector view_cast->_indices
      }
  }
  
  // todo: is this working?
  auto view_cast_i = std::dynamic_pointer_cast<PropagationSpan<Interval>>(view);
  if(view_cast_i)
  {
    for(size_t i = 0 ; i < _x->size() ; i++)
      if(&(*_x)[i] == view_cast_i->_x)
        return true;
  }

  return false;
}

void PropagationSpan<IntervalVector>::contract(const std::shared_ptr<PropagationSpanBase>& view)
{
  auto view_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  assert(view_cast && "fail to pointer cast PropagationSpanBase into PropagationSpan<IntervalVector>");
  assert(view_cast->_x == _x && "views are not related to the same domain");

  std::vector<size_t> indices;
  for(const auto& di : _indices)
    for(const auto& xdi : view_cast->_indices)
      if(di == xdi)
      {
        indices.push_back(di);
        break;
      }
  _indices = indices;
}

void PropagationSpan<IntervalVector>::merge(const std::shared_ptr<PropagationSpanBase>& view)
{
  auto view_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  assert(view_cast && "fail to pointer cast PropagationSpanBase into PropagationSpan<IntervalVector>");
  assert(view_cast->_x == _x && "views are not related to the same domain");

  std::vector<size_t> merged_indices;
  std::set_union(_indices.cbegin(), _indices.cend(),
                 view_cast->_indices.cbegin(), view_cast->_indices.cend(),
                 std::back_inserter(merged_indices));
  _indices = merged_indices;
}

bool PropagationSpan<IntervalVector>::is_view_from_const() const
{
  return false;
}

bool PropagationSpan<IntervalVector>::is_view_from_var() const
{
  return false;
}

std::string PropagationSpan<IntervalVector>::to_string() const
{
  string str = _x->name() + "[";
  for(const auto& i : _indices)
    str += std::to_string(i) + ",";
  str.pop_back();
  return str + "]";
}


// ==================================================================================================
// Specialization PropagationSpan<IntervalVectorComponent>

/*PropagationSpan<IntervalVectorComponent>::PropagationSpan(const IntervalVectorComponent& x)
  : _x(&x._x), _indices({x._i})
{

}

bool PropagationSpan<IntervalVectorComponent>::is_empty() const
{
  return _indices.empty();
}

bool PropagationSpan<IntervalVectorComponent>::is_non_contractible() const
{
  for(const auto& i : _indices)
    if((*_x)[i].volume() != 0.)
      return false;
  return true;
}

bool PropagationSpan<IntervalVectorComponent>::intersects(const std::shared_ptr<PropagationSpanBase>& view) const
{
  auto view_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  if(!view_cast || view_cast->_x != _x)
    return false;

  for(const auto& i : _indices)
    for(const auto& i_view : view_cast->_indices)
    {
      if(i == i_view)
        return true;
      if(i_view > i)
        break; // i is not in the ordered vector view_cast->_indices
    }
  return false;
}

void PropagationSpan<IntervalVectorComponent>::contract(const std::shared_ptr<PropagationSpanBase>& view)
{
  auto view_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  assert(view_cast && "fail to pointer cast PropagationSpanBase into PropagationSpan<IntervalVector>");
  assert(view_cast->_x == _x && "views are not related to the same domain");

  std::vector<size_t> indices;
  for(const auto& di : _indices)
    for(const auto& xdi : view_cast->_indices)
      if(di == xdi)
      {
        indices.push_back(di);
        break;
      }
  _indices = indices;
}

void PropagationSpan<IntervalVectorComponent>::merge(const std::shared_ptr<PropagationSpanBase>& view)
{
  auto view_cast = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
  assert(view_cast && "fail to pointer cast PropagationSpanBase into PropagationSpan<IntervalVector>");
  assert(view_cast->_x == _x && "views are not related to the same domain");

  std::vector<size_t> merged_indices;
  std::set_union(_indices.cbegin(), _indices.cend(),
                 view_cast->_indices.cbegin(), view_cast->_indices.cend(),
                 std::back_inserter(merged_indices));
  _indices = merged_indices;
}

bool PropagationSpan<IntervalVectorComponent>::is_view_from_const() const
{
  return false;
}

bool PropagationSpan<IntervalVectorComponent>::is_view_from_var() const
{
  return false;
}

std::string PropagationSpan<IntervalVectorComponent>::to_string() const
{
  string str = _x->name() + "[";
  for(const auto& i : _indices)
    str += std::to_string(i) + ",";
  str.pop_back();
  return str + "]";
}*/
/*
template<>
std::shared_ptr<PropagationSpanBase> propagspan_from_contraction(IntervalVector_<-1>& bef, IntervalVector_<-1>& aft)
{
  cout << "HERE" << endl;
  assert(bef.size() == aft.size());
  std::vector<size_t> indices;
  for(size_t i = 0 ; i < bef.size() ; i++)
    if(bef[i] != aft[i])
      indices.push_back(i);
  auto ptr = std::make_shared<PropagationSpan<IntervalVector>>(&aft,indices);
  return ptr;
}*/