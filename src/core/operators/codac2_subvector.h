/** 
 *  \file codac2_subvector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExpr.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct SubvectorOp
  {
    template<typename X1>
    static std::string str(const X1& x1, Index i, Index j)
    {
      return x1->str(!x1->is_str_leaf()) + "[" + std::to_string(i) + ":" + std::to_string(j) + "]";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1, Index i, Index j)
    {
      auto shape1=s1->output_shape();
      assert_release(shape1.second==1 && i<=j && j<shape1.first);
      return { 1, j-i+1 };
    }

    static IntervalVector fwd(const IntervalVector& x1, Index i, Index j);
    static VectorType fwd_natural(const VectorType& x1, Index i, Index j);
    static VectorType fwd_centered(const VectorType& x1, Index i, Index j);
    static void bwd(const IntervalVector& y, IntervalVector& x1, Index i, Index j);
  };

  // Analytic operator

  template<>
  class AnalyticOperationExpr<SubvectorOp,VectorType,VectorType> : public AnalyticExpr<VectorType>, public OperationExprBase<AnalyticExpr<VectorType>>
  {
    public:

      AnalyticOperationExpr(const std::shared_ptr<AnalyticExpr<VectorType>>& x1, Index i, Index j)
        : OperationExprBase<AnalyticExpr<VectorType>>(x1), _i(i), _j(j)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr& e)
        : OperationExprBase<AnalyticExpr<VectorType>>(e), _i(e._i), _j(e._j)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<SubvectorOp,VectorType,VectorType>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<VectorType>>::replace_arg(old_arg_id, new_expr);
      }
      
      VectorType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        if(natural_eval)
          return AnalyticExpr<VectorType>::init_value(
            v, SubvectorOp::fwd_natural(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i, _j));
        else
          return AnalyticExpr<VectorType>::init_value(
            v, SubvectorOp::fwd_centered(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i, _j));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        SubvectorOp::bwd(AnalyticExpr<VectorType>::value(v).a, std::get<0>(this->_x)->value(v).a, _i, _j);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      std::pair<Index,Index> output_shape() const {
         return SubvectorOp::output_shape(std::get<0>(this->_x),_i,_j);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::string str(bool in_parentheses = false) const
      {
        // todo: improve the following:
        std::string s = SubvectorOp::str(std::get<0>(this->_x), _i, _j);
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return false;
      }

    protected:

      const Index _i, _j;
  };
  
  // Inline functions

  inline IntervalVector SubvectorOp::fwd(const IntervalVector& x1, Index i, Index j)
  {
    assert(i >= 0 && i < x1.size() && j >= i && j < x1.size());
    return x1.subvector(i,j);
  }

  inline VectorType SubvectorOp::fwd_natural(const VectorType& x1, Index i, Index j)
  {
    assert(i >= 0 && i < x1.a.rows() && j >= i && j < x1.a.rows());
    return {
      fwd(x1.a,i,j),
      x1.def_domain
    };
  }

  inline VectorType SubvectorOp::fwd_centered(const VectorType& x1, Index i, Index j)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1,i,j);

    assert(i >= 0 && i < x1.a.rows() && j >= i && j < x1.a.rows());
    return {
      fwd(x1.m,i,j),
      fwd(x1.a,i,j),
      x1.da.block(i,0,j-i+1,x1.da.cols()),
      x1.def_domain
    };
  }

  inline void SubvectorOp::bwd(const IntervalVector& y, IntervalVector& x1, Index i, Index j)
  {
    assert(i >= 0 && i < x1.size() && j >= i && j < x1.size());
    assert(j-i < y.size());
    for(Index k = 0 ; k < j-i+1 ; k++)
      x1[i+k] &= y[k];
  }
}
