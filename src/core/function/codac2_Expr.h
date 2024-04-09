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
#include "codac2_Expr.h"
#include "codac2_Domain.h"
#include "codac2_DirectedCtc.h"

namespace codac2
{
  class ExprBase
  {
    public:

      virtual ~ExprBase() = default;
  };

  using ValuesMap = std::map<std::shared_ptr<const ExprBase>,std::shared_ptr<Domain>>;

  template<typename T>
  class Expr : public ExprBase, public std::enable_shared_from_this<Expr<T>>
  {
    public:

      Expr<T>& operator=(const Expr<T>& x) = delete;
      virtual std::shared_ptr<Expr<T>> copy() = 0;

      virtual T fwd_eval(ValuesMap& v) const = 0;
      virtual void bwd_eval(ValuesMap& v) const = 0;
      virtual void replace_expr(const std::shared_ptr<const ExprBase>& old_expr, const std::shared_ptr<ExprBase>& new_expr) = 0;

      T init_value(ValuesMap& v, const T& x) const
      {
        auto it = v.find(this->shared_from_this());

        if(it == v.end())
        {
          v[this->shared_from_this()] = std::make_shared<T>(x);
          return x;
        }

        *std::dynamic_pointer_cast<T>(it->second) &= x;
        return x;
      }

      T& value(ValuesMap& v) const
      {
        assert(v.find(this->shared_from_this()) != v.end() && "argument cannot be found");
        return *std::dynamic_pointer_cast<T>(v.at(this->shared_from_this()));
      }
  };

  template<typename C,typename Y,typename... X>
  class OperationExpr : public Expr<Y>
  {
    public:

      OperationExpr(std::shared_ptr<Expr<X>>... x)
        : _x(std::make_tuple((x)...))
      { }

      OperationExpr(const OperationExpr<C,Y,X...>& e)
        : _x(e._x)
      {
        std::apply(
          [](auto &&... x)
          {
            ((x = x->copy()), ...);
          }, _x);
      }

      std::shared_ptr<Expr<Y>> copy()
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
      static void __replace_single_expr(std::shared_ptr<Expr<D>>& x, const std::shared_ptr<const ExprBase>& old_expr, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(x == old_expr)
          x = std::dynamic_pointer_cast<Expr<D>>(new_expr);
        else
          x->replace_expr(old_expr, new_expr);
      }

      void replace_expr(const std::shared_ptr<const ExprBase>& old_expr, const std::shared_ptr<ExprBase>& new_expr)
      {
        std::apply(
          [&old_expr,&new_expr](auto &&... x)
          {
            (__replace_single_expr(x,old_expr,new_expr), ...);
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
        : _x1(e._x1->copy()), _i(e._i)
      { }

      std::shared_ptr<Expr<Interval>> copy()
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

      void replace_expr(const std::shared_ptr<const ExprBase>& old_expr, const std::shared_ptr<ExprBase>& new_expr)
      {
        if(_x1 == old_expr)
          _x1 = std::dynamic_pointer_cast<Expr<IntervalVector>>(new_expr);
        else
          _x1->replace_expr(old_expr, new_expr);
      }

    protected:

      std::shared_ptr<Expr<IntervalVector>> _x1;
      const size_t _i;
  };
}