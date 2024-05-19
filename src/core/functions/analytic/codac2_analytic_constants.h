/** 
 *  \file
 *  ConstValue class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

namespace codac2
{
  template<typename T>
  class ConstValueExpr : public AnalyticExpr<T>
  {
    public:

      ConstValueExpr(const typename T::Domain& x)
        : _x(x)
      { }

      ConstValueExpr(const ConstValueExpr<T>& e) = default;

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ConstValueExpr<T>>(*this);
      }

      T fwd_eval(ValuesMap& v, const FunctionArgsList& f_args, const IntervalVector& flatten_x) const
      {
        return AnalyticExpr<T>::init_value(v, T(
            _x.mid(), _x,
            // the derivative of a const value is zero:
            IntervalMatrix::zeros(_x.size(),f_args.total_size()),
            // the definition domain is necesarily met at this point
            true
          ), flatten_x);
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        AnalyticExpr<T>::value(v).a &= _x;
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return true;
      }

    protected:

      const typename T::Domain _x;
  };

  template<typename T>
  std::shared_ptr<AnalyticExpr<typename Wrapper<T>::Domain>> const_value(const T& x)
  {
    return std::make_shared<ConstValueExpr<typename Wrapper<T>::Domain>>(x);
  }
}