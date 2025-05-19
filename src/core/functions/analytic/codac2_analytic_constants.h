/** 
 *  \file codac2_analytic_constants.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Index.h"
#include "codac2_ExprType.h"

namespace codac2
{
  template<typename T>
  class ConstValueExpr : public AnalyticExpr<T>
  {
    public:

      ConstValueExpr(const typename T::Domain& x)
        : _x(x)
      { }

      const typename T::Domain& value() const
      {
        return _x;
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ConstValueExpr<T>>(*this);
      }

      T fwd_eval(ValuesMap& v, Index total_input_size, bool natural_eval) const
      {
        if(natural_eval)
          return AnalyticExpr<T>::init_value(v, T(
              // the mid is not considered for const values in centered form expression:
              _x,
              // the definition domain is necesarily met at this point:
              true
            ));

        else
          return AnalyticExpr<T>::init_value(v, T(
              // the mid is not considered for const values in centered form expression:
              _x,
              _x,
              // the derivative of a const value is zero:
              IntervalMatrix::zero(_x.size(),total_input_size),
              // the definition domain is necesarily met at this point:
              true
            ));
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        AnalyticExpr<T>::value(v).a &= _x;
      }

      std::pair<Index,Index> output_shape() const
      {
        if constexpr(std::is_same_v<T,ScalarType>)
          return {1,1};

        if constexpr(std::is_same_v<T,VectorType>)
          return {_x.size(),1};

        if constexpr(std::is_same_v<T,MatrixType>)
          return {_x.rows(),_x.cols()};

        assert_release(false && "unknow output shape for constant");
      }

      void replace_arg([[maybe_unused]] const ExprID& old_arg_id, [[maybe_unused]] const std::shared_ptr<ExprBase>& new_expr)
      { }

      virtual bool belongs_to_args_list([[maybe_unused]] const FunctionArgsList& args) const
      {
        return true;
      }

      virtual std::string str(bool in_parentheses = false) const
      {
        std::ostringstream s;
        if(_x.is_degenerated()) s << _x.mid();
        else s << _x;
        return in_parentheses ? "(" + s.str() + ")" : s.str();
      }

      virtual bool is_str_leaf() const
      {
        return true;
      }

    protected:

      const typename T::Domain _x;
  };

  template<typename T>
  inline AnalyticExprWrapper<typename ExprType<T>::Type> const_value(const T& x)
  {
    return { std::make_shared<ConstValueExpr<typename ExprType<T>::Type>>(x) };
  }
}
