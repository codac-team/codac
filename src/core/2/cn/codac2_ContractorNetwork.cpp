/** 
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_ContractorNetwork.h"

using namespace std;
using namespace codac2;

ContractorNetwork::ContractorNetwork()
{ }

ContractorNetwork::ContractorNetwork(std::initializer_list<std::shared_ptr<ContractorNodeBase>> l)
{
  for(const auto& li : l)
    add(li);
}

void ContractorNetwork::add(const std::shared_ptr<ContractorNodeBase>& ctc)
{
  ctc->associate_domains(_v_domains);
  _v_ctc.push_back(ctc);
  _stack.push_back(ctc); // to be contracted at least once
}

void ContractorNetwork::add(const std::vector<std::shared_ptr<ContractorNodeBase>>& v_ctc)
{
  for(const auto& ci : v_ctc)
    add(ci);
}

void ContractorNetwork::merge_into_stack(std::shared_ptr<ContractorNodeBase> ctc)
{
  std::list<std::shared_ptr<ContractorNodeBase>> merged_items;
  for(auto& c : _stack)
    if(c->same_raw_ctcdoms(ctc) && c->is_impacted(ctc->views()))
    {
      c->extend_views(ctc->views());
      // todo: break?
      merged_items.push_back(c);
    }
  
  if(merged_items.empty())
    _stack.push_back(ctc);

  else
    for(const auto& merged_i : merged_items)
      _stack.splice(_stack.end(), _stack, std::find(_stack.begin(), _stack.end(), merged_i));
}

template<typename W>
char plural(const W& w)
{
  return w.size() > 1 ? 's' : '\0';
}

double ContractorNetwork::contract(bool verbose, bool verbose_ctc_calls)
{
  if(verbose)
  {
    std::cout << "Contractor network ("
      << _v_ctc.size() << " contractor" << plural(_v_ctc) << ", "
      << _v_domains.size() << " domain" << plural(_v_domains) << ")" << std::endl;
    std::cout << "Computing, " << _stack.size() << " contractor" << plural(_stack) << " currently in stack..." << std::endl;
  }

  clock_t t_start = clock();

  while(!_stack.empty())
  {
    std::shared_ptr<ContractorNodeBase> ci = _stack.front();
    _stack.pop_front();

    if(verbose_ctc_calls)
      std::cout << "Calling " << ci->to_string() << std::endl;

    auto ci_contracted_views = ci->contract_and_propag();
    ci_contracted_views.remove(nullptr);
    ci_contracted_views.remove_if([](const std::shared_ptr<PropagationSpanBase>& v){ return v->is_empty(); });

    if(verbose_ctc_calls)
    {
      std::cout << "  Propagating views =";
      if(ci_contracted_views.empty())
        std::cout << " none.";
      else
        for(const auto& vi : ci_contracted_views)
          std::cout << " " << vi->to_string();
      std::cout << std::endl;
    }

    for(const auto& cj : _v_ctc) // could be restricted to ctc of domains of this ctc
    {
      // All the contractors related to contracted views are added to the stack
      if((_force_fixed_point || !ci->same_raw_ctcdoms(cj)) && cj->is_impacted(ci_contracted_views))
      {
        auto new_cj = cj->create_viewed_ctc(ci_contracted_views);
        if(verbose_ctc_calls)
          std::cout << "  Creating new contractor node: " << new_cj->to_string() << std::endl;
        merge_into_stack(new_cj);
      }
    }
  }

  double elapsed_time = (double)(clock()-t_start)/CLOCKS_PER_SEC;
  if(verbose)
    std::cout << "> Constraint propagation time: " << elapsed_time << "s" << std::endl;
  return elapsed_time;
}

void ContractorNetwork::reset_variables()
{
  for(auto& x : _v_domains)
    x->reset_if_var();
}

void ContractorNetwork::force_fixed_point(bool force)
{
  _force_fixed_point = force;
}

void ContractorNetwork::get_views_from_cn(std::list<std::shared_ptr<PropagationSpanBase>>& l)
{
  for(auto& ctc : _v_ctc)
    for(auto& ctc_vi : ctc->views())
      if(!ctc_vi->is_view_from_var() && !ctc_vi->is_view_from_const())
          l.push_back(ctc_vi);
}

void ContractorNetwork::trigger_all_contractors()
{
  for(auto& ctc : _v_ctc)
    _stack.push_back(ctc);
}