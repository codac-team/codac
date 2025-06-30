/** 
 *  \file codac2_Traj_operator.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticExprWrapper.h"
#include "codac2_ExprType.h"
#include "codac2_Wrapper.h"

namespace codac2
{
  template<typename TR>
  struct TrajectoryOp
  {
    static typename TR::Type::Domain fwd(const TR& x1, const Interval& x2)
    {
      return x1(x2);
    }

    static typename TR::Type fwd(const TR& x1, const ScalarType& x2)
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
      [[maybe_unused]] const TR& x1,
      [[maybe_unused]] const typename TR::Type::Domain& y,
      [[maybe_unused]] Interval& x2)
    {
      // todo
    }
  };

  template<typename TR, typename T, typename S>
  class AnalyticOperationExpr<TrajectoryOp<TR>,T,S>
    : public AnalyticExpr<typename TR::Type>, public OperationExprBase<AnalyticExpr<ScalarType>>
  {
    public:

      AnalyticOperationExpr(const TR& x1, const ScalarExpr& x2)
        : OperationExprBase<AnalyticExpr<ScalarType>>(x2), _x1(x1)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<TrajectoryOp<TR>,T,ScalarType>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<ScalarType>>::replace_arg(old_arg_id, new_expr);
      }
      
      T fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        return AnalyticExpr<T>::init_value(
          v, TrajectoryOp<TR>::fwd(_x1, std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval)));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        TrajectoryOp<TR>::bwd(_x1, AnalyticExpr<T>::value(v).a, std::get<0>(this->_x)->value(v).a);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      std::pair<Index,Index> output_shape() const {
        return _x1.shape();
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::string str(bool in_parentheses = false) const
      {
        std::string s = "T"; // user cannot (yet) specify a name for the trajectory
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return true;
      }

    protected:

      const TR _x1;
  };
}
