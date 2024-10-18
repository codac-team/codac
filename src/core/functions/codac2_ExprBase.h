/** 
 *  \file codac2_ExprBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>
#include <utility>
#include "codac2_Domain.h"
#include "codac2_directed_ctc.h"

namespace codac2
{
  class ExprBase;
  class VarBase;

  class ExprID
  {
    public:

      ExprID();
      ExprID(const ExprID& i) = default;
      size_t id() const;
      bool operator==(const ExprID& i) const;
      bool operator<(const ExprID& i) const;

    protected:

      const size_t _id;
      static size_t _id_counter;
  };

  class ExprBase : public std::enable_shared_from_this<ExprBase>
  {
    public:

      ExprBase();
      virtual std::shared_ptr<ExprBase> copy() const = 0;
      virtual void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr) = 0;

      const ExprID& unique_id() const;
      bool operator==(const ExprBase& e) const;
      virtual ~ExprBase() = default;

    protected:

      const ExprID _unique_id;
  };

  template<typename... X>
  class OperationExprBase
  {
    public:

      OperationExprBase(std::shared_ptr<X>... x)
        : _x(std::make_tuple((x)...))
      { }

      OperationExprBase(const OperationExprBase<X...>& e)
        : _x(e._x)
      {
        std::apply(
          [](auto &&... x)
          {
            ((x = _copy(x)), ...);
          }, _x);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        std::apply(
          [old_expr_id,new_expr](auto &&... x)
          {
            (__replace_single_expr(x,old_expr_id,new_expr), ...);
          }, _x);
      }

    protected:

      template<typename X_>
      static std::shared_ptr<X_> _copy(const std::shared_ptr<X_>& x)
      {
        return std::dynamic_pointer_cast<X_>(x->copy());
      }

      template<typename D>
      static void __replace_single_expr(std::shared_ptr<D>& x, const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(x->unique_id() == old_expr_id)
        {
          assert(std::dynamic_pointer_cast<VarBase>(x) && "this subexpr should be some variable");
          x = std::dynamic_pointer_cast<D>(new_expr->copy() /* todo: keep this copy? */);
        }
        else
          x->replace_expr(old_expr_id, new_expr);
      }

      std::tuple<std::shared_ptr<X>...> _x;
  };
}