#ifndef __CODAC2_DOMAINVIEW__
#define __CODAC2_DOMAINVIEW__

#include <numeric>
#include "codac2_Tube.h"
#include "codac2_Var.h"
#include "codac2_isbaseof.h"

namespace codac2
{
  class PropagationSpanBase
  {
    public:

      virtual ~PropagationSpanBase() = default;
      virtual bool is_empty() const = 0;
      virtual bool intersects(const std::shared_ptr<PropagationSpanBase>& view) const = 0;
      virtual void contract(const std::shared_ptr<PropagationSpanBase>& view) = 0;
      virtual void merge(const std::shared_ptr<PropagationSpanBase>& view) = 0;
      virtual bool cannot_propagate() const = 0;
      virtual bool is_view_from_var() const = 0;
      virtual bool is_view_from_const() const = 0;
      virtual std::string to_string() const
      {
        return "?";
      }
  };

  template<typename T>
  class PropagationSpan : public PropagationSpanBase
  {
    public:

      const bool _is_const = !std::is_base_of<Domain,typename std::remove_reference<T>::type>::value;
      const bool _is_var = is_base_of_any<Var,typename std::remove_reference<T>::type>::value;

      PropagationSpan(const typename std::remove_reference<T>::type* x = nullptr)
        : _x(x)
      { }

      bool is_empty() const
      {
        return !_x;
      }

      bool is_non_contractible() const
      {
        if constexpr(std::is_same<double,T>::value || std::is_same<Vector,T>::value)
          return true;
        if constexpr(std::is_same<Interval,T>::value || std::is_same<IntervalVector,T>::value)
          return _x->volume() == 0.;
        return false; // for large domains, avoiding to compute volume
      }

      template<typename D>
      PropagationSpan<T> operator&([[maybe_unused]] const PropagationSpan<D>& y) const
      {
        return PropagationSpan<T>();
      }
      
      bool intersects(const std::shared_ptr<PropagationSpanBase>& view) const
      {
        auto view_cast = std::dynamic_pointer_cast<PropagationSpan<T>>(view);
        return view_cast && (view_cast->_x == _x);
      }
      
      void contract([[maybe_unused]] const std::shared_ptr<PropagationSpanBase>& view)
      {
        assert(std::dynamic_pointer_cast<PropagationSpan<T>>(view) && "cannot contract views from different domains");
        // Nothing to do
      }
      
      void merge([[maybe_unused]] const std::shared_ptr<PropagationSpanBase>& view)
      {
        assert(std::dynamic_pointer_cast<PropagationSpan<T>>(view) && "cannot merge views from different domains");
        // Nothing to do
      }

      bool cannot_propagate() const
      {
        return is_view_from_const() || is_non_contractible();
      }

      bool is_view_from_const() const
      {
        return _is_const;
      }

      bool is_view_from_var() const
      {
        return _is_var;
      }

      std::string to_string() const
      {
        if(_x)
        {
          if constexpr(std::is_base_of<Domain,T>::value)
          {
            assert(_x);
            if(_x!=nullptr)return _x->name();
            else return "e";
          }
          else return "[]";
          //else // could be list<double>
          //{
          //  std::ostringstream s;
          //  s << *_x;
          //  return s.str();
          //}
        }
        else return "[]";
      }

    //protected:

      const typename std::remove_reference<T>::type* _x = nullptr;
  };

  //template<typename T>
  //std::shared_ptr<PropagationSpanBase> propagspan_from_contraction(IntervalVectorComponent* bef, T* aft) // bef should be const
  //{
  //  if(*bef != *aft)
  //    return std::make_shared<PropagationSpan<T>>(aft);
  //  return std::make_shared<PropagationSpan<T>>();
  //}

  template<>
  class PropagationSpan<IntervalVector> : public PropagationSpanBase
  {
    public:

      PropagationSpan(const IntervalVector* x);
      PropagationSpan(const IntervalVector* x, const std::vector<size_t>& indices);
      bool is_empty() const;
      bool is_non_contractible() const;

      bool cannot_propagate() const
      {
        return is_view_from_const() || is_non_contractible();
      }

      template<typename D>
      PropagationSpan<IntervalVector> operator&(const PropagationSpan<D>& y) const
      {
        return PropagationSpan<IntervalVector>();
      }
      
      bool intersects(const std::shared_ptr<PropagationSpanBase>& view) const;
      void contract(const std::shared_ptr<PropagationSpanBase>& view);
      void merge(const std::shared_ptr<PropagationSpanBase>& view);
      bool is_view_from_const() const;
      bool is_view_from_var() const;
      std::string to_string() const;

    //protected:

      const IntervalVector* _x = nullptr;
      std::vector<size_t> _indices;
  };

  template<>
  inline bool PropagationSpan<Interval>::intersects(const std::shared_ptr<PropagationSpanBase>& view) const
  {
    auto view_cast_i = std::dynamic_pointer_cast<PropagationSpan<Interval>>(view);
    if(view_cast_i && view_cast_i->_x == _x)
      return true;

    auto view_cast_iv = std::dynamic_pointer_cast<PropagationSpan<IntervalVector>>(view);
    if(view_cast_iv)
      for(const auto& i_view : view_cast_iv->_indices)
        if(_x == &(*view_cast_iv->_x)[i_view])
          return true;

    return false;
  }

  template<typename Q>
  static std::shared_ptr<PropagationSpanBase> propagspan_from_contraction(Q& bef, Q& aft) // bef should be const
  {
    if /* todo C++20: constexpr and use is_var*/constexpr(is_base_of_any<Var,typename std::remove_reference<Q>::type>::value)
      return propagspan_from_contraction_var(bef, aft);

    if(bef != aft)
      return std::make_shared<PropagationSpan<Q>>(&aft);
    return nullptr; // sure?
  }

  template<typename T,typename Q>
  std::shared_ptr<PropagationSpanBase> propagspan_from_contraction_var(Var<T>& bef, Var<Q>& aft)
  {
    return propagspan_from_contraction(static_cast<T&>(bef), static_cast<Q&>(aft));
  }

  template<>
  std::shared_ptr<PropagationSpanBase> propagspan_from_contraction(IntervalVector& bef, IntervalVector& aft)
  {
    assert(bef.size() == aft.size());
    std::vector<size_t> indices;
    for(size_t i = 0 ; i < bef.size() ; i++)
      if(bef[i] != aft[i])
        indices.push_back(i);
    if(!indices.empty())
      return std::make_shared<PropagationSpan<IntervalVector>>(&aft,indices);
    else
      return nullptr;
  }

  /*template<>
  class PropagationSpan<IntervalVectorComponent> : public PropagationSpanBase
  {
    public:

      PropagationSpan(const IntervalVectorComponent& x);
      bool is_empty() const;
      bool is_non_contractible() const;

      bool cannot_propagate() const
      {
        return is_view_from_const() || is_non_contractible();
      }

      template<typename D>
      PropagationSpan<IntervalVector> operator&(const PropagationSpan<D>& y) const
      {
        return PropagationSpan<IntervalVector>();
      }
      
      bool intersects(const std::shared_ptr<PropagationSpanBase>& view) const;
      void contract(const std::shared_ptr<PropagationSpanBase>& view);
      void merge(const std::shared_ptr<PropagationSpanBase>& view);
      bool is_view_from_const() const;
      bool is_view_from_var() const;
      std::string to_string() const;

    //protected:

      const IntervalVector* _x = nullptr;
      std::vector<size_t> _indices;
  };*/
}

#endif