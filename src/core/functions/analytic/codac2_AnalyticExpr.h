/** 
 *  \file codac2_AnalyticExpr.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>
#include <utility>
#include "codac2_ExprBase.h"
#include "codac2_Domain.h"
#include "codac2_FunctionArgsList.h"
#include "codac2_AnalyticType.h"

namespace codac2
{
  using ValuesMap = std::map<ExprID,std::shared_ptr<AnalyticTypeBase>>;

  template<typename T>
  class AnalyticExpr : public ExprBase
  {
    public:

      virtual T fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const = 0;
      virtual void bwd_eval(ValuesMap& v) const = 0;
      virtual std::pair<Index,Index> output_shape() const = 0;

      const T& init_value(ValuesMap& v, const T& x) const
      {
        auto& p = v[unique_id()];

        if(!p)
        {
          p = std::make_shared<T>(x);
          return *static_cast<T*>(p.get());
        }

        else
        {
          auto pt = std::dynamic_pointer_cast<T>(p);
          assert(pt && "Type mismatch in ValuesMap for this ExprID");
          *pt = x;
          return *pt;
        }
      }

      T& value(ValuesMap& v) const
      {
        auto it = v.find(unique_id());
        assert(it != v.end() && "argument cannot be found");
        auto p = std::dynamic_pointer_cast<T>(it->second);
        assert(p && "Type mismatch in ValuesMap for this ExprID");
        return *p;
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const = 0;
      virtual std::string str(bool in_parentheses = false) const = 0;
      virtual bool is_str_leaf() const = 0;
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

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<X>...>::replace_arg(old_arg_id, new_expr);
      }

      Y fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        return std::apply(
          [this,&v,total_input_size,natural_eval](auto &&... x)
          {
            if(natural_eval)
              return AnalyticExpr<Y>::init_value(v,
                C::fwd_natural(x->fwd_eval(v, total_input_size, natural_eval)...));

            else
              return AnalyticExpr<Y>::init_value(v,
                C::fwd_centered(x->fwd_eval(v, total_input_size, natural_eval)...));
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

      virtual std::string str(bool in_parentheses = false) const
      {
        std::string s = std::apply([](auto &&... x) {
          return C::str(x...);
        }, this->_x);
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return false;
      }

      std::pair<Index,Index> output_shape() const
      {
        std::pair<Index,Index> s;
        std::apply([&s](auto &&... x)
        {
          s = C::output_shape(x...);
        }, this->_x);
        return s;
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        bool b = true;

        std::apply([&b,&args](auto &&... x)
        {
          ((b &= x->belongs_to_args_list(args)), ...);
        }, this->_x);

        return b;
      }
  };
}
