#ifndef __CODAC2_DOMAINNODE__
#define __CODAC2_DOMAINNODE__

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include "codac2_Domain.h"
#include "codac2_Var.h"

namespace codac2
{
  class Contractor;
  class ContractorNodeBase;

  class DomainNodeBase
  {
    public:

      virtual ~DomainNodeBase() = default;
      virtual void print() const = 0;
      virtual const Domain* raw_ptr() const = 0;
      virtual bool has_been_contrated() = 0;
      virtual const std::vector<std::weak_ptr<ContractorNodeBase>>& contractors() const = 0;
      virtual void add_ctc(const std::shared_ptr<ContractorNodeBase>& ctc) = 0;
      virtual std::string domain_class_name() const = 0;
      virtual void reset() = 0;
      virtual bool is_var() const = 0;

      friend std::ostream& operator<<(std::ostream& os, const DomainNodeBase& d);
  };

  template<typename T_>
  class DomainNode : public DomainNodeBase
  {
    using T = typename std::remove_const<T_>::type;

    public:

      constexpr static bool _is_var = std::is_base_of<VarBase,T>::value;

      std::reference_wrapper<T> make_ref(T_& x)
      {
        if constexpr (std::is_const<T_>::value)
        {
          _local_dom = std::make_shared<T>(x);
          return std::ref(*_local_dom);
        }
        else
          return std::ref(x);
      }

      explicit DomainNode(T_& x)
        : _x(make_ref(x)), _volume(x.dom_volume())
      {
        if constexpr (std::is_const<T_>::value)
          _raw_ptr = &x;
        else
          _raw_ptr = &(_x.get());
      }

      T& get()
      {
        return _x.get();
      }

      void print() const
      {
        std::cout << _x.get() << std::flush;
      }

      const Domain* raw_ptr() const
      {
        return _raw_ptr;
      }

      bool has_been_contrated()
      {
        DomainVolume new_vol = _x.get().dom_volume();
        bool contracted = new_vol != _volume;
        _volume = new_vol;
        return contracted;
      }

      const std::vector<std::weak_ptr<ContractorNodeBase>>& contractors() const
      {
        return _v_ctc;
      }

      void add_ctc(const std::shared_ptr<ContractorNodeBase>& ctc)
      {
        _v_ctc.push_back(ctc);
      }

      std::string domain_class_name() const
      {
        return typeid(T).name();
      }

      void reset()
      {
        if constexpr(_is_var)
          _x.get().reset();
      }

      void reset(const T& x)
      {
        if constexpr(_is_var)
          _x.get().reset(x);
      }

      constexpr bool is_var() const
      {
        return _is_var;
      }

    protected:

      std::shared_ptr<T> _local_dom = nullptr;
      std::reference_wrapper<T> _x;
      DomainVolume _volume;
      std::vector<std::weak_ptr<ContractorNodeBase>> _v_ctc; // is dynamic
      const Domain *_raw_ptr; // used as a key
  };
}

#endif