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
  class ConstValueExpr : public Expr<T>
  {
    public:

      ConstValueExpr(const T& x)
        : _x(x)
      { }

      ConstValueExpr(const ConstValueExpr<T>& e)
        : _x(e._x)
      { }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ConstValueExpr<T>>(*this);
      }

      T fwd_eval(ValuesMap& v) const
      {
        return Expr<T>::init_value(v, _x);
      }
      
      void bwd_eval(ValuesMap& v) const
      {
        Expr<T>::value(v) &= _x;
      }

      void replace_expr(ExprID old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }

    protected:

      const T _x;
  };

  template<typename T>
  std::shared_ptr<Expr<typename Wrapper<T>::Domain>> const_value(const T& x)
  {
    return std::make_shared<ConstValueExpr<typename Wrapper<T>::Domain>>(x);
  }
}