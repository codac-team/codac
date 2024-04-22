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
        ValuesMap v;
        fill_from_args(v, x...);
        return this->expr()->fwd_eval(v,this->args()).a;
      }

    protected:

      template<typename Y>
      friend class CtcInverse;

      template<typename D>
      void add_value_to_arg_map(ValuesMap& v, const D& x, size_t i) const
      {
        size_t p = 0;
        for(size_t j = 0 ; j < i ; j++)
          p += this->args()[j]->size();

        v[this->args()[i]->unique_id()] = std::make_shared<typename Wrapper<D>::Domain>(
          x, this->args().total_size(), p, p+this->args()[i]->size()-1
        );
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
  };
}