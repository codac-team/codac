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
      {
        for(const auto& arg : args)
          push_back(arg->arg_copy());
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

      size_t total_size() const
      {
        size_t n = 0;
        for(const auto& ai : *this)
          n += ai->size();
        return n;
      }
  };
}