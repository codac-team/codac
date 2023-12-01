#ifndef __CODAC2_CONTRACTORNODE__
#define __CODAC2_CONTRACTORNODE__

#include <list>
#include <tuple>
#include <memory>
#include <functional>
#include "codac2_DomainNode.h"

namespace codac2
{
  class Contractor;
  class ContractorNodeBase;

  class ContractorNodeBase
  {
    public:

      virtual ~ContractorNodeBase() = default;
      virtual constexpr size_t nb_args() const = 0;
      virtual std::list<std::shared_ptr<DomainNodeBase>> call_contract() = 0;
      virtual Contractor* raw_ptr() const = 0;
      virtual void associate_domains(std::vector<std::shared_ptr<DomainNodeBase>>& cn_domains) = 0;
      virtual std::string contractor_class_name() const = 0;
      void set_call_flag(bool flag = true);
      bool to_be_called() const;

      friend std::ostream& operator<<(std::ostream& os, const ContractorNodeBase& d);

    protected:

      bool _to_be_called = false; // this is redundant with the presence of the node 
      // in the CN stack, but avoids to search the node in the stack
  };

  template<typename C,typename... T>
  class ContractorNode : public ContractorNodeBase, 
    // The following is needed to handle shared_ptr with pybind11:
    public std::enable_shared_from_this<ContractorNode<C,T...>>
  {
    public:

      ContractorNode(C& ctc, T&... x)
        : _ctc(std::ref(ctc)), 
          _x(std::make_tuple((std::make_shared<DomainNode<
              // x is T&, so the reference needs to be removed
              // in order to build DomainNode<T>:
              typename std::remove_reference<decltype(x)>::type
            >>(x)) ...))
      {

      }

      constexpr size_t nb_args() const
      {
        return std::tuple_size<decltype(_x)>::value;
      }

      Contractor* raw_ptr() const
      {
        return &(_ctc.get());
      }

      void add_domain_if_contracted(const std::shared_ptr<DomainNodeBase>& d, std::list<std::shared_ptr<DomainNodeBase>>& l)
      {
        if(d->has_been_contrated())
          l.push_back(d);
      }

      std::list<std::shared_ptr<DomainNodeBase>> call_contract()
      {
        _to_be_called = false;
        std::list<std::shared_ptr<DomainNodeBase>> contracted_doms;

        std::apply(
          [this](auto &&... args)
          {
            _ctc.get().contract(args->get()...);
          }, _x);

        std::apply(
          [this,&contracted_doms](auto &&... args)
          {
            (add_domain_if_contracted(args,contracted_doms),...);
          }, _x);

        return contracted_doms;
      }

      template<typename D>
      void associate_domain(std::vector<std::shared_ptr<DomainNodeBase>>& cn_domains, std::shared_ptr<D>& d)
      {
        for(auto& cn_xi : cn_domains)
          if(cn_xi.get() == d.get() || d->raw_ptr() == cn_xi->raw_ptr())
          {
            d = std::dynamic_pointer_cast<D>(cn_xi);
            return;
          }

        cn_domains.push_back(d);
      }
      
      void associate_domains(std::vector<std::shared_ptr<DomainNodeBase>>& cn_domains)
      {
        // Associate domains with items already existing in the CN
        std::apply(
          [this,&cn_domains](auto &&... args)
          {
            (associate_domain(cn_domains,args),...);
          }, _x);

        // Update the contractors lists of the domains with the current new contractor
          std::apply(
          [this](auto &&... args)
          {
            (args->add_ctc(this->shared_from_this()),...);
          }, _x);
      }

      std::string contractor_class_name() const
      {
        return typeid(C).name();
      }


    protected:

      std::reference_wrapper<C> _ctc;
      std::tuple<std::shared_ptr<DomainNode<T>>...> _x;
  };
}

#endif