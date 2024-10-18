/** 
 *  \file codac2_set_operations.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_set_variables.h"
#include "codac2_set_operators.h"
#include "codac2_set_constants.h"

namespace codac2
{
  using SetExpr_ptr = std::shared_ptr<SetExpr>;

  inline SetExpr_ptr
  operator&(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<InterSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  operator&(const IntervalVector& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<InterSetOp,SetExpr,SetExpr>>(const_set(x1),x2);
  }

  inline SetExpr_ptr
  operator&(const SetExpr_ptr& x1, const IntervalVector& x2)
  {
    return std::make_shared<SetOperationExpr<InterSetOp,SetExpr,SetExpr>>(x1,const_set(x2));
  }

  inline SetExpr_ptr
  operator|(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<UnionSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  operator|(const IntervalVector& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<UnionSetOp,SetExpr,SetExpr>>(const_set(x1),x2);
  }

  inline SetExpr_ptr
  operator|(const SetExpr_ptr& x1, const IntervalVector& x2)
  {
    return std::make_shared<SetOperationExpr<UnionSetOp,SetExpr,SetExpr>>(x1,const_set(x2));
  }

  inline SetExpr_ptr
  proj(const SetExpr_ptr& x1, const std::vector<size_t>& proj_indices, double eps = 0.01)
  {
    return std::make_shared<SetOperationExpr<ProjSetOp,SetExpr>>(x1,proj_indices,eps);
  }

  inline SetExpr_ptr
  proj(const SetExpr_ptr& x1, const std::vector<size_t>& proj_indices, const IntervalVector& y, double eps = 0.01)
  {
    return std::make_shared<SetOperationExpr<ProjSetOp,SetExpr>>(x1,proj_indices,y,eps);
  }

  inline SetExpr_ptr
  cart_prod(const SetExpr_ptr& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<CartProdSetOp,SetExpr,SetExpr>>(x1,x2);
  }

  inline SetExpr_ptr
  cart_prod(const SetExpr_ptr& x1, const IntervalVector& x2)
  {
    return std::make_shared<SetOperationExpr<CartProdSetOp,SetExpr,SetExpr>>(x1,const_set(x2));
  }

  inline SetExpr_ptr
  cart_prod(const IntervalVector& x1, const SetExpr_ptr& x2)
  {
    return std::make_shared<SetOperationExpr<CartProdSetOp,SetExpr,SetExpr>>(const_set(x1),x2);
  }

  inline SetExpr_ptr
  inverse(const AnalyticFunction<VectorOpValue>& f, const SetExpr_ptr& x1)
  {
    return std::make_shared<SetOperationExpr<InverseSetOp,SetExpr>>(f,x1);
  }

  inline SetExpr_ptr
  inverse(const AnalyticFunction<VectorOpValue>& f, const IntervalVector& x1)
  {
    return std::make_shared<SetOperationExpr<InverseSetOp,SetExpr>>(f,const_set(x1));
  }

  inline SetExpr_ptr
  operator!(const IntervalVector& x1)
  {
    return std::make_shared<SetOperationExpr<NotSetOp,SetExpr>>(const_set(x1));
  }

  inline SetExpr_ptr
  operator!(const SetExpr_ptr& x1)
  {
    return std::make_shared<SetOperationExpr<NotSetOp,SetExpr>>(x1);
  }

  inline std::shared_ptr<SetExpr>
  OctaSym::operator()(const std::shared_ptr<SetExpr>& x1) const
  {
    return std::make_shared<SetOperationExpr<ActionSetOp,SetExpr>>(*this,x1);
  }
}