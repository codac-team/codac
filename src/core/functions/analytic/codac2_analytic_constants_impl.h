/** 
 *  \file codac2_analytic_constants_impl.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  template<typename T>
  std::string ConstValueExpr<T>::str(bool in_parentheses) const
  {
    std::ostringstream s;
    if(_x.is_degenerated()) s << _x.mid();
    else s << _x;
    return in_parentheses ? "(" + s.str() + ")" : s.str();
  }
}