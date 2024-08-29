/** 
 *  \file codac2_arithmetic_div.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_template_tools.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_MatrixBaseBlock.h"

namespace codac2
{
  // ====== First operand: vector

  inline Vector operator/(const Vector& x1, double x2)
  {
    return eigen(x1) / x2;
  }

  inline IntervalVector operator/(const Vector& x1, const Interval& x2)
  {
    return eigen(x1).template cast<Interval>() / x2;
  }

  // ====== First operand: matrix

  template<typename M>
    requires IsMatrix<M>
  Matrix operator/(const M& x1, double x2)
  {
    return eigen(x1) / x2;
  }

  template<typename M>
    requires IsMatrix<M>
  IntervalMatrix operator/(const M& x1, const Interval& x2)
  {
    return eigen(x1).template cast<Interval>() / x2;
  }

  // ====== First operand: interval vector

  inline IntervalVector operator/(const IntervalVector& x1, double x2)
  {
    return eigen(x1) / Interval(x2);
  }

  inline IntervalVector operator/(const IntervalVector& x1, const Interval& x2)
  {
    return eigen(x1) / x2;
  }

  // ====== First operand: interval matrix

  template<typename IM>
    requires IsIntervalMatrix<IM>
  IntervalMatrix operator/(const IM& x1, double x2)
  {
    return eigen(x1) / Interval(x2);
  }

  template<typename IM>
    requires IsIntervalMatrix<IM>
  IntervalMatrix operator/(const IM& x1, const Interval& x2)
  {
    return eigen(x1) / x2;
  }
}