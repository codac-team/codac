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

      explicit IntervalVector(size_t n)
        : IntervalVector(n,Interval())
      {
        assert_release(n > 0);
      }

      explicit IntervalVector(size_t n, const Interval& x)
        : MatrixBase<IntervalVector,Interval>(n,1,x),
          IntervalMatrixBase<IntervalVector,Vector>(n,1),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(n)
      {
        assert_release(n > 0);
      }

      IntervalVector(const Vector& x)
        : MatrixBase<IntervalVector,Interval>(x._e.template cast<Interval>()),
          IntervalMatrixBase<IntervalVector,Vector>(x.size(),1),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(x.size())
      { }

      explicit IntervalVector(const Vector& lb, const Vector& ub)
        : MatrixBase<IntervalVector,Interval>(lb._e.template cast<Interval>()),
          IntervalMatrixBase<IntervalVector,Vector>(lb.size(),1),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(lb.size())
      {
        assert_release(lb.size() == ub.size());
        for(size_t i = 0 ; i < lb.size() ; i++)
        {
          assert_release(lb[i] <= ub[i]);
        }

        *this |= ub;
      }

      explicit IntervalVector(size_t n, const double bounds[][2])
        : MatrixBase<IntervalVector,Interval>(n,1),
          IntervalMatrixBase<IntervalVector,Vector>(n,1,bounds),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(n)
      {
        assert_release(n > 0);
      }

      IntervalVector(std::initializer_list<Interval> l)
        : MatrixBase<IntervalVector,Interval>(l.size(),1),
          IntervalMatrixBase<IntervalVector,Vector>(l.size(),1),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(l)
      {
        assert_release(!std::empty(l));
      }

      IntervalVector(const IntervalMatrixBase<IntervalVector,Vector>& x)
        : MatrixBase<IntervalVector,Interval>(x),
          IntervalMatrixBase<IntervalVector,Vector>(x),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(x._e)
      { }

      template<typename OtherDerived>
      IntervalVector(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<IntervalVector,Interval>(x.template cast<Interval>()),
          IntervalMatrixBase<IntervalVector,Vector>(x.template cast<Interval>()),
          VectorBase<IntervalVector,IntervalMatrix,Interval>(x.template cast<Interval>())
      { }

      template<typename OtherDerived>
      IntervalVector(const MatrixBaseBlock<OtherDerived,IntervalMatrix,Interval>& x)
        : IntervalVector(x.eval())
      {
        assert_release(x._q == 1);
      }

      using IntervalMatrixBase<IntervalVector,Vector>::operator==;

      std::vector<IntervalVector> complementary() const
      {
        return IntervalVector(this->size()).diff(*this);
      }

      std::vector<IntervalVector> diff(const IntervalVector& y, bool compactness = true) const
      {
        // This code originates from the ibex-lib
        // See: ibex_TemplateVector.h
        // Author: Gilles Chabert
        // It has been revised with modern C++ and templated types

        const size_t n = this->size();
        assert_release(y.size() == n);

        if(y == *this)
          return { IntervalVector::empty(n) };

        IntervalVector x = *this;
        IntervalVector z = x & y;

        if(z.is_empty())
          return { x };

        else
        {
          // Check if in one dimension y is flat and x not,
          // in which case the diff returns also x directly
          if(compactness)
            for(size_t i = 0 ; i < n ; i++)
              if(z[i].is_degenerated() && !x[i].is_degenerated())
                return { x };
        }

        std::vector<IntervalVector> l;

        for(size_t var = 0 ; var < n ; var++)
        {
          Interval c1, c2;
          
          for(const auto& ci : x[var].diff(y[var], compactness))
          {
            assert(!ci.is_empty());

            IntervalVector v(n);
            for(size_t i = 0 ; i < var ; i++)
              v[i] = x[i];
            v[var] = ci;
            for(size_t i = var+1 ; i < n ; i++)
              v[i] = x[i];
            l.push_back(v);
          }

          x[var] = z[var];
        }

        return l;
      }

      static IntervalVector zeros(size_t n)
      {
        assert_release(n > 0);
        return EigenMatrix<Interval>::Zero(n,1);
      }

      static IntervalVector ones(size_t n)
      {
        assert_release(n > 0);
        return EigenMatrix<Interval>::Ones(n,1);
      }

      static IntervalVector empty(size_t n)
      {
        assert_release(n > 0);
        return IntervalVector(n,Interval::empty());
      }
  };

  /*inline IntervalMatrix::IntervalMatrix(std::initializer_list<IntervalVector> l)
    : MatrixBase<IntervalMatrix,Interval>(l.begin()->size(),l.size()), IntervalMatrixBase<IntervalMatrix,Matrix>(l.begin()->size(),l.size())
  {
    assert_release(!std::empty(l));
    size_t i = 0;

    for(const auto& li : l)
    {
      assert_release(this->nb_rows() != li.size());
      this->col(i++) = li;
    }

    assert(i == this->nb_cols());
  }*/

  inline std::ostream& operator<<(std::ostream& os, const IntervalVector& x)
  {
    if(x.is_empty())
      return os << "( empty vector )";

    else
      return os << (const VectorBase<IntervalVector,IntervalMatrix,Interval>&)x;
  }

  inline IntervalVector operator*(const IntervalMatrix& x1, const IntervalVector& x2)
  {
    return x1._e * x2._e;
  }

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
}