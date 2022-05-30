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

#include <utility>
#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"
#include "codac2_TubeVectorComponent.h"

using namespace std;

namespace codac2
{
  TubeVector::TubeVector(size_t n, TDomain& tdomain) :
    _tdomain(tdomain)
  {
    for(list<TSlice>::iterator it = _tdomain._tslices.begin();
      it != _tdomain._tslices.end(); ++it)
    {
      it->_slices.insert(
        pair<const TubeVector*,SliceVector>(this,
          SliceVector(n, *this, it)));
    }
  }

  TubeVector::TubeVector(const TubeVector& x) :
    _tdomain(x.tdomain())
  {
    for(list<TSlice>::iterator it = _tdomain._tslices.begin();
      it != _tdomain._tslices.end(); ++it)
    {
      it->_slices.insert(
        pair<const TubeVector*,SliceVector>(this,
          SliceVector(it->_slices.at(&x).size(), *this, it)));
    }
  }

  TubeVector::~TubeVector()
  {
    for(auto& s : _tdomain._tslices)
      s._slices.erase(this);
  }

  size_t TubeVector::size() const
  {
    return first_slice().size();
  }

  size_t TubeVector::nb_slices() const
  {
    return _tdomain.nb_tslices();
  }

  const SliceVector& TubeVector::first_slice() const
  {
    return _tdomain.tslices().front().slices().at(this);
  }

  SliceVector& TubeVector::first_slice()
  {
    return const_cast<SliceVector&>(
      static_cast<const TubeVector&>(*this).first_slice());
  }

  const SliceVector& TubeVector::last_slice() const
  {
    return _tdomain.tslices().back().slices().at(this);
  }

  SliceVector& TubeVector::last_slice()
  {
    return const_cast<SliceVector&>(
      static_cast<const TubeVector&>(*this).last_slice());
  }

  bool TubeVector::contains(const TrajectoryVector& value) const
  {
    return true;
  }

  TDomain& TubeVector::tdomain() const
  {
    return _tdomain;
  }

  Interval TubeVector::t0_tf() const
  {
    return _tdomain.t0_tf();
  }

  IntervalVector TubeVector::codomain() const
  {
    IntervalVector codomain(size());
    codomain.set_empty();
    for(const auto& s : *this)
      codomain |= s.codomain();
    return codomain;
  }
  
  TubeVectorEvaluation TubeVector::operator()(double t)
  {
    return TubeVectorEvaluation(*this, t);
  }
  
  TubeVectorEvaluation TubeVector::operator()(const Interval& t)
  {
    return TubeVectorEvaluation(*this, t);
  }
  
  IntervalVector TubeVector::eval(double t) const
  {
    return _tdomain.iterator_tslice(t)->_slices.at(this).codomain();
  }
  
  IntervalVector TubeVector::eval(const Interval& t) const
  {
    list<TSlice>::iterator it = _tdomain.iterator_tslice(t.lb());
    IntervalVector codomain = it->_slices.at(this).codomain();

    while(it != _tdomain.iterator_tslice(t.ub()))
    {
      codomain |= it->_slices.at(this).codomain();
      it++;
    }

    return codomain;
  }

  void TubeVector::set(const IntervalVector& codomain)
  {
    assert((size_t)codomain.size() == size());
    for(auto& s : *this)
      s.set(codomain);
  }

  TubeVectorComponent TubeVector::operator[](size_t i)
  {
    assert(i >= 0 && i < size());
    return TubeVectorComponent(*this, i);
  }

  ostream& operator<<(ostream& os, const TubeVector& x)
  {
    x.TubeVector_const::print(os);
    // Adding information related to sliced structure
    os << ", " << x.nb_slices()
       << " slice" << (x.nb_slices() > 1 ? "s" : "")
       << flush;
    return os;
  }
} // namespace codac