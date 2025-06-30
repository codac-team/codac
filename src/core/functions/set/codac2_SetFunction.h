/** 
 *  \file codac2_SetFunction.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_SetExpr.h"
#include "codac2_Domain.h"
#include "codac2_set_variables.h"
#include "codac2_FunctionBase.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SetFunction : public FunctionBase<SetExpr>
  {
    public:

      SetFunction(const FunctionArgsList& args, const std::shared_ptr<SetExpr>& y)
        : FunctionBase<SetExpr>(args, y)
      {
        assert_release(y->belongs_to_args_list(this->args()) && 
          "Invalid arguments: variable not present in input arguments");
      }

      SetFunction(const SetFunction& f)
        : FunctionBase<SetExpr>(f)
      { }

      template<typename... Args>
      auto create_ctc(const Args&... x)
      {
        std::vector<std::shared_ptr<CtcBase<IntervalVector>>> ref_x(sizeof...(Args));
        Index i = 0;
        ((ref_x[i++] = create_arg_ctc_copy(x)), ...);
        assert_release(args().size() == ref_x.size() && 
          "Invalid arguments: wrong number of input arguments");
        return this->expr()->create_ctc(args(), ref_x);
      }

      template<typename... Args>
      auto create_sep(const Args&... x)
      {
        std::vector<std::shared_ptr<SepBase>> ref_x(sizeof...(Args));
        Index i = 0;
        ((ref_x[i++] = create_arg_sep_copy(x)), ...);
        assert_release(args().size() == ref_x.size() && 
          "Invalid arguments: wrong number of input arguments");
        return this->expr()->create_sep(args(), ref_x);
      }

      template<typename... X>
      std::shared_ptr<SetExpr> operator()(const X&... x) const
      {
        auto expr_copy = expr()->copy();
        Index i = 0;
        (expr_copy->replace_arg(_args[i++]->unique_id(), this->__get_copy(x)), ...);
        assert_release(i == this->args().size() && 
          "Invalid arguments: wrong number of input arguments");
        return std::dynamic_pointer_cast<SetExpr>(expr_copy);
      }

      friend std::ostream& operator<<(std::ostream& os, const SetFunction& f)
      {
        os << "set function";
        return os;
      }


    protected:

      template<typename A>
      std::shared_ptr<CtcBase<IntervalVector>> create_arg_ctc_copy(const A& x)
      {
        if constexpr(std::is_base_of_v<Domain,A>)
        {
          CtcWrapper<A> sx(x);
          return sx.copy();
        }

        else if constexpr(std::is_base_of_v<CtcBase<IntervalVector>,A>)
          return x.copy();

        else
        {
          assert_release_constexpr(false && "Invalid argument: unknown input type");
          return nullptr;
        }
      }

      template<typename A>
      std::shared_ptr<SepBase> create_arg_sep_copy(const A& x)
      {
        if constexpr(std::is_base_of_v<Domain,A>)
        {
          SepWrapper<A> sx(x);
          return sx.copy();
        }

        else if constexpr(std::is_base_of_v<SepBase,A>)
          return x.copy();

        else
        {
          assert_release_constexpr(false && "Invalid argument: unknown input type");
          return nullptr;
        }
      }

      std::shared_ptr<SetExpr> __get_copy(std::shared_ptr<SetExpr> x) const
      {
        return std::dynamic_pointer_cast<SetExpr>(x->copy());
      }

      template<typename A>
      std::shared_ptr<SetExpr> __get_copy(const A& x) const
      {
        if constexpr(std::is_base_of_v<VarBase,A>)
          return std::dynamic_pointer_cast<A>(x.copy());
        else
          return const_set(x);
      }
  };
}