/** 
 *  \file codac2_SetExpr.h
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
#include <functional>
#include "codac2_ExprBase.h"
#include "codac2_Domain.h"
#include "codac2_FunctionArgsList.h"
#include "codac2_set_operators.h"

namespace codac2
{
  using SetValuesMap = std::map<ExprID,std::shared_ptr<SepBase>>;

  class SetExpr : public ExprBase
  {
    public:

      SetExpr& operator=(const SetExpr& x) = delete;
      virtual std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& x) const = 0;
      virtual std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& x) const = 0;

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

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<X...>::replace_arg(old_arg_id, new_expr);
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

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& values) const
      {
        return std::apply(
          [this,&values,&args](auto &&... x)
          {
            return C::create_ctc(x->create_ctc(args,values)...);
          },
        this->_x);
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& values) const
      {
        return std::apply(
          [this,&values,&args](auto &&... x)
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

      SetOperationExpr(std::shared_ptr<SetExpr> x, const std::vector<Index>& proj_indices, double eps)
        : OperationExprBase<SetExpr>(x), _proj_indices(proj_indices), _y(nullptr), _eps(eps)
      { }

      SetOperationExpr(std::shared_ptr<SetExpr> x, const std::vector<Index>& proj_indices, const IntervalVector& y, double eps)
        : OperationExprBase<SetExpr>(x), _proj_indices(proj_indices), _y(std::make_shared<IntervalVector>(y)), _eps(eps)
      { }

      SetOperationExpr(const SetOperationExpr& e)
        : OperationExprBase<SetExpr>(e), _proj_indices(e._proj_indices), _y(e._y), _eps(e._eps)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<ProjSetOp,SetExpr>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<SetExpr>::replace_arg(old_arg_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& values) const
      {
        throw std::logic_error("CtcProj not yet available");
        return nullptr;
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& values) const
      {
        if(_y)
          return ProjSetOp::create_sep(std::get<0>(this->_x)->create_sep(args,values), _proj_indices, *_y, _eps);
        else
          return ProjSetOp::create_sep(std::get<0>(this->_x)->create_sep(args,values), _proj_indices, _eps);
      }

    protected:

      const std::vector<Index> _proj_indices;
      const std::shared_ptr<IntervalVector> _y;
      const double _eps;
  };

  template<>
  class SetOperationExpr<InverseSetOp,SetExpr> : public SetExpr, public OperationExprBase<SetExpr>
  {
    public:

      SetOperationExpr(const AnalyticFunction<VectorType>& f, std::shared_ptr<SetExpr> x)
        : OperationExprBase<SetExpr>(x), _f(f)
      { }

      SetOperationExpr(const SetOperationExpr& e)
        : OperationExprBase<SetExpr>(e), _f(e._f)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<InverseSetOp,SetExpr>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<SetExpr>::replace_arg(old_arg_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& values) const
      {
        return InverseSetOp::create_ctc(_f, std::get<0>(this->_x)->create_ctc(args,values));
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& values) const
      {
        return InverseSetOp::create_sep(_f, std::get<0>(this->_x)->create_sep(args,values));
      }

    protected:

      const AnalyticFunction<VectorType> _f;
  };

  template<>
  class SetOperationExpr<ActionSetOp,SetExpr> : public SetExpr, public OperationExprBase<SetExpr>
  {
    public:

      SetOperationExpr(const OctaSym& a, std::shared_ptr<SetExpr> x)
        : OperationExprBase<SetExpr>(x), _a(a)
      { }

      SetOperationExpr(const SetOperationExpr& e)
        : OperationExprBase<SetExpr>(e), _a(e._a)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetOperationExpr<ActionSetOp,SetExpr>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<SetExpr>::replace_arg(old_arg_id, new_expr);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& values) const
      {
        return ActionSetOp::create_ctc(_a, std::get<0>(this->_x)->create_ctc(args,values));
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& values) const
      {
        return ActionSetOp::create_sep(_a, std::get<0>(this->_x)->create_sep(args,values));
      }

    protected:

      const OctaSym _a;
  };
}