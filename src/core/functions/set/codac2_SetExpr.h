/** 
 *  \file
 *  Expr classes
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include <memory>
#include <utility>
#include <functional>
#include "codac2_ExprBase.h"
#include "codac2_Domain.h"
#include "codac2_FunctionArgsList.h"
#include "codac2_set_operators.h"

namespace codac2
{
  using SetValuesMap = std::map<ExprID,std::shared_ptr<Sep>>;

  class SetExpr : public ExprBase
  {
    public:

      SetExpr& operator=(const SetExpr& x) = delete;
      virtual std::shared_ptr<Sep> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<Sep>>& x) const = 0;

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const = 0;
  };

  template<typename C,typename... X>
  class SetOperationExpr : public SetExpr, public OperationExprBase<X...>
  {
    public:

      SetOperationExpr(std::shared_ptr<X>... x)
        : OperationExprBase<X...>(x...)
      { }

      SetOperationExpr(const SetOperationExpr<C,X...>& e)
        : OperationExprBase<X...>(e)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<C,X...>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<X...>::replace_expr(old_expr_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        bool b = true;

        std::apply([&b,args](auto &&... x)
        {
          ((b &= x->belongs_to_args_list(args)), ...);
        }, this->_x);

        return b;
      }

      std::shared_ptr<Sep> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<Sep>>& values) const
      {
        return std::apply(
          [this,values,args](auto &&... x)
          {
            return C::create_sep(x->create_sep(args,values)...);
          },
        this->_x);
      }
  };

  template<>
  class SetOperationExpr<ProjSetOp,SetExpr> : public SetExpr, public OperationExprBase<SetExpr>
  {
    public:

      SetOperationExpr(std::shared_ptr<SetExpr> x, const IntervalVector& y)
        : OperationExprBase<SetExpr>(x), _y(y)
      { }

      SetOperationExpr(const SetOperationExpr& e)
        : OperationExprBase<SetExpr>(e), _y(e._y)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<ProjSetOp,SetExpr>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<SetExpr>::replace_expr(old_expr_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::shared_ptr<Sep> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<Sep>>& values) const
      {
        return ProjSetOp::create_sep(std::get<0>(this->_x)->create_sep(args,values), _y);
      }

    protected:

      const IntervalVector _y;
  };

  template<>
  class SetOperationExpr<InverseSetOp,SetExpr> : public SetExpr, public OperationExprBase<SetExpr>
  {
    public:

      SetOperationExpr(const AnalyticFunction<VectorOpValue>& f, std::shared_ptr<SetExpr> x)
        : OperationExprBase<SetExpr>(x), _f(f)
      { }

      SetOperationExpr(const SetOperationExpr& e)
        : OperationExprBase<SetExpr>(e), _f(e._f)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<InverseSetOp,SetExpr>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<SetExpr>::replace_expr(old_expr_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::shared_ptr<Sep> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<Sep>>& values) const
      {
        return InverseSetOp::create_sep(_f, std::get<0>(this->_x)->create_sep(args,values));
      }

    protected:

      const AnalyticFunction<VectorOpValue> _f;
  };
}