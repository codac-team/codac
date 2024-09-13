/** 
 *  codac2_IntervalVector.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_IntervalVector.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  IntervalVector::IntervalVector(size_t n)
    : IntervalVector(n,Interval())
  {
    assert_release(n >= 0);
  }

  IntervalVector::IntervalVector(size_t n, const Interval& x)
    : MatrixBase<IntervalVector,Interval>(n,1,x),
      IntervalMatrixBase<IntervalVector,Vector>(n,1),
      VectorBase<IntervalVector,IntervalMatrix,Interval>(n)
  {
    assert_release(n >= 0);
  }

  IntervalVector::IntervalVector(size_t n, const double bounds[][2])
    : MatrixBase<IntervalVector,Interval>(n,1),
      IntervalMatrixBase<IntervalVector,Vector>(n,1,bounds),
      VectorBase<IntervalVector,IntervalMatrix,Interval>(n)
  {
    assert_release(n >= 0);
  }

  IntervalVector::IntervalVector(const Vector& x)
    : MatrixBase<IntervalVector,Interval>(x._e.template cast<Interval>()),
      IntervalMatrixBase<IntervalVector,Vector>(x.size(),1),
      VectorBase<IntervalVector,IntervalMatrix,Interval>(x.size())
  { }

  IntervalVector::IntervalVector(const Vector& lb, const Vector& ub)
    : MatrixBase<IntervalVector,Interval>(lb._e.template cast<Interval>()),
      IntervalMatrixBase<IntervalVector,Vector>(lb.size(),1),
      VectorBase<IntervalVector,IntervalMatrix,Interval>(lb.size())
  {
    assert_release(lb.size() == ub.size());
    *this |= ub;
  }

  IntervalVector::IntervalVector(initializer_list<Interval> l)
    : MatrixBase<IntervalVector,Interval>(l.size(),1),
      IntervalMatrixBase<IntervalVector,Vector>(l.size(),1),
      VectorBase<IntervalVector,IntervalMatrix,Interval>(l)
  {
    assert_release(!std::empty(l));
  }

  IntervalVector::IntervalVector(const MatrixBase<Vector,double>& x)
    : IntervalVector(x._e.template cast<Interval>())
  { }

  IntervalVector::IntervalVector(const MatrixBase<IntervalVector,Interval>& x)
    : IntervalVector(x._e)
  { }

  bool operator==(const IntervalVector& x1, const IntervalVector& x2)
  {
    // ^ This overload allows automatic cast for Vector == IntervalVector comparisons
    return (IntervalMatrixBase<IntervalVector,Vector>)x1 == (IntervalMatrixBase<IntervalVector,Vector>)x2;
  }

  list<IntervalVector> IntervalVector::complementary() const
  {
    return IntervalVector(this->size()).diff(*this);
  }

  list<IntervalVector> IntervalVector::diff(const IntervalVector& y, bool compactness) const
  {
    // This code originates from the ibex-lib
    // See: ibex_TemplateVector.h
    // Author: Gilles Chabert
    // It has been revised with modern C++ and templated types

    const size_t n = this->size();
    assert_release(y.size() == n);

    if(y == *this)
      return { };

    IntervalVector x = *this;
    IntervalVector z = x & y;

    if(z.is_empty())
    {
      if(x.is_empty()) return { };
      else return { x };
    }

    else
    {
      // Check if in one dimension y is flat and x not,
      // in which case the diff returns also x directly
      if(compactness)
        for(size_t i = 0 ; i < n ; i++)
          if(z[i].is_degenerated() && !x[i].is_degenerated())
          {
            if(x.is_empty()) return { };
            else return { x };
          }
    }

    list<IntervalVector> l;

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
        if(!v.is_empty())
          l.push_back(v);
      }

      x[var] = z[var];
    }

    return l;
  }

  IntervalVector IntervalVector::empty(size_t n)
  {
    assert_release(n >= 0);
    return IntervalVector(n,Interval::empty());
  }

  ostream& operator<<(ostream& os, const IntervalVector& x)
  {
    if(x.is_empty())
      return os << "( empty vector )";

    else
      return os << (const VectorBase<IntervalVector,IntervalMatrix,Interval>&)x;
  }
}