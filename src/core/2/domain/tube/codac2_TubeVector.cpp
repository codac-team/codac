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
    for(list<shared_ptr<TSlice>>::iterator it = _tdomain._tslices.begin();
      it != _tdomain._tslices.end(); ++it)
      it->get()->add_slice(make_shared<SliceVector>(n, *this, it));
  }

  size_t TubeVector::size() const
  {
    return first_slice()->size();
  }

  size_t TubeVector::nb_slices() const
  {
    return _tdomain.nb_tslices();
  }

  const std::shared_ptr<SliceVector>& TubeVector::first_slice() const
  {
    return _tdomain.tslices().front()->slices().at(this);
  }

  std::shared_ptr<SliceVector>& TubeVector::first_slice()
  {
    return const_cast<shared_ptr<SliceVector>&>(
      static_cast<const TubeVector&>(*this).first_slice());
  }

  const std::shared_ptr<SliceVector>& TubeVector::last_slice() const
  {
    return _tdomain.tslices().back()->slices().at(this);
  }

  std::shared_ptr<SliceVector>& TubeVector::last_slice()
  {
    return const_cast<shared_ptr<SliceVector>&>(
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
  
  IntervalVector TubeVector::operator()(double t) const
  {
    return codomain();
  }
  
  IntervalVector TubeVector::operator()(const Interval& t) const
  {
    return codomain();
  }

  void TubeVector::set(const IntervalVector& codomain)
  {
    assert((size_t)codomain.size() == size());
    for(auto& s : *this)
      s.set(codomain);
  }

  /*TubeVectorComponent TubeVector::operator[](size_t index)
  {
    assert(index >= 0 && index < size());
    return TubeVectorComponent(*this, index);
  }

  const TubeVectorComponent TubeVector::operator[](size_t index) const
  {
//    assert(index >= 0 && index < size());
//return const_cast<Tube&>(static_cast<const TubeVector&>(*this).operator[](index));
 
    assert(index >= 0 && index < size());
    return TubeVectorComponent(*this, index);
  }*/

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