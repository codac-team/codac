/** 
 *  \file codac2_FunctionBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_ExprBase.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionArgsList.h"

// todo: remove this:
#include "codac2_analytic_constants.h"

namespace codac2
{
  template<typename E>
  class FunctionBase
  {
    public:

      FunctionBase(const std::vector<std::reference_wrapper<VarBase>>& args, const std::shared_ptr<E>& y)
        : FunctionBase(FunctionArgsList(args),y)
      { }

      FunctionBase(const FunctionArgsList& args, const std::shared_ptr<E>& y)
        : _y(std::dynamic_pointer_cast<E>(y->copy())), _args(args)
      { }

      FunctionBase(const FunctionBase<E>& f)
        : _y(std::dynamic_pointer_cast<E>(f.expr()->copy())), _args(f.args()) // todo: keep this dynamic_pointer_cast?
      { }
      
      virtual ~FunctionBase()
      { }

      const FunctionArgsList& args() const
      {
        return _args;
      }

      const std::shared_ptr<E>& expr() const
      {
        return _y;
      }

      template<typename... X>
      std::shared_ptr<E> operator()(const X&... x) const
      {
        auto expr_copy = expr()->copy();
        size_t i = 0;
        (expr_copy->replace_expr(_args[i++]->unique_id(), this->__get_copy(x)), ...);
        assert_release(i == this->args().size() && 
          "Invalid arguments: wrong number of input arguments");
        return std::dynamic_pointer_cast<E>(expr_copy);
      }

      std::shared_ptr<E> operator()(const std::vector<std::shared_ptr<ExprBase>>& x) const
      {
        assert_release(x.size() == this->args().size() && 
          "Invalid arguments: wrong number of input arguments");
        auto expr_copy = expr()->copy();
        for(size_t i = 0 ; i < x.size() ; i++)
          expr_copy->replace_expr(_args[i]->unique_id(), x[i]->copy());
        return std::dynamic_pointer_cast<E>(expr_copy);
      }

      Index input_size() const
      {
        return this->_args.total_size();
      }

    protected:

      template<typename X>
      std::shared_ptr<X> __get_copy(std::shared_ptr<X> x) const
      {
        return std::dynamic_pointer_cast<X>(x->copy());
      }

      template<typename X>
      std::shared_ptr<ExprBase> __get_copy(const AnalyticExprWrapper<X>& x) const
      {
        return x->copy();
      }

      template<typename A>
      auto __get_copy(const A& x) const
      {
        if constexpr(std::is_base_of_v<VarBase,A>)
          return std::dynamic_pointer_cast<A>(x.copy());
        else
        {
          // todo: make this generic (analytic / set):
          return const_value(x);
        }
      }

      const std::shared_ptr<E> _y;
      const FunctionArgsList _args;
  };
}