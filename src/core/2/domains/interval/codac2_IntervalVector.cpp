/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_IntervalVector.h"

using namespace std;

namespace codac2
{
  // Class IntervalVector

    /*IntervalVector::IntervalVector(size_t n)
      : IntervalVector_<>(n)
    { }

    IntervalVector::IntervalVector(size_t n, const Interval& x)
      : IntervalVector_<>(n, x)
    { }

    //IntervalVector::IntervalVector(const Interval& x)
    //  : IntervalVector_<>({x})
    //{ }

    IntervalVector::IntervalVector(const IntervalVector& x)
      : IntervalVector_<>(x)
    { }
    
    IntervalVector::IntervalVector(size_t n, const double bounds[][2])
      : IntervalVector_<>(n, bounds)
    { }
    
    IntervalVector::IntervalVector(std::initializer_list<Interval> l)
      : IntervalVector_<>(l)
    {
      assert(l.size() > 0);
    }

    IntervalVectorComponent IntervalVector::operator[](size_t i)
    {
      return IntervalVectorComponent(*this, i);
    }

    IntervalVectorSubvector IntervalVector::operator[](std::array<size_t,2> i)
    {
      return IntervalVectorSubvector(*this, i);
    }

    IntervalVectorConstComponent IntervalVector::operator[](size_t i) const
    {
      return IntervalVectorConstComponent(*this, i);
    }

    IntervalVectorConstSubvector IntervalVector::operator[](std::array<size_t,2> i) const
    {
      return IntervalVectorConstSubvector(*this, i);
    }

    void IntervalVector::resize(size_t n)
    {
      this->IntervalVector_<>::resize(n);
    }

    IntervalVector IntervalVector::empty_set(size_t n)
    {
      return IntervalMatrix_<>::empty_set(n,1);
    }*/

  // Class IntervalVectorComponent
#if 0
    IntervalVectorComponent::IntervalVectorComponent(IntervalVector& x, size_t i)
      : /*Interval(x.i(i)),*/ _x(x), _i(i)
    {
      //#ifndef NDEBUG
      set_name(x.name() + "[" + std::to_string(i) + "]");
      //#endif
    }

    IntervalVectorComponent& IntervalVectorComponent::operator=(const IntervalVectorComponent& xi)
    {
      _x.i(_i) = xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator=(const Interval& xi)
    {
      _x.i(_i) = xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator&=(const Interval& xi)
    {
      _x.i(_i) &= xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator|=(const Interval& xi)
    {
      _x.i(_i) |= xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator+=(const Interval& xi)
    {
      _x.i(_i) += xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator-=(const Interval& xi)
    {
      _x.i(_i) -= xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator*=(const Interval& xi)
    {
      _x.i(_i) *= xi;
      return *this;
    }

    IntervalVectorComponent& IntervalVectorComponent::operator/=(const Interval& xi)
    {
      _x.i(_i) /= xi;
      return *this;
    }

    Interval& IntervalVectorComponent::operator()()
    {
      return _x.i(_i);
    }

    const Interval& IntervalVectorComponent::operator()() const
    {
      return _x.i(_i);
    }

    IntervalVectorComponent::operator Interval&()
    {
      return _x.i(_i);
    }

    IntervalVectorComponent::operator const Interval&() const
    {
      return _x.i(_i);
    }

    IntervalVectorComponent& IntervalVectorComponent::inflate(double r)
    {
      _x.i(_i).inflate(r);
      return *this;
    }
  
    std::ostream& operator<<(std::ostream& os, const IntervalVectorComponent& p)
    {
      os << p.operator const Interval&();
      return os;
    }

  // Class IntervalVectorConstComponent

    IntervalVectorConstComponent::IntervalVectorConstComponent(const IntervalVector& x, size_t i)
      : Interval(x.i(i)), _x(x), _i(i)
    {

    }

    const Interval& IntervalVectorConstComponent::operator()() const
    {
      return _x.i(_i);
    }

  // Class IntervalVectorSubvector

    IntervalVectorSubvector::IntervalVectorSubvector(IntervalVector& x, const std::array<size_t,2>& i)
      : IntervalVector(x.subvector(i[0],i[1])), _x(x), _i(i)
    {

    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator=(const IntervalVectorSubvector& xi)
    {
      assert(_i[1]-_i[0] == xi._i[1]-xi._i[0]);
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) = xi[j-_i[0]];
      return *this;
    }

    const IntervalVectorSubvector& IntervalVectorSubvector::operator=(const IntervalVectorSubvector& xi) const
    {
      assert(_i[1]-_i[0] == xi._i[1]-xi._i[0]);
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) = xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator=(const IntervalVector& xi)
    {
      assert(_i[1]-_i[0]+1 == xi.size());
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) = xi[j-_i[0]];
      return *this;
    }

    const IntervalVectorSubvector& IntervalVectorSubvector::operator=(const IntervalVector& xi) const
    {
      assert(_i[1]-_i[0]+1 == xi.size());
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) = xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator&=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) &= xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator|=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) |= xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator+=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) += xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator-=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) -= xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator*=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) *= xi[j-_i[0]];
      return *this;
    }

    IntervalVectorSubvector& IntervalVectorSubvector::operator/=(const IntervalVector& xi)
    {
      for(size_t j = _i[0] ; j <= _i[1] ; j++)
        _x.i(j) /= xi[j-_i[0]];
      return *this;
    }

  // Class IntervalVectorConstSubvector

    IntervalVectorConstSubvector::IntervalVectorConstSubvector(const IntervalVector& x, const std::array<size_t,2>& i)
      : IntervalVector(x.subvector(i[0],i[1])), _x(x), _i(i)
    {

    }
#endif
} // namespace codac