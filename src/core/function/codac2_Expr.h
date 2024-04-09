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
#include "codac2_Expr.h"
#include "codac2_Domain.h"
#include "codac2_DirectedCtc.h"

namespace codac2
{
  class ExprBase;

  class ExprID
  {
    public:

      ExprID()
        : _id(this)
      { }

      bool operator==(const ExprID i) const
      {
        return _id == i._id;
      }

      bool operator<(const ExprID i) const
      {
        return _id < i._id;
      }

    protected:

      ExprID *_id;
  };

  class ExprBase : public std::enable_shared_from_this<ExprBase>
  {
    public:

      ExprBase()
        : _unique_id(/* creating id from object address */)
      { }

      virtual std::shared_ptr<ExprBase> copy() const = 0;
      virtual void replace_expr(ExprID old_expr_id, const std::shared_ptr<ExprBase>& new_expr) = 0;

      ExprID unique_id() const
      {
        return _unique_id;
      }

      bool operator==(const ExprBase& e) const
      {
        return _unique_id == e._unique_id;
      }

      virtual ~ExprBase() = default;

    protected:

      ExprID _unique_id;
  };

  using ValuesMap = std::map<ExprID,std::shared_ptr<Domain>>;

  template<typename T>
  class Expr : public ExprBase
  {
    public:

      Expr<T>& operator=(const Expr<T>& x) = delete;

      virtual T fwd_eval(ValuesMap& v) const = 0;
      virtual void bwd_eval(ValuesMap& v) const = 0;

      T init_value(ValuesMap& v, const T& x) const
      {
        auto it = v.find(unique_id());

        if(it == v.end())
        {
          v[unique_id()] = std::make_shared<T>(x);
          return x;
        }

        *std::dynamic_pointer_cast<T>(it->second) &= x;
        return x;
      }

      T& value(ValuesMap& v) const
      {
        assert(v.find(unique_id()) != v.end() && "argument cannot be found");
        return *std::dynamic_pointer_cast<T>(v.at(unique_id()));
      }
  };

  template<typename C,typename Y,typename... X>
  class OperationExpr : public Expr<Y>
  {
    public:

      OperationExpr(std::shared_ptr<Expr<X>>... x)
        : _x(std::make_tuple((x)...))
      { }

      template<typename X_>
      static std::shared_ptr<codac2::Expr<X_>> _copy(const std::shared_ptr<codac2::Expr<X_>>& x)
      {
        return std::dynamic_pointer_cast<codac2::Expr<X_>>(x->copy());
      }

      OperationExpr(const OperationExpr<C,Y,X...>& e)
        : _x(e._x)
      {
        std::apply(
          [](auto &&... x)
          {
            ((x = _copy(x)), ...);
          }, _x);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<OperationExpr<C,Y,X...>>(*this);
      }

      static Y __fwd_eval_from_tuple(ValuesMap& v, std::shared_ptr<Expr<X>>... x)
      {
        return C::fwd_eval(x->fwd_eval(v)...);
      }

      Y fwd_eval(ValuesMap& v) const
      {
        return Expr<Y>::init_value(v,
          std::apply(__fwd_eval_from_tuple, std::tuple_cat(std::tie(v), _x)));
      }
      
      static void __bwd_eval_from_tuple(const Y& y, ValuesMap& v, std::shared_ptr<Expr<X>>... x)
      {
        C::bwd_eval(y, x->value(v)...);
      }

      void bwd_eval(ValuesMap& v) const
      {
        std::apply(__bwd_eval_from_tuple, std::tuple_cat(
          std::tie(Expr<Y>::value(v)), std::tie(v), _x));

        std::apply(
          [&v](auto &&... x)
          {
            (x->bwd_eval(v), ...);
          }, _x);
      }

      template<typename D>
      static void __replace_single_expr(std::shared_ptr<Expr<D>>& x, ExprID old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(x->unique_id() == old_expr_id)
          x = std::dynamic_pointer_cast<Expr<D>>(new_expr);
        else
          x->replace_expr(old_expr_id, new_expr);
      }

      void replace_expr(ExprID old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        std::apply(
          [&old_expr_id,&new_expr](auto &&... x)
          {
            (__replace_single_expr(x,old_expr_id,new_expr), ...);
          }, _x);
      }

    protected:

      std::tuple<std::shared_ptr<Expr<X>>...> _x;
  };

  template<>
  class OperationExpr<CtcComponent,Interval,IntervalVector> : public Expr<Interval>
  {
    public:

      OperationExpr(const std::shared_ptr<Expr<IntervalVector>>& x1, size_t i)
        : _x1(x1), _i(i)
      { }

      OperationExpr(const OperationExpr& e)
        : _x1(std::dynamic_pointer_cast<Expr<IntervalVector>>(e._x1->copy())), _i(e._i)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<OperationExpr<CtcComponent,Interval,IntervalVector>>(*this);
      }
      
      Interval fwd_eval(ValuesMap& v) const
      {
        return Expr<Interval>::init_value(v, CtcComponent::fwd_eval(_x1->fwd_eval(v), _i));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        CtcComponent::bwd_eval(Expr<Interval>::value(v), _x1->value(v), _i);
        _x1->bwd_eval(v);
      }

      void replace_expr(ExprID old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(_x1->unique_id() == old_expr_id)
          _x1 = std::dynamic_pointer_cast<Expr<IntervalVector>>(new_expr);
        else
          _x1->replace_expr(old_expr_id, new_expr);
      }

    protected:

      std::shared_ptr<Expr<IntervalVector>> _x1;
      const size_t _i;
  };
}