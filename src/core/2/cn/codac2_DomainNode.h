#ifndef __CODAC2_DOMAINNODE__
#define __CODAC2_DOMAINNODE__

#include <string>
#include <memory>
#include <vector>

#include "codac2_ContractorNode.h"
#include "codac2_PropagationSpan.h"
#include "codac2_Var.h"
#include "codac2_isbaseof.h"

namespace codac2
{
  class ContractorNodeBase;

  class DomainNodeBase
  {
    public:

      DomainNodeBase() {}
      virtual ~DomainNodeBase() = default;
      virtual const std::vector<std::weak_ptr<ContractorNodeBase>>& contractors() const = 0;
      virtual void reset_if_var() = 0;
      virtual bool operator==(const std::shared_ptr<DomainNodeBase>& x) const = 0;
      virtual std::string to_string() const = 0;
      virtual bool is_var() const = 0;
  };


  template<typename T>
  class PropagationSpan;

  template<typename T>
  class DomainCaster;

  template<typename T>
  class DomainNode : public DomainNodeBase
  {
    public:

      DomainNode(T&& x)
        : _dom_cast(std::forward<T>(x))
      { }

      operator typename DomainCaster<T>::OutputType&()
      {
        return _dom_cast.operator typename DomainCaster<T>::OutputType&();
      }

      void add_ctc(const std::shared_ptr<ContractorNodeBase>& ctc)
      {
        _v_ctc.push_back(ctc);
      }

      std::shared_ptr<PropagationSpanBase> update_and_propag()
      {
        return _dom_cast.update_and_propag();
      }

      const std::vector<std::weak_ptr<ContractorNodeBase>>& contractors() const
      {
        return _v_ctc;
      }

      bool operator==(const std::shared_ptr<DomainNodeBase>& x) const
      {
        if(typeid(DomainNode<T>) != typeid(*x))
          return false;
        return _dom_cast == std::dynamic_pointer_cast<DomainNode<T>>(x)->_dom_cast;
      }

      /* todo C++20: constexpr*/ bool is_var() const
      {
        return is_base_of_any<Var,typename std::remove_reference<T>::type>::value;
      }

      void reset_if_var()
      {
        if /* todo C++20: constexpr and use is_var*/constexpr(is_base_of_any<Var,typename std::remove_reference<T>::type>::value)
          _dom_cast.operator typename DomainCaster<T>::OutputType&().reset_to_default_value();
      }

      std::string to_string() const
      {
        return _dom_cast.to_string();
      }

    protected:

      DomainCaster<T> _dom_cast;
      std::vector<std::weak_ptr<ContractorNodeBase>> _v_ctc;
  };
}

#endif