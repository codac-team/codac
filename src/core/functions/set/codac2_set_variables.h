/** 
 *  \file codac2_set_variables.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <iostream>
#include "codac2_SetExpr.h"
#include "codac2_VarBase.h"

namespace codac2
{
  class SetVar : public SetExpr, public VarBase
  {
    public:

      explicit SetVar(Index n, const std::string& name = "?")
        : VarBase(name), _n(n)
      { }

      virtual const ExprID& unique_id() const
      {
        return SetExpr::unique_id();
      }

      std::shared_ptr<VarBase> arg_copy() const
      {
        return std::make_shared<SetVar>(*this);
      }

      std::shared_ptr<ExprBase> copy() const
      {
        return std::make_shared<SetVar>(*this);
      }

      Index size() const
      {
        return _n;
      }

      void replace_arg(const ExprID& old_arg_id, const std::shared_ptr<ExprBase>& new_expr)
      { }
      
      operator std::shared_ptr<SetExpr>() const
      {
        return std::dynamic_pointer_cast<SetExpr>(this->copy());
      }

      virtual bool belongs_to_args_list(const FunctionArgsList& args) const
      {
        for(const auto& xi : args)
          if(xi->unique_id() == this->unique_id())
            return true;
        return false;
      }

      std::shared_ptr<CtcBase<IntervalVector>> create_ctc(const FunctionArgsList& args, const std::vector<std::shared_ptr<CtcBase<IntervalVector>>>& x) const
      {
        for(Index i = 0 ; i < args.size() ; i++)
          if(args[i]->unique_id() == unique_id())
            return x[i];
        assert(false);
        return nullptr;
      }

      std::shared_ptr<SepBase> create_sep(const FunctionArgsList& args, const std::vector<std::shared_ptr<SepBase>>& x) const
      {
        for(Index i = 0 ; i < args.size() ; i++)
          if(args[i]->unique_id() == unique_id())
            return x[i];
        assert(false);
        return nullptr;
      }

    protected:

      Index _n;
  };
}