/** 
 *  \file codac2_trunc.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_matrices.h"

namespace codac2
{
  double trunc(double x);
  Interval trunc(const Interval& x);
  Interval trunc(const Interval& x);
  
  double untrunc(double x);
  Interval untrunc(const Interval& x);

  template<typename OtherDerived,typename Func>
  auto trunc_common(const Eigen::MatrixBase<OtherDerived>& x, const Func& apply_trunc)
  {
    using M = Eigen::MatrixBase<OtherDerived>;
    Eigen::Matrix<typename M::Scalar,M::RowsAtCompileTime,M::ColsAtCompileTime> u(x.rows(),x.cols());

    for(Index i = 0 ; i < x.rows() ; i++)
      for(Index j = 0 ; j < x.cols() ; j++)
        u(i,j) = apply_trunc(x(i,j));

    return u;
  }

  template<typename OtherDerived>
  inline auto trunc(const Eigen::MatrixBase<OtherDerived>& x)
  {
    using T = Eigen::MatrixBase<OtherDerived>::Scalar;
    return trunc_common(x,static_cast<T(*)(const T&)>(&trunc));
  }

  template<typename OtherDerived>
  inline auto untrunc(const Eigen::MatrixBase<OtherDerived>& x)
  {
    using T = Eigen::MatrixBase<OtherDerived>::Scalar;
    return trunc_common(x,static_cast<T(*)(const T&)>(&untrunc));
  }

  double graphic_trunc(double x);
}