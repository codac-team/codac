/** 
 *  \file codac2_component.h
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

namespace codac2
{
  struct ComponentOp
  {
    template<typename X1>
    static std::string str(const X1& x1, Index i)
    {
      return x1->str(!x1->is_str_leaf()) + "[" + std::to_string(i) + "]";
    }

    template<typename X1>
    static std::string str(const X1& x1, Index i, Index j)
    {
      return x1->str(!x1->is_str_leaf()) + "(" + std::to_string(i) + "," + std::to_string(j) + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1, Index i)
    {
      auto shape1 = s1->output_shape();
      assert(shape1.second==1 && i<shape1.first);
      return {1,1};
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape(const X1& s1, Index i, Index j)
    {
      auto shape1 = s1->output_shape();
      assert(j<shape1.second && i<shape1.first);
      return {1,1};
    }

    static Interval fwd(const IntervalVector& x1, Index i);
    static ScalarType fwd_natural(const VectorType& x1, Index i);
    static ScalarType fwd_centered(const VectorType& x1, Index i);
    static void bwd(const Interval& y, IntervalVector& x1, Index i);

    static Interval fwd(const IntervalMatrix& x1, Index i, Index j);
    static ScalarType fwd_natural(const MatrixType& x1, Index i, Index j);
    static ScalarType fwd_centered(const MatrixType& x1, Index i, Index j);
    static void bwd(const Interval& y, IntervalMatrix& x1, Index i, Index j);
  };

  // Analytic operator

  template<>
  class AnalyticOperationExpr<ComponentOp,ScalarType,VectorType> : public AnalyticExpr<ScalarType>, public OperationExprBase<AnalyticExpr<VectorType>>
  {
    public:

      AnalyticOperationExpr(const std::shared_ptr<AnalyticExpr<VectorType>>& x1, Index i)
        : OperationExprBase<AnalyticExpr<VectorType>>(x1), _i(i)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr& e)
        : OperationExprBase<AnalyticExpr<VectorType>>(e), _i(e._i)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,VectorType>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<VectorType>>::replace_arg(old_arg_id, new_expr);
      }

      ScalarType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        if(natural_eval)
          return AnalyticExpr<ScalarType>::init_value(
            v, ComponentOp::fwd_natural(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i));
        else
          return AnalyticExpr<ScalarType>::init_value(
            v, ComponentOp::fwd_centered(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        ComponentOp::bwd(AnalyticExpr<ScalarType>::value(v).a, std::get<0>(this->_x)->value(v).a, _i);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      std::pair<Index,Index> output_shape() const
      {
        return ComponentOp::output_shape(std::get<0>(this->_x),_i);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::string str(bool in_parentheses = false) const
      {
        std::string s = ComponentOp::str(std::get<0>(this->_x), _i);
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return true;
      }

    protected:

      const Index _i;
  };

  template<>
  class AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType> : public AnalyticExpr<ScalarType>, public OperationExprBase<AnalyticExpr<MatrixType>>
  {
    public:

      AnalyticOperationExpr(const std::shared_ptr<AnalyticExpr<MatrixType>>& x1, Index i, Index j)
        : OperationExprBase<AnalyticExpr<MatrixType>>(x1), _i(i), _j(j)
      { }

      AnalyticOperationExpr(const AnalyticOperationExpr& e)
        : OperationExprBase<AnalyticExpr<MatrixType>>(e), _i(e._i), _j(e._j)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType>>(*this);
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      {
        return OperationExprBase<AnalyticExpr<MatrixType>>::replace_arg(old_arg_id, new_expr);
      }

      ScalarType fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        if(natural_eval)
          return AnalyticExpr<ScalarType>::init_value(
            v, ComponentOp::fwd_natural(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i, _j));
        else
          return AnalyticExpr<ScalarType>::init_value(
            v, ComponentOp::fwd_centered(std::get<0>(this->_x)->fwd_eval(v, total_input_size, natural_eval), _i, _j));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        ComponentOp::bwd(AnalyticExpr<ScalarType>::value(v).a, std::get<0>(this->_x)->value(v).a, _i, _j);
        std::get<0>(this->_x)->bwd_eval(v);
      }

      std::pair<Index,Index> output_shape() const
      {
        return ComponentOp::output_shape(std::get<0>(this->_x),_i,_j);
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return std::get<0>(this->_x)->belongs_to_args_list(args);
      }

      std::string str(bool in_parentheses = false) const
      {
        std::string s = ComponentOp::str(std::get<0>(this->_x), _i, _j);
        return in_parentheses ? "(" + s + ")" : s;
      }

      virtual bool is_str_leaf() const
      {
        return true;
      }

    protected:

      const Index _i, _j;
  };

  // Inline functions

  // Vector component

    inline Interval ComponentOp::fwd(const IntervalVector& x1, Index i)
    {
      assert(i >= 0 && i < x1.size());
      return x1[i];
    }

    inline ScalarType ComponentOp::fwd_natural(const VectorType& x1, Index i)
    {
      assert(i >= 0 && i < x1.a.rows());
      return {
        fwd(x1.a,i),
        x1.def_domain
      };
    }

    inline ScalarType ComponentOp::fwd_centered(const VectorType& x1, Index i)
    {
      if(centered_form_not_available_for_args(x1))
        return fwd_natural(x1,i);

      assert(i >= 0 && i < x1.a.rows());
      return {
        fwd(x1.m,i),
        fwd(x1.a,i),
        x1.da.row(i),
        x1.def_domain
      };
    }

    inline void ComponentOp::bwd(const Interval& y, IntervalVector& x1, Index i)
    {
      assert(i >= 0 && i < x1.size());
      x1[i] &= y;
    }

  // Matrix component

    inline Interval ComponentOp::fwd(const IntervalMatrix& x1, Index i, Index j)
    {
      assert(i >= 0 && i < x1.rows());
      assert(j >= 0 && j < x1.cols());
      return x1(i,j);
    }

    inline ScalarType ComponentOp::fwd_natural(const MatrixType& x1, Index i, Index j)
    {
      assert(i >= 0 && i < x1.a.rows());
      assert(j >= 0 && j < x1.a.cols());
      return {
        fwd(x1.a,i,j),
        x1.def_domain
      };
    }

    inline ScalarType ComponentOp::fwd_centered(const MatrixType& x1, Index i, Index j)
    {
      if(centered_form_not_available_for_args(x1))
        return fwd_natural(x1,i,j);

      assert(i >= 0 && i < x1.a.rows());
      assert(j >= 0 && j < x1.a.cols());
      return {
        fwd(x1.m,i,j),
        fwd(x1.a,i,j),
        x1.da.row(x1.a.rows()*i+j), // centered form, ColMajor
        x1.def_domain
      };
    }

    inline void ComponentOp::bwd(const Interval& y, IntervalMatrix& x1, Index i, Index j)
    {
      assert(i >= 0 && i < x1.rows());
      assert(j >= 0 && j < x1.cols());
      x1(i,j) &= y;
    }
}
