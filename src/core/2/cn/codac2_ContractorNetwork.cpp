#include <cassert>
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
    assert(!ctc->to_be_called());
    _stack.push_back(ctc);
    ctc->set_call_flag(true);
  }

  void ContractorNetwork::disable_auto_fixpoint(bool disable)
  {
    _auto_fixpoint = !disable;
  }

  void ContractorNetwork::contract()
  {
    do
    {
      shared_ptr<ContractorNodeBase> current_ctc = _stack.front();
      _stack.pop_front();

      auto contracted_doms = current_ctc->call_contract();

      for(auto& d : contracted_doms)
        for(auto& ci : d->contractors())
        {
          auto p_c = ci.lock();
          if(!_auto_fixpoint && current_ctc.get() == p_c.get()) continue;
          if(!p_c->to_be_called())
            add_ctc_to_stack(p_c);
        }

    } while(!_stack.empty());
  }
}