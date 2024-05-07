/** 
 *  \file
 *  FunctionBase class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include "codac2_ExprBase.h"
#include "codac2_analytic_variables.h"
#include "codac2_FunctionArgsList.h"

namespace codac2
{
  template<typename E>
  class FunctionBase
  {
    public:

      FunctionBase(const std::vector<std::reference_wrapper<VarBase>>& args, const std::shared_ptr<E>& y)
        : _y(std::dynamic_pointer_cast<E>(y->copy())), _args(args)
      { }

      FunctionBase(const FunctionBase<E>& f)
        : _y(f.expr()), _args(f.args()) // todo: keep this dynamic_pointer_cast?
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

      template<typename... F>
      std::shared_ptr<E> operator()(std::shared_ptr<F>... x) const
      {
        auto expr_copy = expr()->copy();
        size_t i = 0;
        (expr_copy->replace_expr(_args[i++]->unique_id(), x), ...);
        return std::dynamic_pointer_cast<E>(expr_copy);
      }

      template<typename A, typename = typename std::enable_if<
          std::is_base_of_v<VarBase,A>
        >::type>
      std::shared_ptr<E> operator()(const A& x) const
      {
        return operator()(x.copy());
      }

    protected:

      const std::shared_ptr<E> _y;
      const FunctionArgsList _args;
  };
}