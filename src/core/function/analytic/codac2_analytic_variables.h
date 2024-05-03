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

#include "codac2_AnalyticExpr.h"
#include "codac2_VarBase.h"

namespace codac2
{
  template<typename T>
  struct Wrapper
  { };

  template<>
  struct Wrapper<int> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<size_t> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<double> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<Interval> {
    using Domain = ScalarOpValue;
  };

  template<>
  struct Wrapper<Vector> {
    using Domain = VectorOpValue;
  };

  template<>
  struct Wrapper<IntervalVector> {
    using Domain = VectorOpValue;
  };

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
        T y = AnalyticExpr<T>::value(v);
        y.compute_centered_form(flatten_x);
        return y;
      }
      
      void bwd_eval(ValuesMap& v) const
      { }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }
      
      operator std::shared_ptr<AnalyticExpr<T>>()
      {
        return std::dynamic_pointer_cast<AnalyticExpr<T>>(this->copy());
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
  };

  class VectorVar : public AnalyticVarExpr<VectorOpValue>
  {
    public:

      VectorVar(size_t n)
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

      std::shared_ptr<AnalyticExpr<ScalarOpValue>> operator[](size_t i)
      {
        assert(i >= 0 && i < _n);
        return std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarOpValue,VectorOpValue>>(
          std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(this->copy()), i);
      }

    protected:

      size_t _n;
  };
}