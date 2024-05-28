/** 
 *  \file
 *  Operations
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>
#include "codac2_set_variables.h"
#include "codac2_set_operators.h"

namespace codac2
{
  using SetExpr_ptr = std::shared_ptr<SetExpr>;

  inline SetExpr_ptr
  operator&(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<InterSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  operator|(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<UnionSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  proj(const SetExpr_ptr& x1, const IntervalVector& x2)
  {
    return std::make_shared<SetOperationExpr<ProjSetOp,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  cart_prod(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<CartProdSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  inverse(const AnalyticFunction<VectorOpValue>& f, const SetExpr_ptr& x1)
  {
    return std::make_shared<SetOperationExpr<InverseSetOp,SetExpr>>(f,x1);
  }

  inline SetExpr_ptr
  operator!(const SetExpr_ptr& x1)
  {
    return std::make_shared<SetOperationExpr<NotSetOp,SetExpr>>(x1);
  }
}