/** 
 *  \file
 *  Ctc class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTC__
#define __CODAC2_CTC__

#include <string>
#include <memory>
#include <codac2_PropagationSpan.h>
#include <codac2_ContractorNode.h>

namespace codac2
{
  /**
   * \enum TimePropag
   * \brief Specifies the temporal propagation way (forward or backward in time)
   */
  enum class TimePropag
  {
    FORWARD = 0x01, ///< forward in time (from \f$t^-\f$ to \f$t^+\f$)
    BACKWARD = 0x02 ///< backward in time (from \f$t^+\f$ to \f$t^-\f$)
  };
  
  /**
   * \brief Allows tests on combinations of propagation ways
   *
   * Used for tests such as `if(time_propag & TimePropag::FORWARD) { ... }`
   *
   * \param a first TimePropag operand
   * \param b second TimePropag operand
   * \return intersection of propagation ways
   */
  inline int operator&(TimePropag a, TimePropag b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }
  
  /**
   * \brief Allows a combination of propagation ways
   *
   * \note For instance: `FORWARD | BACKWARD`
   *
   * \param a first TimePropag operand
   * \param b second TimePropag operand
   * \return union of propagation ways, such as `FORWARD | BACKWARD`
   */
  inline TimePropag operator|(TimePropag a, TimePropag b)
  { return static_cast<TimePropag>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \brief Base class for contractors
   * 
   * The class allows to name the contractors objects for graphical/debugging purposes.
   */
  class Ctc
  {
    public:

      std::string name() const
      {
        return _name.empty() ? "C?" : _name;
      }

      void set_name(const std::string& name) const
      {
        _name = name;
      }

    protected:

      mutable std::string _name;
  };

  class BoxCtc : public Ctc
  {
    public:

      virtual void contract(IntervalVector& x) = 0;
  };

  // The following functions allow to use the contractors into CNs.
  // They must be defined in each derived class for this purpose.

  /**
   * \brief Returns a CN node handling the current Ctc object related to the specified domains.
   * 
   * \param x the contractor domains as specified by the user
   * \return a pointer to a ContractorNode
   */
  #define make_available_to_cn___call_operator() \
    \
    template<typename... T> \
    auto operator()(T&&... x) \
    { \
      return std::make_shared<ContractorNode<typename std::remove_reference<decltype(*this)>::type,T...>>(*this, std::forward<T>(x)...); \
    } \

  /**
   * \brief Returns the default propagation spans for this contractor, assuming a maximal contraction effect.
   * 
   * This function is used to evaluate if this contractor is impacted by a given propagation effect,
   * by comparing its default spans to those resulting from a former contraction.
   * 
   * \param x the contractor domains as specified by the user
   * \return a list of domains potentially related to this contractor
   */
  #define make_available_to_cn___get_default_spans() \
    \
    template<typename T> \
    auto create_span_var(Var<T>& x) \
    { \
      return create_span(static_cast<T&>(x)); \
    } \
    \
    template<typename T> \
    auto create_span(T& x) \
    { \
      if constexpr(is_base_of_any<Var,typename std::remove_reference<T>::type>::value) \
        return create_span_var(x); \
      else \
        return std::make_shared<PropagationSpan<typename std::remove_reference<T>::type>>(&x); \
    } \
    \
    template<typename... T> \
    auto get_default_spans(T&... x) \
    { \
      std::list<std::shared_ptr<PropagationSpanBase>> spans; \
      /* Each domain involved in the "contract()" function of the Ctc class are potentially responsible for propagations. */ \
      (spans.push_back(create_span(x)), ...); \
      spans.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); }); \
      return spans; \
    } \

  /**
   * \brief Performs the contraction (inside the CN process) and returns the spans allowing propagation.
   * 
   * \param x the contractor domains after automatic cast (see DomainCaster)
   * \return a list of spans representing parts of domains that have been contracted
   */
  #define make_available_to_cn___contract_and_propag() \
    \
    template<typename... T> \
    auto contract_and_propag(T&&... x) \
    { \
      contract(x...); /* actual mandatory function 'contract(...) of the Ctc object */ \
      std::list<std::shared_ptr<PropagationSpanBase>> propag_spans; \
      (propag_spans.push_back(x.update_and_propag()), ...); \
      propag_spans.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); }); \
      return propag_spans; \
    } \

  /**
   * \brief Allows to vectorize a scalar contractor to a list of vector domains.
   * 
   * \param x the contractor domains after automatic cast (see DomainCaster)
   * \return a list of spans representing parts of domains that have been contracted
   */
  // not working #define make_available_to_cn___vect() \
  // not working   \
  // not working   template<typename... T> \
  // not working   auto vect(T&&... x) \
  // not working   { \
  // not working     using CtcClass = typename std::remove_reference<decltype(*this)>::type; \
  // not working     size_t n = [](const auto& first, const auto& ...) -> auto { return first.size(); }(x...); \
  // not working     std::vector<std::shared_ptr<ContractorNode<CtcClass,typename std::remove_reference<T>::type::VECTOR_ITEM...>>> v_contractors(n); \
  // not working     for(size_t i = 0 ; i < n ; i++) \
  // not working       v_contractors[i] = std::make_shared<ContractorNode<CtcClass,typename std::remove_reference<T>::type::VECTOR_ITEM...>>( \
  // not working         *this, std::forward<typename std::remove_reference<T>::type::VECTOR_ITEM>(x[i])...); \
  // not working     return v_contractors; \
  // not working   } \

  #define make_available_to_cn() \
    make_available_to_cn___call_operator() \
    make_available_to_cn___get_default_spans() \
    make_available_to_cn___contract_and_propag()


  // The following function allows to use the extand some contractors on slices, to tubes.
  // It must be defined in each derived class for this purpose.

  #define make_available_to_tubes() \
    template<typename... T> \
    void contract_fwdbwd(TimePropag t_propa, Tube<T>&... x) \
    { \
      auto tdomain = std::get<0>(std::make_tuple(x...)).tdomain(); \
      \
      if(t_propa & TimePropag::FORWARD) /* Computations related to forward propagation in time */ \
        for(const auto& st : tdomain->tslices()) \
          contract((*std::static_pointer_cast<Slice<T>>(st.slices().at(&x)))...); \
      \
      if(t_propa & TimePropag::BACKWARD) /* Computations related to backward propagation in time */ \
        for(auto it = tdomain->tslices().rbegin() ; it != tdomain->tslices().rend() ; ++it) \
          contract((*std::static_pointer_cast<Slice<T>>(it->slices().at(&x)))...); \
    } \
    
}

#endif