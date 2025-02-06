/** 
 *  \file codac2_Traj_operator.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_SampledTraj.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_ValueType.h"
#include "codac2_Wrapper.h"

namespace codac2
{
  template<typename T>
  struct TrajectoryOp
  {
    static typename Wrapper<T>::Domain fwd(const TrajBase<T>& x1, const Interval& x2)
    {
      return x1(x2);
    }

    static typename ValueType<T>::Type fwd(const TrajBase<T>& x1, const ScalarType& x2)
    {
      IntervalMatrix d(x1.size(),x2.da.cols());

      return {
        fwd(x1,x2.m),
        fwd(x1,x2.a),
        d,
        x2.def_domain && x1.tdomain().is_superset(x2.m)
      };
    }

    static void bwd(
      [[maybe_unused]] const TrajBase<T>& x1,
      [[maybe_unused]] const typename Wrapper<T>::Domain& y,
      [[maybe_unused]] Interval& x2)
    {
      // todo
    }
  };

  template<typename T>
  class AnalyticOperationExpr<TrajectoryOp<T>,typename ValueType<T>::Type,ScalarType>
    : public AnalyticExpr<typename ValueType<T>::Type>, public OperationExprBase<AnalyticExpr<ScalarType>>
  {
    public:

      using O = typename ValueType<T>::Type;

      AnalyticOperationExpr(const TrajBase<T>& x1, const ScalarExpr& x2)
        : OperationExprBase<AnalyticExpr<ScalarType>>(x2), _x1(x1)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<TrajectoryOp<T>,O,ScalarType>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<ScalarType>>::replace_expr(old_expr_id, new_expr);
      }
      
      O fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        return AnalyticExpr<O>::init_value(
          v, TrajectoryOp<T>::fwd(_x1, std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval)));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        TrajectoryOp<T>::bwd(_x1, AnalyticExpr<O>::value(v).a, std::get<0>(this->_x)->value(v).a);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

    protected:

      const TrajBase<T>& _x1;
  };
  
  template<typename T>
  AnalyticFunction<typename ValueType<T>::Type> TrajBase<T>::as_function() const
  {
    using D = typename ValueType<T>::Type;

    ScalarVar t;
    return {{t},
      AnalyticExprWrapper<D>(std::make_shared<
        AnalyticOperationExpr
        <TrajectoryOp<T>, D, ScalarType>>(*this,t))
    };
  }
}