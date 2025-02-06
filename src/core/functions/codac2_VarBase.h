/** 
 *  \file codac2_VarBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_ExprBase.h"

namespace codac2
{
  class VarBase
  {
    public:

      virtual const ExprID& unique_id() const = 0;
      virtual std::shared_ptr<VarBase> arg_copy() const = 0;
      virtual ~VarBase() = default;
      virtual Index size() const = 0;
  };
}