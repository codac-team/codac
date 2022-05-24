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

#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"

using namespace std;

namespace codac2
{
  SliceVector::SliceVector(size_t n, const TubeVector& tube_vector, list<shared_ptr<TSlice>>::iterator it_tslice) :
    _tube_vector(tube_vector), _it_tslice(it_tslice),
    _codomain(IntervalVector(n))
  {

  }

  SliceVector::~SliceVector()
  {

  }

  const TubeVector& SliceVector::tube_vector() const
  {
    return _tube_vector;
  }

  size_t SliceVector::size() const
  {
    return _codomain.size();
  }

  bool SliceVector::contains(const TrajectoryVector& value) const
  {
    return true;
  }

  const Interval& SliceVector::tdomain() const
  {
    return _it_tslice->get()->tdomain();
  }

  shared_ptr<const SliceVector> SliceVector::prev_slice() const
  {
    if(_tube_vector.first_slice().get() == this)
      return shared_ptr<const SliceVector>(nullptr);
    return prev(_it_tslice)->get()->slices().at(&_tube_vector);
  }

  shared_ptr<SliceVector> SliceVector::prev_slice()
  {
    return const_pointer_cast<SliceVector>(
      static_cast<const SliceVector&>(*this).prev_slice());
  }

  shared_ptr<const SliceVector> SliceVector::next_slice() const
  {
    if(_tube_vector.last_slice().get() == this)
      return shared_ptr<const SliceVector>(nullptr);
    return next(_it_tslice)->get()->slices().at(&_tube_vector);
  }

  shared_ptr<SliceVector> SliceVector::next_slice()
  {
    return const_pointer_cast<SliceVector>(
      static_cast<const SliceVector&>(*this).next_slice());
  }

  const IntervalVector& SliceVector::codomain() const
  {
    return _codomain;
  }

  const IntervalVector SliceVector::input_gate() const
  {
    IntervalVector gate = codomain();
    if(prev_slice())
      gate &= prev_slice().get()->codomain();
    return gate;
  }

  const IntervalVector SliceVector::output_gate() const
  {
    IntervalVector gate = codomain();
    if(next_slice())
      gate &= next_slice().get()->codomain();
    return gate;
  }

  void SliceVector::set(const IntervalVector& codomain)
  {
    assert((size_t)codomain.size() == size());
    _codomain = codomain;
  }

  ostream& operator<<(ostream& os, const SliceVector& x)
  {
    os << x.tdomain()
       << "↦" << x.codomain()
       << flush;
    return os;
  }
} // namespace codac