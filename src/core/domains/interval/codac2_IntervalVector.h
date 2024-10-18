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

      Interval& operator()(size_t i, size_t j) = delete;
      const Interval& operator()(size_t i, size_t j) const = delete;

      friend bool operator==(const IntervalVector& x1, const IntervalVector& x2);

      std::list<IntervalVector> complementary() const;

      std::list<IntervalVector> diff(const IntervalVector& y, bool compactness = true) const;

      static IntervalVector empty(size_t n);

      // Operators

      IntervalVector& operator+=(const IntervalVector& x)
      {
        assert_release(this->size() == x.size());
        this->_e += x._e;
        return *this;
      }

      IntervalVector& operator-=(const IntervalVector& x)
      {
        assert_release(this->size() == x.size());
        this->_e -= x._e;
        return *this;
      }

      IntervalVector& operator*=(const Interval& x)
      {
        this->_e *= x;
        return *this;
      }

      IntervalVector& operator/=(const Interval& x)
      {
        this->_e /= x;
        return *this;
      }
  };

  std::ostream& operator<<(std::ostream& os, const IntervalVector& x);


  inline IntervalVector to_IntervalVector(const Interval& x)
  {
    return IntervalVector(1,x);
  }

  inline IntervalVector to_IntervalVector(const IntervalVector& x)
  {
    return x;
  }

  template<typename... X>
    requires ((std::is_same_v<int,X> || std::is_same_v<double,X> || std::is_same_v<Interval,X> || std::is_same_v<Vector,X> || std::is_same_v<IntervalVector,X>) && ...)
  inline IntervalVector cart_prod(const X&... x)
  {
    std::vector<IntervalVector> v_x;
    ((v_x.push_back(to_IntervalVector(x))), ...);

    size_t n = 0;
    for(const auto& xi : v_x)
      n += xi.size();
    IntervalVector x_(n);

    size_t i = 0;
    for(const auto& xi : v_x)
    {
      x_.put(i, xi);
      i += xi.size();
    }
    return x_;
  }

  inline IntervalVector hull(const std::list<IntervalVector>& l)
  {
    assert_release(!l.empty());
    IntervalVector h(l.front());
    for(const auto& li : l)
      h |= li;
    return h;
  }
}