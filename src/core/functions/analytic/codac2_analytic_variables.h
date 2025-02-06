/** 
 *  \file codac2_analytic_variables.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <iostream>
#include "codac2_AnalyticExpr.h"
#include "codac2_VarBase.h"
#include "codac2_component.h"

namespace codac2
{
  template<typename T>
  struct AnalyticExprWrapper;

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

      T fwd_eval(ValuesMap& v, [[maybe_unused]] Index total_input_size, [[maybe_unused]] bool natural_eval) const
      {
        return AnalyticExpr<T>::value(v);
      }
      
      void bwd_eval([[maybe_unused]] ValuesMap& v) const
      { }

      void replace_expr([[maybe_unused]] const ExprID& old_expr_id, [[maybe_unused]] const std::shared_ptr<ExprBase>& new_expr)
      { }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        for(const auto& xi : args)
          if(xi->unique_id() == this->unique_id())
            return true;
        return false;
      }
  };

  class ScalarVar : public AnalyticVarExpr<ScalarType>
  {
    public:

      ScalarVar();

      std::shared_ptr<VarBase> arg_copy() const;
      std::shared_ptr<ExprBase> copy() const;
      Index size() const;

      AnalyticExprWrapper<ScalarType> operator-() const;
  };

  class VectorVar : public AnalyticVarExpr<VectorType>
  {
    public:

      explicit VectorVar(Index n);

      std::shared_ptr<VarBase> arg_copy() const;
      std::shared_ptr<ExprBase> copy() const;
      Index size() const;

      AnalyticExprWrapper<ScalarType> operator[](Index i) const;
      AnalyticExprWrapper<VectorType> subvector(Index i, Index j) const;

    protected:

      Index _n;
  };
}