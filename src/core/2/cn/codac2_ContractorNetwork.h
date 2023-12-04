#ifndef __CODAC2_CONTRACTORNETWORK__
#define __CODAC2_CONTRACTORNETWORK__

#include <list>
#include <vector>
#include <memory>
#include "codac2_DomainNode.h"
#include "codac2_ContractorNode.h"
#include "codac2_Var.h"

namespace codac2
{
  class ContractorNetwork
  {
    public:

      ContractorNetwork();
      void add(const std::shared_ptr<ContractorNodeBase>& ctc);
      void add_ctc_to_stack(const std::shared_ptr<ContractorNodeBase>& ctc);
      void disable_auto_fixpoint(bool disable = true);
      double contract(bool verbose = true);

      void reset_all_vars();

      template<typename T>
      void reset_var(const Var<T> *ref, const T& x)
      {
        for(auto& d : _v_domains)
          if(d->raw_ptr() == ref)
          {
            static_cast<DomainNode<T>&>(*d).get() = x;
            return;
          }

        assert(false && "unable to find variable");
      }

      void trigger_all_contractors();

    //protected:

      std::vector<std::shared_ptr<ContractorNodeBase>> _v_ctc;
      std::vector<std::shared_ptr<DomainNodeBase>> _v_domains;
      std::list<std::shared_ptr<ContractorNodeBase>> _stack;

      bool _auto_fixpoint = true;
  };

}

#endif