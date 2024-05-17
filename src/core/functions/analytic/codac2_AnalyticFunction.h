/** 
 *  \file
 *  AnalyticFunction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include "codac2_AnalyticExpr.h"
#include "codac2_Domain.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionBase.h"

namespace codac2
{
  template<typename T, typename = typename std::enable_if<
      std::is_base_of_v<OpValueBase,T>
    >::type>
  class AnalyticFunction : public FunctionBase<AnalyticExpr<T>>
  {
    public:

      AnalyticFunction(const std::vector<std::reference_wrapper<VarBase>>& args, const std::shared_ptr<AnalyticExpr<T>>& y)
        : FunctionBase<AnalyticExpr<T>>(args, y)
      { }

      AnalyticFunction(const AnalyticFunction<T>& f)
        : FunctionBase<AnalyticExpr<T>>(f)
      { }

      template<typename... Args>
      typename T::Domain eval(const Args&... x) const
      {
        check_valid_inputs(x...);
        return eval_(x...).a;
      }

      template<typename... Args>
      typename T::Domain eval_centered(const Args&... x) const
      {
        check_valid_inputs(x...);
        auto x_ = eval_(x...);
        auto flatten_x = cart_prod(x...);
        
        if constexpr(std::is_same_v<typename T::Domain,Interval>)
          return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid().template cast<Interval>()))[0]);
        else
          return x_.a & (x_.m + (x_.da*(flatten_x-flatten_x.mid().template cast<Interval>())).col(0));
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
        assert(x.size() == this->args()[i]->size());

        IntervalMatrix d = IntervalMatrix::zeros(this->args().total_size(), x.size());
        
        size_t p = 0, j = 0;
        for(size_t j = 0 ; j < i ; j++)
          p += this->args()[j]->size();

        for(size_t k = p ; k < p+x.size() ; k++)
          d(k,j++) = 1.;

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
        fill_from_args(v, x...);
        return this->expr()->fwd_eval(v, this->args(), cart_prod(x...));
      }

      template<typename... Args>
      void check_valid_inputs(const Args&... x) const
      {
        size_t n = 0;
        ((n += x.size()), ...);
        
        if(this->_args.total_size() != n)
        {
          std::cerr << "Wrong function input: check the number and dimension of input arguments" << std::endl;
          std::cerr << "  Required inputs: total_size=" << this->_args.total_size() << std::endl;
          std::cerr << "  Provided inputs: total_size=" << n << std::endl;
          throw std::exception();
        }
      }
  };
}