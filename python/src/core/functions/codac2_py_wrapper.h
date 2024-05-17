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
#include "codac2_analytic_operations.h"
#include "codac2_analytic_variables.h"
#include "codac2_AnalyticExpr.h"
#include "codac2_AnalyticFunction.h"

template<typename T>
struct ExprWrapper
{
  ExprWrapper(const codac2::ScalarVar& e)
    : _e(std::dynamic_pointer_cast<codac2::AnalyticExpr<T>>(e.copy()))
  {
    static_assert(std::is_same_v<T,codac2::ScalarOpValue>);
  }

  ExprWrapper(const codac2::VectorVar& e)
    : _e(std::dynamic_pointer_cast<codac2::AnalyticExpr<T>>(e.copy()))
  {
    static_assert(std::is_same_v<T,codac2::VectorOpValue>);
  }

  ExprWrapper(const std::shared_ptr<codac2::AnalyticExpr<T>>& e)
    : _e(e)
  { }

  const std::shared_ptr<codac2::AnalyticExpr<T>> _e;
};