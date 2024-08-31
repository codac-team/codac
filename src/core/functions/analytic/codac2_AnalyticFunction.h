/** 
 *  \file codac2_AnalyticFunction.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_AnalyticExpr.h"
#include "codac2_Domain.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionBase.h"
#include "codac2_template_tools.h"
#include "codac2_analytic_operations.h"

namespace codac2
{
  enum class EvaluationMode
  {
    NATURAL = 0x01,
    CENTERED = 0x02
  };

  inline EvaluationMode operator&(EvaluationMode a, EvaluationMode b)
  { return static_cast<EvaluationMode>(static_cast<int>(a) & static_cast<int>(b)); }

  inline EvaluationMode operator|(EvaluationMode a, EvaluationMode b)
  { return static_cast<EvaluationMode>(static_cast<int>(a) | static_cast<int>(b)); }
  
  template<typename T>
    requires std::is_base_of_v<OpValueBase,T>
  class AnalyticFunction : public FunctionBase<AnalyticExpr<T>>
  {
    public:

      AnalyticFunction(const FunctionArgsList& args, const std::shared_ptr<AnalyticExpr<T>>& y)
        : FunctionBase<AnalyticExpr<T>>(args, y)
      {
        assert_release(y->belongs_to_args_list(this->args()) && 
          "Invalid argument: variable not present in input arguments");
      }

      AnalyticFunction(const FunctionArgsList& args, const AnalyticVarExpr<T>& y)
        : AnalyticFunction(args, y.operator std::shared_ptr<AnalyticExpr<T>>())
      { }

      AnalyticFunction(const AnalyticFunction<T>& f)
        : FunctionBase<AnalyticExpr<T>>(f)
      { }

      template<typename... Args>
      typename T::Domain eval(const EvaluationMode& m, const Args&... x) const
      {
        switch(m)
        {
          case EvaluationMode::NATURAL:
            return natural_eval(x...);

          case EvaluationMode::CENTERED:
            return centered_eval(x...);

          default:
            return eval(x...);
        }
      }

      template<typename... Args>
      typename T::Domain eval(const Args&... x) const
      {
        check_valid_inputs(x...);
        auto x_ = eval_(x...);

        if(x_.da.size() == 0) // if the centered form is not available for this expression
          return natural_eval(x...);

        auto flatten_x = cart_prod(x...);

        if constexpr(std::is_same_v<typename T::Domain,Interval>)
          return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0]);
        else
          return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0));
      }

      template<typename... Args>
      typename T::Domain natural_eval(const Args&... x) const
      {
        check_valid_inputs(x...);
        return eval_(x...).a;
      }

      template<typename... Args>
      typename T::Domain centered_eval(const Args&... x) const
      {
        check_valid_inputs(x...);
        auto x_ = eval_(x...);
        auto flatten_x = cart_prod(x...);

        if constexpr(std::is_same_v<typename T::Domain,Interval>)
          return x_.m + (x_.da*(flatten_x-flatten_x.mid()))[0];
        else
          return x_.m + (x_.da*(flatten_x-flatten_x.mid())).col(0);
      }

      template<typename... Args>
      auto diff(const Args&... x) const
      {
        check_valid_inputs(x...);
        return eval_(x...).da;
      }

      friend std::ostream& operator<<(std::ostream& os, const AnalyticFunction<T>& f)
      {
        if constexpr(std::is_same_v<typename T::Domain,Interval>) 
          os << "scalar function";
        else if constexpr(std::is_same_v<typename T::Domain,IntervalVector>) 
          os << "vector function";
        return os;
      }

    protected:

      template<typename Y>
      friend class CtcInverse;

      template<typename D>
      void add_value_to_arg_map(ValuesMap& v, const D& x, size_t i) const
      {
        assert(i >= 0 && i < this->args().size());
        assert_release(size_of(x) == this->args()[i]->size() && "provided arguments do not match function inputs");

        IntervalMatrix d = IntervalMatrix::zeros(size_of(x), this->args().total_size());
        
        size_t p = 0, j = 0;
        for( ; j < i ; j++)
          p += this->args()[j]->size();

        for(size_t k = p ; k < p+size_of(x) ; k++)
          d(k-p,k) = 1.;

        using D_DOMAIN = typename Wrapper<D>::Domain;

        v[this->args()[i]->unique_id()] = 
          std::make_shared<D_DOMAIN>(typename D_DOMAIN::Domain(x).mid(), x, d, true);
      }

      template<typename... Args>
      void fill_from_args(ValuesMap& v, const Args&... x) const
      {
        size_t i = 0;
        (add_value_to_arg_map(v, x, i++), ...);
      }

      template<typename D>
      void intersect_value_from_arg_map(const ValuesMap& v, D& x, size_t i) const
      {
        assert(v.find(this->args()[i]->unique_id()) != v.end() && "argument cannot be found");
        x &= std::dynamic_pointer_cast<typename Wrapper<D>::Domain>(v.at(this->args()[i]->unique_id()))->a;
      }

      template<typename... Args>
      void intersect_from_args(const ValuesMap& v, Args&... x) const
      {
        size_t i = 0;
        (intersect_value_from_arg_map(v, x, i++), ...);
      }

      template<typename... Args>
      auto eval_(const Args&... x) const
      {
        ValuesMap v;

        if constexpr(sizeof...(Args) == 0)
          return this->expr()->fwd_eval(v, 0);

        else
        {
          fill_from_args(v, x...);
          return this->expr()->fwd_eval(v, cart_prod(x...).size()); // todo: improve size computation
        }
      }

      template<typename... Args>
      void check_valid_inputs(const Args&... x) const
      {
        size_t n = 0;
        ((n += size_of(x)), ...);

        assert_release(this->_args.total_size() == n && 
          "Invalid arguments: wrong number of input arguments");
      }
  };

}