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
#include "codac2_ExprBase.h"
#include "codac2_Domain.h"
#include "codac2_DirectedCtc.h"
#include "codac2_FunctionArgsList.h"

namespace codac2
{
  using ValuesMap = std::map<ExprID,std::shared_ptr<OpValueBase>>;

  template<typename T>
  class AnalyticExpr : public ExprBase
  {
    public:

      AnalyticExpr<T>& operator=(const AnalyticExpr<T>& x) = delete;

      virtual T fwd_eval(ValuesMap& v, const FunctionArgsList& f_args) const = 0;
      virtual void bwd_eval(ValuesMap& v) const = 0;

      T init_value(ValuesMap& v, const T& x) const
      {
        auto it = v.find(unique_id());

        if(it == v.end())
        {
          v[unique_id()] = std::make_shared<T>(x);
          return x;
        }

        std::dynamic_pointer_cast<T>(it->second)->a &= x.a;
        return x;
      }

      T& value(ValuesMap& v) const
      {
        assert(v.find(unique_id()) != v.end() && "argument cannot be found");
        return *std::dynamic_pointer_cast<T>(v[unique_id()]);
      }
  };

  template<typename C,typename Y,typename... X>
  class AnalyticOperationExpr : public AnalyticExpr<Y>, public OperationExprBase<AnalyticExpr<X>...>
  {
    public:

      AnalyticOperationExpr(std::shared_ptr<AnalyticExpr<X>>... x)
        : OperationExprBase<AnalyticExpr<X>...>(x...)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr<C,Y,X...>& e)
        : OperationExprBase<AnalyticExpr<X>...>(e)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<C,Y,X...>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<X>...>::replace_expr(old_expr_id, new_expr);
      }

      static Y __fwd_eval_from_tuple(ValuesMap& v, const FunctionArgsList& f_args, std::shared_ptr<AnalyticExpr<X>>... x)
      {
        return C::fwd(x->fwd_eval(v,f_args)...);
      }

      Y fwd_eval(ValuesMap& v, const FunctionArgsList& f_args) const
      {
        return AnalyticExpr<Y>::init_value(v,
          std::apply(__fwd_eval_from_tuple, std::tuple_cat(std::tie(v), std::make_tuple(f_args), this->_x)));
      }
      
      static void __bwd_from_tuple(const Y& y, ValuesMap& v, std::shared_ptr<AnalyticExpr<X>>... x)
      {
        C::bwd(y.a, x->value(v).a...);
      }

      void bwd_eval(ValuesMap& v) const
      {
        std::apply(__bwd_from_tuple, std::tuple_cat(
          std::tie(AnalyticExpr<Y>::value(v)), std::tie(v), this->_x));

        std::apply(
          [&v](auto &&... x)
          {
            (x->bwd_eval(v), ...);
          }, this->_x);
      }
  };

  template<>
  class AnalyticOperationExpr<ComponentOp,ScalarOpValue,VectorOpValue> : public AnalyticExpr<ScalarOpValue>, public OperationExprBase<AnalyticExpr<VectorOpValue>>
  {
    public:

      AnalyticOperationExpr(const std::shared_ptr<AnalyticExpr<VectorOpValue>>& x1, size_t i)
        : OperationExprBase<AnalyticExpr<VectorOpValue>>(x1), _i(i)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr& e)
        : OperationExprBase<AnalyticExpr<VectorOpValue>>(e), _i(e._i)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarOpValue,VectorOpValue>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<VectorOpValue>>::replace_expr(old_expr_id, new_expr);
      }
      
      ScalarOpValue fwd_eval(ValuesMap& v, const FunctionArgsList& f_args) const
      {
        return AnalyticExpr<ScalarOpValue>::init_value(v, ComponentOp::fwd(std::get<0>(this->_x)->fwd_eval(v,f_args), _i));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        ComponentOp::bwd(AnalyticExpr<ScalarOpValue>::value(v).a, std::get<0>(this->_x)->value(v).a, _i);
        std::get<0>(this->_x)->bwd_eval(v);
      }

    protected:

      const size_t _i;
  };
}