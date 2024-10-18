/** 
 *  \file codac2_AnalyticExpr.h
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
#include "codac2_ExprBase.h"
#include "codac2_Domain.h"
#include "codac2_directed_ctc.h"
#include "codac2_FunctionArgsList.h"

namespace codac2
{
  using ValuesMap = std::map<ExprID,std::shared_ptr<OpValueBase>>;

  template<typename T>
  class AnalyticExpr : public ExprBase
  {
    public:

      AnalyticExpr<T>& operator=(const AnalyticExpr<T>& x) = delete;

      virtual T fwd_eval(ValuesMap& v, size_t total_input_size) const = 0;
      virtual void bwd_eval(ValuesMap& v) const = 0;

      T init_value(ValuesMap& v, const T& x) const
      {
        auto it = v.find(unique_id());

        if(it == v.end())
        {
          v[unique_id()] = std::make_shared<T>(x);
          return x;
        }

        *std::dynamic_pointer_cast<T>(it->second) = x;
        return *std::dynamic_pointer_cast<T>(it->second);
      }

      T& value(ValuesMap& v) const
      {
        assert(v.find(unique_id()) != v.end() && "argument cannot be found");
        return *std::dynamic_pointer_cast<T>(v[unique_id()]);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const = 0;
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

      Y fwd_eval(ValuesMap& v, size_t total_input_size) const
      {
        return std::apply(
          [this,&v,total_input_size](auto &&... x)
          {
            return AnalyticExpr<Y>::init_value(v, C::fwd(x->fwd_eval(v, total_input_size)...));
          },
        this->_x);
      }

      void bwd_eval(ValuesMap& v) const
      {
        auto y = AnalyticExpr<Y>::value(v);

        std::apply([&v,y](auto &&... x)
        {
          C::bwd(y.a, x->value(v).a...);
        }, this->_x);

        std::apply([&v](auto &&... x)
        {
          (x->bwd_eval(v), ...);
        }, this->_x);
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
      
      ScalarOpValue fwd_eval(ValuesMap& v, size_t total_input_size) const
      {
        return AnalyticExpr<ScalarOpValue>::init_value(
          v, ComponentOp::fwd(std::get<0>(this->_x)->fwd_eval(v, total_input_size), _i));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        ComponentOp::bwd(AnalyticExpr<ScalarOpValue>::value(v).a, std::get<0>(this->_x)->value(v).a, _i);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

    protected:

      const size_t _i;
  };

  template<>
  class AnalyticOperationExpr<SubvectorOp,VectorOpValue,VectorOpValue> : public AnalyticExpr<VectorOpValue>, public OperationExprBase<AnalyticExpr<VectorOpValue>>
  {
    public:

      AnalyticOperationExpr(const std::shared_ptr<AnalyticExpr<VectorOpValue>>& x1, size_t i, size_t j)
        : OperationExprBase<AnalyticExpr<VectorOpValue>>(x1), _i(i), _j(j)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr& e)
        : OperationExprBase<AnalyticExpr<VectorOpValue>>(e), _i(e._i), _j(e._j)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<SubvectorOp,VectorOpValue,VectorOpValue>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<VectorOpValue>>::replace_expr(old_expr_id, new_expr);
      }
      
      VectorOpValue fwd_eval(ValuesMap& v, size_t total_input_size) const
      {
        return AnalyticExpr<VectorOpValue>::init_value(
          v, SubvectorOp::fwd(std::get<0>(this->_x)->fwd_eval(v, total_input_size), _i, _j));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        SubvectorOp::bwd(AnalyticExpr<VectorOpValue>::value(v).a, std::get<0>(this->_x)->value(v).a, _i, _j);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

    protected:

      const size_t _i, _j;
  };
}