/** 
 *  \file codac2_FunctionArgsList.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_VarBase.h"

namespace codac2
{
  class FunctionArgsList : public std::vector<std::shared_ptr<VarBase>>
  {
    public:

      FunctionArgsList()
      { }

      FunctionArgsList(const FunctionArgsList& args)
        : std::vector<std::shared_ptr<VarBase>>(args.size())
      {
        size_t i = 0;
        for(const auto& arg : args)
          (*this)[i++] = arg->arg_copy();
      }

      FunctionArgsList(const std::vector<std::reference_wrapper<VarBase>>& args)
      {
        for(const auto& arg : args)
          push_back(arg.get().arg_copy());
      }

      FunctionArgsList(std::initializer_list<std::reference_wrapper<VarBase>> args)
      {
        for(const auto& arg : args)
          push_back(arg.get().arg_copy());
      }

      Index total_size() const
      {
        Index n = 0;
        for(const auto& ai : *this)
          n += ai->size();
        return n;
      }
  };
}