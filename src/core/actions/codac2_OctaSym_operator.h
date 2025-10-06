/** 
 *  \file codac2_OctaSym_operator.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_OctaSym.h"

namespace codac2
{
  struct OctaSymOp
  {
    template<typename X1>
    static inline std::string str(const X1& x1)
    {
      return "sym(" + x1->str() + ")";
    }

    template<typename X1>
    static inline std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1)
    {
      return s1->output_shape();
    }

    static inline IntervalVector fwd(const OctaSym& s, const IntervalVector& x1)
    {
      assert((Index)s.size() == x1.size());
      return s(x1);
    }

    static inline VectorType fwd_natural(const OctaSym& s, const VectorType& x1)
    {
      assert((Index)s.size() == x1.m.size());
      return {
        fwd(s, x1.a),
        x1.def_domain
      };
    }

    static inline VectorType fwd_centered(const OctaSym& s, const VectorType& x1)
    {
      assert((Index)s.size() == x1.m.size());

      auto da = x1.da;
      for(size_t i = 0 ; i < s.size() ; i++)
        da.row(i) = sign(s[i])*x1.da.row(std::abs(s[i])-1);

      return {
        fwd(s, x1.m),
        fwd(s, x1.a),
        da,
        x1.def_domain
      };
    }

    static inline void bwd(const OctaSym& s, const IntervalVector& y, IntervalVector& x1)
    {
      assert((Index)s.size() == y.size() && (Index)s.size() == x1.size());
      x1 &= s.invert()(y);
    }
  };


  template<>
  class AnalyticOperationExpr<OctaSymOp,VectorType,VectorType>
    : public AnalyticExpr<VectorType>, public OperationExprBase<AnalyticExpr<VectorType>>
  {
    public:

      AnalyticOperationExpr(const OctaSym& s, const VectorExpr& x1)
        : OperationExprBase<AnalyticExpr<VectorType>>(x1), _s(s)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<OctaSymOp,VectorType,VectorType>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<VectorType>>::replace_arg(old_arg_id, new_expr);
      }
      
      VectorType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        if(natural_eval)
          return AnalyticExpr<VectorType>::init_value(
            v, OctaSymOp::fwd_natural(_s, std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval)));
        else
          return AnalyticExpr<VectorType>::init_value(
            v, OctaSymOp::fwd_centered(_s, std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval)));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        OctaSymOp::bwd(_s, AnalyticExpr<VectorType>::value(v).a, std::get<0>(this->_x)->value(v).a);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      std::pair<Index,Index> output_shape() const {
        return { _s.size(), 1 };
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::string str(bool in_parentheses = false) const
      {
        std::string s = "S"; // user cannot (yet) specify a name for the symmetry
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return true;
      }

    protected:

      const OctaSym _s;
  };
}