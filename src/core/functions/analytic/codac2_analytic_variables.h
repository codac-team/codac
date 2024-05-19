/** 
 *  \file
 *  Var class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <iostream>
#include "codac2_AnalyticExpr.h"
#include "codac2_VarBase.h"

namespace codac2
{
  template<typename T>
  class AnalyticVarExpr : public AnalyticExpr<T>, public VarBase
  {
    public:

      AnalyticVarExpr()
      { }

      virtual const ExprID& unique_id() const
      {
        return AnalyticExpr<T>::unique_id();
      }

      T fwd_eval(ValuesMap& v, const FunctionArgsList& f_args, const IntervalVector& flatten_x) const
      {
        return AnalyticExpr<T>::value(v);
      }
      
      void bwd_eval(ValuesMap& v) const
      { }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }
      
      operator std::shared_ptr<AnalyticExpr<T>>() const
      {
        return std::dynamic_pointer_cast<AnalyticExpr<T>>(this->copy());
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        for(const auto& xi : args)
          if(xi->unique_id() == this->unique_id())
            return true;
        return false;
      }
  };

  class ScalarVar : public AnalyticVarExpr<ScalarOpValue>
  {
    public:

      ScalarVar()
      { }

      ScalarVar(const ScalarVar& x)
        : AnalyticVarExpr<ScalarOpValue>(x)
      { }

      std::shared_ptr<VarBase> arg_copy() const
      {
        return std::make_shared<ScalarVar>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ScalarVar>(*this);
      }

      size_t size() const
      {
        return 1;
      }

      std::shared_ptr<AnalyticVarExpr<ScalarOpValue>> operator-() const
      {
        return std::dynamic_pointer_cast<AnalyticVarExpr<ScalarOpValue>>(-*this);
      }
  };

  class VectorVar : public AnalyticVarExpr<VectorOpValue>
  {
    public:

      explicit VectorVar(size_t n)
        : _n(n)
      { }

      VectorVar(const VectorVar& x)
        : AnalyticVarExpr<VectorOpValue>(x), _n(x._n)
      { }

      std::shared_ptr<VarBase> arg_copy() const
      {
        return std::make_shared<VectorVar>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<VectorVar>(*this);
      }

      size_t size() const
      {
        return _n;
      }

      std::shared_ptr<AnalyticExpr<ScalarOpValue>> operator[](size_t i) const
      {
        assert(i >= 0 && i < _n);
        return std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarOpValue,VectorOpValue>>(
          std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(this->copy()), i);
      }

    protected:

      size_t _n;
  };
}