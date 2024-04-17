/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>
#include "codac2_Arg.h"
#include "codac2_Expr.h"
#include "codac2_Function.h"

struct ExprWrapper
{
  ExprWrapper(const codac2::Arg& e)
    : _e( std::dynamic_pointer_cast<codac2::Expr<codac2::Interval>>(e.copy()))
  {

  }

  ExprWrapper(const std::shared_ptr<codac2::Expr<codac2::Interval>>& e)
    : _e(e)
  {

  }

  const std::shared_ptr<codac2::Expr<codac2::Interval>> _e;
};