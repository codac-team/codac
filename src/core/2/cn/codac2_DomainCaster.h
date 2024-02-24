/** 
 *  \file
 *  DomainCaster class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_DOMAINCASTER__
#define __CODAC2_DOMAINCASTER__

#include <iostream>
#include <memory>
#include <codac2_PropagationSpan.h>
#include <codac2_Var.h>
#include <codac2_isbaseof.h>
// Includes related to specialization
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_Paving.h>

namespace codac2
{
  /**
   * \brief Virtual class for handling DomainCaster<> objects
   */
  class DomainCasterBase
  {
    public:

      virtual std::shared_ptr<PropagationSpanBase> update_and_propag() = 0;
  };

  /*template<typename T>
  struct KK
  {
  public:
    using TT = T;
  };

  template<typename T>
  struct KK<Var<T>>
  {
  public:
    using TT = T;
  };*/

  /**
   * \brief Defines a caster for implicitly converting input types ('double', 'Interval', 'Vector') into
   * contractible types that can be directly processed by contractors (resp. 'Interval', 'Interval',
   * 'IntervalVector'). This facilitates the use of CN and allows more flexibility is the combination of contractors.
   * In addition, the caster is responsible for propagation by producing a PropagationSpan.
   * 
   * \tparam InputType type of the input variable that has to be casted into a domain type
   */
  template<typename InputType>
  class DomainCaster : public DomainCasterBase
  {
    public:

      //using TT = typename KK<typename std::remove_reference<InputType>::type>::TT;

      using OutputType = typename std::remove_reference<InputType>::type;
      // ^ The OutputType is the type that is actually processed by the CN.
      //   The InputType is the C++ type of the external object involved by the user.
      //   InputType is relative to the language (it can be 'double', 'Vector', 'Interval')
      //   while OutputType is a type that can be processed by contractors (resp. 'Interval', 
      //   'IntervalVector', 'Interval').

      const bool _is_const_domain = !std::is_base_of<Domain,OutputType>::value;
      // ^ By default, an object that inherits from Domain is not const.
      //   For specific cases such as const Interval&, template specialization allows
      //   to redefine this value.

      /**
       * \brief Creates a domain caster from InputType x to an OutputType contractible domain.
       *
       * \param x input type, also used as a key in the CN, based on its memory adress
       */
      DomainCaster(InputType x) // InputType may be a reference
        : _x(x), _x_before_ctc(x)
      { }

      /**
       * \brief Operator allowing the cast from InputType to OutputType
       * 
       * \return the corresponding casted domain, that can be directly used by contractors
       */
      operator OutputType&()
      {
        // In this class, if InputType!=OutputType, we assume that automatic
        // cast is possible from InputType to OutputType.
        // See template specialization for other cases.
        return _x;
      }

      /**
       * \brief It may be necessary to call for the update of the external domain (that is not 
       * managed by the CN, and corresponding to InputType for the caster). The update can be
       * done based on the value of the OutputType of this caster. The function returns a view 
       * corresponding to the updated part of the InputType. This propagation span then allows
       * further contractions in the CN.
       *
       * \return a pointer to the new created view
       */
      std::shared_ptr<PropagationSpanBase> update_and_propag()
      {
        // No explicit update is needed: direct reference has
        // been used to contract the domain.

        // Generating the view based on the recent contractness:
        auto span = propagspan_from_contraction<InputType>(_x_before_ctc, _x);
        _x_before_ctc = _x; // saved value for future comparisons
        return span;
      }

      /**
       * \brief Tests if this caster and x are related to the same external value.
       * 
       * \param x a pointer to the caster to be tested.
       * \return `true` if the two casters casts the same external value.
       */
      bool operator==(const DomainCaster<InputType>& x) const
      {
        return &_x == &x._x;
      }

      /**
       * \brief Returns the name of the represented domain in the CN.
       * This name is actually related to the external value, that can be
       * directly named by the user.
       *
       * \return the name of the domain.
       */
      std::string to_string() const
      {
        return _x.name();
      }

    protected:

      InputType _x; // may be a reference depending on InputType
      // _x represents the actual value managed by the CN during propagations
      // _x is also used as a key in the CN, based on its memory adress
      OutputType _x_before_ctc;
      // _x_before_ctc is a local variable only used to assess the contraction:
      // it enables the propagation process via PropagationSpan objects
  };

  #define create_caster(InputType, OutputType_, is_const_fnc, view_fnc, update_fnc, str_fnc) \
    class DomainCaster<InputType> : public DomainCasterBase \
    { \
      public: \
        \
        using OutputType = OutputType_; \
        const bool _is_const_domain = is_const_fnc; \
        \
        DomainCaster(InputType x) \
          : _x(x) \
        { \
          _y = std::make_shared<OutputType>(_x); \
          _y_before_ctc = std::make_shared<OutputType>(_x); \
        } \
        \
        operator OutputType&() \
        { \
          return *_y; \
        } \
        \
        std::shared_ptr<PropagationSpanBase> update_and_propag() \
        { \
          auto span = view_fnc; \
          update_fnc; \
          return span; \
        } \
        \
        bool operator==(const DomainCaster<InputType>& x) const \
        { \
          return &_x == &x._x; \
        } \
        \
        std::string to_string() const \
        { \
          std::ostringstream s; \
          str_fnc; \
          return s.str(); \
        } \
      \
      protected: \
         \
        InputType _x; \
        std::shared_ptr<OutputType> _y = nullptr, _y_before_ctc = nullptr; \
    }; \

  #define create_caster_for_const_value(InputType_, OutputType_) \
    create_caster(InputType_, OutputType_, \
      true, /* the external value is const */ \
      nullptr, /* the propagation span is null: no propagation for const values */ \
      *_y = *_y_before_ctc, /* the internal domain is updated with a saved value before contraction */ \
      s << _x /* the name of the domain is the value itself, as defined by the user */ \
    ) \

  // Casters for const values
  
  template<>
  create_caster_for_const_value(int, Interval)
  
  template<>
  create_caster_for_const_value(int&, Interval)
  
  template<>
  create_caster_for_const_value(double, Interval)

  template<>
  create_caster_for_const_value(double&, Interval)

  template<>
  create_caster_for_const_value(Vector, IntervalVector);

  template<>
  create_caster_for_const_value(const Vector&, IntervalVector);

  template<>
  create_caster_for_const_value(Interval, Interval)

  template<>
  create_caster_for_const_value(const Interval&, Interval);

  template<>
  create_caster_for_const_value(const IntervalVector&, IntervalVector);

  // Other cases
  
  template<>
  class DomainCaster<Paving&> : public DomainCasterBase
  {
    public:

      using OutputType = Paving;

      DomainCaster(Paving& x);
      operator Paving&();
      std::shared_ptr<PropagationSpanBase> update_and_propag();
      bool operator==(const DomainCaster<Paving&>& x) const;
      std::string to_string() const;

    protected:

      Paving& _x;
      double _prev_volume = 0.;
      size_t _prev_nb_leaves = 0.;
  };
}

#endif