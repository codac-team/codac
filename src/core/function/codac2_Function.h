/** 
 *  \file
 *  Function class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include "codac2_Expr.h"
#include "codac2_Domain.h"
#include "codac2_Arg.h"

namespace codac2
{
  template<typename T>
  class CtcFunction;

  template<typename T>
  class Function
  {
    public:

      Function(const Function<T>& f)
        : _args(f._args), _y(f._y->copy())
      { }

      Function(const std::vector<const ArgBase&>& args, const std::shared_ptr<Expr<T>>& y)
        : _y(y)
      {
        for(const auto& arg : args)
          _args.push_back(arg);
      }

      template<typename D>
      void add_value_to_arg_map(ValuesMap& v, const D& x, size_t i) const
      {
        v[_args[i].unique_id()] = std::make_shared<typename Wrapper<D>::Domain>(x);
      }

      template<typename... Args>
      void fill_from_args(ValuesMap& v, const Args&... x) const
      {
        size_t i = 0;
        (add_value_to_arg_map(v, x, i++) , ...);
      }

      template<typename D>
      void intersect_value_from_arg_map(const ValuesMap& v, D& x, size_t i) const
      {
        assert(v.find(_args[i].unique_id()) != v.end() && "argument cannot be found");
        x &= *std::dynamic_pointer_cast<D>(v.at(_args[i].unique_id()));
      }

      template<typename... Args>
      void intersect_from_args(const ValuesMap& v, Args&... x) const
      {
        size_t i = 0;
        (intersect_value_from_arg_map(v, x, i++), ...);
      }

      template<typename... Args>
      T eval(const Args&... x) const
      {
        ValuesMap v;
        fill_from_args(v, x...);
        return _y->fwd_eval(v);
      }

      template<typename... E>
      std::shared_ptr<Expr<T>> operator()(std::shared_ptr<E>... x) const
      {
        auto expr_copy = _y->copy();
        size_t i = 0;
        (expr_copy->replace_expr(_args[i++].unique_id(), x), ...);
        return expr_copy;
      }

      std::shared_ptr<Expr<T>> operator()(const ArgBase& x) const
      {
        return operator()(x.exprbase_ptr());
      }

      const auto& args() const
      {
        return _args;
      }

      std::shared_ptr<Expr<T>> expr() const
      {
        return _y;
      }

      size_t nb_args() const
      {
        return _args.size();
      }

    protected:

      std::vector<ArgBase> _args;
      std::shared_ptr<Expr<T>> _y;
  };

  using ScalarFunction = Function<Interval>;
  using VectorFunction = Function<IntervalVector>;
  using MatrixFunction = Function<IntervalMatrix>;
}