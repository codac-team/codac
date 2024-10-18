/** 
 *  \file codac2_set_constants.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  template<typename T>
  class ConstValueSetExpr : public SetExpr
  {
    public:

      ConstValueSetExpr(const T& x)
        : _x(x)
      { }

      ConstValueSetExpr(const ConstValueSetExpr<T>& e) = default;

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<ConstValueSetExpr<T>>(*this);
      }

      void replace_expr(const ExprID& old_expr_id, const std::shared_ptr<ExprBase>& new_expr)
      { }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        return true;
      }

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& x) const
      {
        return std::make_shared<CtcWrapper_<T>>(_x);
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& x) const
      {
        return std::make_shared<SepWrapper_<T>>(_x);
      }
      
      operator std::shared_ptr<SetExpr>() const
      {
        return std::dynamic_pointer_cast<SetExpr>(this->copy());
      }

    protected:

      const T _x;
  };

  template<typename T>
  std::shared_ptr<SetExpr> const_set(const T& x)
  {
    return std::make_shared<ConstValueSetExpr<T>>(x);
  }
}