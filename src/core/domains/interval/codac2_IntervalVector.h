/** 
 *  \file codac2_IntervalVector.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalVector. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalVector (IBEX lib, author: Gilles Chabert)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_assert.h"
#include "codac2_Vector.h"
#include "codac2_IntervalMatrixBase.h"
#include "codac2_VectorBase.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  class IntervalVector : public IntervalMatrixBase<IntervalVector,Vector>, public VectorBase<IntervalVector,IntervalMatrix,Interval>
  {
    public:

      explicit IntervalVector(size_t n);

      explicit IntervalVector(size_t n, const Interval& x);

      explicit IntervalVector(size_t n, const double bounds[][2]);

      IntervalVector(const Vector& x);

      explicit IntervalVector(const Vector& lb, const Vector& ub);

      IntervalVector(std::initializer_list<Interval> l);

      IntervalVector(const MatrixBase<Vector,double>& x);

      IntervalVector(const MatrixBase<IntervalVector,Interval>& x);

      template<typename OtherDerived>
      IntervalVector(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<IntervalVector,Interval>(x.template cast<Interval>()),
          IntervalMatrixBase<IntervalVector,Vector>(x.template cast<Interval>()),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(x.template cast<Interval>())
      {
        assert_release(x.cols() == 1);
      }

      template<typename OtherDerived>
      IntervalVector(const MatrixBaseBlock<OtherDerived,Interval>& x)
        : IntervalVector(x.eval())
      {
        assert_release(x._q == 1); // column block only
      }

      friend bool operator==(const IntervalVector& x1, const IntervalVector& x2);

      std::vector<IntervalVector> complementary() const;

      std::vector<IntervalVector> diff(const IntervalVector& y, bool compactness = true) const;

      static IntervalVector empty(size_t n);
  };

  std::ostream& operator<<(std::ostream& os, const IntervalVector& x);

  template<typename... X, typename = typename std::enable_if<(true && ... && (
      (std::is_same_v<Interval,X> || std::is_same_v<IntervalVector,X> || std::is_same_v<Vector,X>)
    )), void>::type>
  inline IntervalVector cart_prod(const X&... x)
  {
    size_t n = 0;
    ((n += x.size()), ...);
    IntervalVector x_(n);

    size_t i = 0;

    auto increm = [](size_t& i, size_t n)
    {
      size_t i_ = i;
      i += n;
      return i_;
    };

    (x_.put(increm(i,x.size()),IntervalVector({x})), ...);
    return x_;
  }


  template<typename Q_>
  IntervalVector operator*(const MatrixBaseBlock<Q_,Interval>& x1, const Vector& x2)
  {
    assert_release(x1.nb_cols() == x2.size());
    return x1.eval() * x2._e.template cast<Interval>();
  }

  template<typename Q_>
  IntervalVector operator*(const Matrix& x1, const MatrixBaseBlock<Q_,Interval>& x2)
  {
    assert_release(x1.nb_cols() == x2._p);
    return x1._e.template cast<Interval>() * x2.eval();
  }
}