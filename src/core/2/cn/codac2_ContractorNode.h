#ifndef __CODAC2_CONTRACTORNODE__
#define __CODAC2_CONTRACTORNODE__

#include <list>
#include <memory>
#include <string>
#include "codac2_DomainNode.h"
#include "codac2_PropagationSpan.h"
#include "codac2_tuples.h"

namespace codac2
{
  class DomainNodeBase;

  class ContractorNodeBase
  {
    public:

      ContractorNodeBase() {}
      virtual std::list<std::shared_ptr<PropagationSpanBase>> contract_and_propag() = 0;
      virtual bool is_impacted(const std::list<std::shared_ptr<PropagationSpanBase>>& views) const = 0;
      virtual std::shared_ptr<ContractorNodeBase> create_viewed_ctc(const std::list<std::shared_ptr<PropagationSpanBase>>& views) = 0;
      virtual bool same_raw_ctcdoms(const std::shared_ptr<ContractorNodeBase>& c) const = 0;
      virtual void extend_views(const std::list<std::shared_ptr<PropagationSpanBase>>& views) = 0;
      virtual const std::list<std::shared_ptr<PropagationSpanBase>>& views() = 0;
      virtual void associate_domains(std::vector<std::shared_ptr<DomainNodeBase>>& cn_domains) = 0;

      virtual std::string to_string() const = 0;
  };

  template<typename T>
  class DomainNode;

  template<typename C,typename... T>
  class ContractorNode : public ContractorNodeBase, 
      // The following is needed to handle shared_ptr with pybind11: (todo: still valid?)
      public std::enable_shared_from_this<ContractorNode<C,T...>>
  {
    public:

      ContractorNode(C& ctc, T&&... x)
        : _ctc(ctc), _x(std::make_tuple((std::make_shared<DomainNode<T>>(std::forward<T>(x)))...)),
        _views(ctc.get_default_spans(x...))
      { }

      bool is_impacted(const std::list<std::shared_ptr<PropagationSpanBase>>& views) const
      {
        for(const auto& external_view : views)
          for(const auto& internal_view : _views)
            if(external_view->intersects(internal_view))
              return true;
        return false;
      }

      void extend_views(const std::list<std::shared_ptr<PropagationSpanBase>>& views)
      {
        for(const auto& vout : views)
        {
          if(vout == nullptr || vout->is_empty())
            continue;

          bool has_been_merged = false;

          for(auto& vin : _views)
            if(vin->intersects(vout))
            {
              vin->merge(vout);
              has_been_merged = true;
              break;
            }

          if(!has_been_merged)
          {
            std::cout << "o" << std::endl;
            _views.push_back(vout);
            std::cout << "pushingback " << vout->to_string() << std::endl;
          }
        }
      }

      const std::list<std::shared_ptr<PropagationSpanBase>>& views()
      {
        return _views;
      }

      // must be protected:
      ContractorNode(ContractorNode<C,T...>& c, const std::list<std::shared_ptr<PropagationSpanBase>>& views)
        : _ctc(c._ctc), _x(c._x), _views(views)
      {
        
      }

      bool same_raw_ctcdoms(const std::shared_ptr<ContractorNodeBase>& c) const
      {
        auto pc = std::dynamic_pointer_cast<ContractorNode<C,T...>>(c);
        return pc && &pc->_ctc == &_ctc && are_all_members_the_same(_x,pc->_x);
      }

      std::shared_ptr<ContractorNodeBase> create_viewed_ctc(const std::list<std::shared_ptr<PropagationSpanBase>>& views)
      {
        std::list<std::shared_ptr<PropagationSpanBase>> merged_views(_views);
        // Merging views
        for(auto& v : merged_views)
        {
          bool has_been_restricted = false;
          for(const auto& ev : views)
          {
            if(v->intersects(ev))
            {
              v->contract(ev);
              has_been_restricted = true;
              break;
            }
          }
          if(!has_been_restricted)
            v = nullptr;
        }
        merged_views.remove(nullptr);
        return std::make_shared<ContractorNode<C,T...>>(*this, merged_views);
      }

      std::list<std::shared_ptr<PropagationSpanBase>> contract_and_propag()
      {
        std::list<std::shared_ptr<PropagationSpanBase>> output_views;

        std::apply(
          [this,&output_views](auto &&... args)
          {
            output_views = _ctc.contract_and_propag(*args...);
          }, _x);

        return output_views;
      }

      std::string to_string() const
      {
        std::string ctc_name = _ctc.name();
        ctc_name += "(";
        std::apply(
          [&ctc_name](auto &&... args)
          {
            ((ctc_name += args->to_string() + ","),...);
          }, _x);
        ctc_name.pop_back();
        ctc_name += ") triggered from one of the views={";
        for(const auto& vi : _views)
          ctc_name += vi->to_string() + ",";
        ctc_name.pop_back();
        return ctc_name + "}";
      }

      template<typename D>
      void associate_domain(std::vector<std::shared_ptr<DomainNodeBase>>& cn_domains, std::shared_ptr<D>& d)
      {
        for(auto& cn_xi : cn_domains)
          if(*d == cn_xi) // domains have same origin
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

    protected:

      C& _ctc;
      std::tuple<std::shared_ptr<DomainNode<T>>...> _x;
      std::list<std::shared_ptr<PropagationSpanBase>> _views;
  };
}

#endif