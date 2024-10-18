/** 
 *  \file codac2_arithmetic_add.h
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

  inline Vector operator+(const Vector& x1, const Vector& x2)
  {
    assert_release(x1.size() == x2.size());
    return eigen(x1) + eigen(x2);
  }

  inline IntervalVector operator+(const Vector& x1, const IntervalVector& x2)
  {
    assert_release(x1.size() == x2.size());
    return eigen(x1).template cast<Interval>() + eigen(x2);
  }
  
  // ====== First operand: matrix

  template<typename M,typename M_>
    requires IsMatrix<M> && IsMatrix<M_>
  Matrix operator+(const M& x1, const M_& x2)
  {
    assert_release(x1.nb_rows() == x2.nb_rows() && x1.nb_cols() == x2.nb_cols());
    return eigen(x1) + eigen(x2);
  }

  template<typename M,typename IM>
    requires IsMatrix<M> && IsIntervalMatrix<IM>
  IntervalMatrix operator+(const M& x1, const IM& x2)
  {
    assert_release(x1.nb_rows() == x2.nb_rows() && x1.nb_cols() == x2.nb_cols());
    return eigen(x1).template cast<Interval>() + eigen(x2);
  }

  // ====== First operand: interval vector

  inline IntervalVector operator+(const IntervalVector& x1, const Vector& x2)
  {
    assert_release(x1.size() == x2.size());
    return eigen(x1) + eigen(x2).template cast<Interval>();
  }

  inline IntervalVector operator+(const IntervalVector& x1, const IntervalVector& x2)
  {
    assert_release(x1.size() == x2.size());
    return eigen(x1) + eigen(x2);
  }

  // ====== First operand: interval matrix

  template<typename IM,typename M>
    requires IsIntervalMatrix<IM> && IsMatrix<M>
  IntervalMatrix operator+(const IM& x1, const M& x2)
  {
    assert_release(x1.nb_rows() == x2.nb_rows() && x1.nb_cols() == x2.nb_cols());
    return eigen(x1) + eigen(x2).template cast<Interval>();
  }

  template<typename IM,typename IM_>
    requires IsIntervalMatrix<IM> && IsIntervalMatrix<IM_>
  IntervalMatrix operator+(const IM& x1, const IM_& x2)
  {
    assert_release(x1.nb_rows() == x2.nb_rows() && x1.nb_cols() == x2.nb_cols());
    return eigen(x1) + eigen(x2);
  }
}