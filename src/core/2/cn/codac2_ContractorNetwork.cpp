#include <cassert>
#include <time.h>
#include "codac2_ContractorNetwork.h"

using namespace std;

namespace codac2
{
  ContractorNetwork::ContractorNetwork()
  {

  }

  void ContractorNetwork::add(const shared_ptr<ContractorNodeBase>& ctc)
  {
    ctc->associate_domains(_v_domains);
    _v_ctc.push_back(ctc);
    add_ctc_to_stack(ctc); // to be contracted at least once
  }

  void ContractorNetwork::add_ctc_to_stack(const shared_ptr<ContractorNodeBase>& ctc)
  {
    if(find(_stack.begin(),_stack.end(),ctc) == _stack.end())
      _stack.push_back(ctc);
  }

  void ContractorNetwork::disable_auto_fixpoint(bool disable)
  {
    _auto_fixpoint = !disable;
  }

  double ContractorNetwork::contract(bool verbose)
  {
    if(verbose)
    {
      std::cout << "Contractor network (" << _v_ctc.size()
           << " contractors, " << _v_domains.size() << " domains)" << std::endl;
      std::cout << "Computing, " << _stack.size() << " contractors currently in stack" << std::endl;
    }

    clock_t t_start = clock();

    while(!_stack.empty())
    {
      shared_ptr<ContractorNodeBase> current_ctc = _stack.front();
      _stack.pop_front();

      auto contracted_doms = current_ctc->call_contract(false);

      for(auto& d : contracted_doms)
        for(auto& ci : d->contractors())
        {
          auto p_c = ci.lock();
          if(!_auto_fixpoint && current_ctc.get() == p_c.get()) continue;
          add_ctc_to_stack(p_c);
        }
    }
    
    double elapsed_time = (double)(clock()-t_start)/CLOCKS_PER_SEC;
    if(verbose)
      std::cout << "  Constraint propagation time: " << elapsed_time << "s" << std::endl;
    return elapsed_time;
  }
  
  void ContractorNetwork::reset_all_vars()
  {
    for(auto& d : _v_domains)
      if(d->is_var())
        d->reset();
  }

  void ContractorNetwork::trigger_all_contractors()
  {
    for(auto& c : _v_ctc)
      add_ctc_to_stack(c);
  }
}