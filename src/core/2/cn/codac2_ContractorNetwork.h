#ifndef __CODAC2_CONTRACTORNETWORK__
#define __CODAC2_CONTRACTORNETWORK__

#include <list>
#include <vector>
#include <memory>
#include "codac2_DomainNode.h"
#include "codac2_ContractorNode.h"

namespace codac2
{

  class ContractorNetwork
  {
    public:

      ContractorNetwork();
      void add(const std::shared_ptr<ContractorNodeBase>& ctc);
      void add_ctc_to_stack(const std::shared_ptr<ContractorNodeBase>& ctc);
      void disable_auto_fixpoint(bool disable = true);
      void contract();

    //protected:

      std::vector<std::shared_ptr<ContractorNodeBase>> _v_ctc;
      std::vector<std::shared_ptr<DomainNodeBase>> _v_domains;
      std::list<std::shared_ptr<ContractorNodeBase>> _stack;

      bool _auto_fixpoint = true;
  };

}

#endif