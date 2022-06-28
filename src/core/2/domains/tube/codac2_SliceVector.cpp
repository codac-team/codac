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
#include "codac_Exception.h"

using namespace std;

namespace codac2
{
  SliceVector::SliceVector(size_t n, const TubeVector& tube_vector, const list<TSlice>::iterator& it_tslice) :
    SliceVector(IntervalVector(n), tube_vector, it_tslice)
  {

  }

  SliceVector::SliceVector(const IntervalVector& box, const TubeVector& tube_vector, const list<TSlice>::iterator& it_tslice) :
    _tubevector(tube_vector), _it_tslice(it_tslice), _codomain(box)
  {
    assert(holds_alternative<IntervalVector>(_codomain));
  }

  SliceVector::SliceVector(const IParals& ad, const TubeVector& tube_vector, const list<TSlice>::iterator& it_tslice) :
    _tubevector(tube_vector), _it_tslice(it_tslice), _codomain(ad)
  {
    assert(holds_alternative<IParals>(_codomain));
  }

  SliceVector::SliceVector(const SliceVector& s) :
    _tubevector(s._tubevector), _it_tslice(s._it_tslice), _codomain(s._codomain)
  {
    
  }

  SliceVector::~SliceVector()
  {

  }

  const TubeVector& SliceVector::tube_vector() const
  {
    return _tubevector;
  }

  size_t SliceVector::size() const
  {
    return codomain().size();
  }

  bool SliceVector::is_gate() const
  {
    return t0_tf().is_degenerated();
  }

  bool SliceVector::is_empty() const
  {
    return input_gate().is_empty() || output_gate().is_empty();
  }

  bool SliceVector::is_unbounded() const
  {
    return codomain().is_unbounded();
  }

  bool SliceVector::contains(const TrajectoryVector& value) const
  {
    return true;
  }

  const Interval& SliceVector::t0_tf() const
  {
    return _it_tslice->t0_tf();
  }

  const TSlice& SliceVector::tslice() const
  {
    return *_it_tslice;
  }

  const SliceVector* SliceVector::prev_slice() const
  {
    if(&_tubevector.first_slice() == this)
      return nullptr;
    return &prev(_it_tslice)->slices().at(&_tubevector);
  }

  SliceVector* SliceVector::prev_slice()
  {
    return const_cast<SliceVector*>(
      static_cast<const SliceVector&>(*this).prev_slice());
  }

  const SliceVector* SliceVector::next_slice() const
  {
    if(&_tubevector.last_slice() == this)
      return nullptr;
    return &next(_it_tslice)->slices().at(&_tubevector);
  }

  SliceVector* SliceVector::next_slice()
  {
    return const_cast<SliceVector*>(
      static_cast<const SliceVector&>(*this).next_slice());
  }

  const IntervalVector& SliceVector::codomain() const
  {
    if(holds_alternative<IntervalVector>(_codomain))
      return get<IntervalVector>(_codomain);

    else if(holds_alternative<IParals>(_codomain))
      return get<IParals>(_codomain).box();

    assert(false && "unhandled case");
    // return IntervalVector(0); // should not happen
  }

  IntervalVector SliceVector::input_gate() const
  {
    IntervalVector gate = codomain();
    if(prev_slice())
      gate &= prev_slice()->codomain();
    return gate;
  }

  IntervalVector SliceVector::output_gate() const
  {
    IntervalVector gate = codomain();
    if(next_slice())
      gate &= next_slice()->codomain();
    return gate;
  }



  const IParals& SliceVector::codomainI()
  {
    if(holds_alternative<IParals>(_codomain))
      return get<IParals>(_codomain); 
    else if (holds_alternative<IntervalVector>(_codomain)) {
      IParals ip(get<IntervalVector>(_codomain));
      _codomain.emplace<IParals>(ip);
      return get<IParals>(_codomain); 
    }
    assert(false && "unhandled case");
    // return IntervalVector(0); // should not happen
  }

  IParals SliceVector::codomainI() const
  {
    if(holds_alternative<IParals>(_codomain))
      return get<IParals>(_codomain); 
    else if (holds_alternative<IntervalVector>(_codomain)) {
      IParals ip(get<IntervalVector>(_codomain));
      return ip;
    }
    assert(false && "unhandled case");
    // return IntervalVector(0); // should not happen
  }

  IParals SliceVector::input_gateI() const
  {
    IParals gate = codomainI();
    if(prev_slice())
      gate &= prev_slice()->codomainI();
    return gate;
  }

  IParals SliceVector::output_gateI() const
  {
    IParals gate = codomain();
    if(next_slice())
      gate &= next_slice()->codomainI();
    return gate;
  }

  void SliceVector::set(const IntervalVector& x)
  {
    if(holds_alternative<IntervalVector>(_codomain))
    {
      assert((size_t)codomain().size() == size());
      get<IntervalVector>(_codomain) = x;
      if(is_gate())
        get<IntervalVector>(_codomain) &= prev_slice()->codomain() & next_slice()->codomain();
    } else if (holds_alternative<IParals>(_codomain)) {
      IParals n (x);
      set(n);
    }
    else
      throw codac::Exception(__func__, "unable to set values for this AbstractDomain");
  }

  void SliceVector::set(const IParals& ip)
  {
    if (holds_alternative<IParals>(_codomain)) {
      get<IParals>(_codomain) = ip;
    } else {
      _codomain.emplace<IParals>(ip);
    }
    if(is_gate())
        get<IParals>(_codomain).meetKeep(prev_slice()->codomainI() & next_slice()->codomainI());
  }

  void SliceVector::set_component(size_t i, const Interval& xi)
  {
    if(holds_alternative<IntervalVector>(_codomain))
    {
      assert((size_t)codomain().size() == size());
      get<IntervalVector>(_codomain)[i] = xi;
      if(is_gate())
        get<IntervalVector>(_codomain)[i] &= prev_slice()->codomain()[i] & next_slice()->codomain()[i];
    }

    else
      throw codac::Exception(__func__, "unable to set values for this AbstractDomain");
  }

  ostream& operator<<(ostream& os, const SliceVector& x)
  {
    os << x.t0_tf()
       << "↦" << x.codomain()
       << flush;
    return os;
  }
} // namespace codac
