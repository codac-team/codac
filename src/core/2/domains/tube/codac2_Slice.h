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

#ifndef __CODAC2_SLICE_H__
#define __CODAC2_SLICE_H__

#include <list>
#include <variant>
#include <memory>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"
#include "codac_Exception.h"
#include "codac2_AbstractSlice.h"

namespace codac2
{
  using codac::Interval;
  using codac::IntervalVector;
  using codac::TrajectoryVector;

  class AbstractSlicedTube;
  class TSlice;

  template<class T>
  class Slice : public AbstractSlice
  {
    public:

      explicit Slice(size_t n, const AbstractSlicedTube& tube_vector, const std::list<TSlice>::iterator& it_tslice) :
        Slice(T(n), tube_vector, it_tslice)
      {

      }

      explicit Slice(const T& box, const AbstractSlicedTube& tube_vector, const std::list<TSlice>::iterator& it_tslice) :
        AbstractSlice(tube_vector,it_tslice), _codomain(box)
      {

      }

      Slice(const Slice& s) :
        AbstractSlice(s._tubevector, s._it_tslice), _codomain(s._codomain)
      {
        
      }

      ~Slice()
      {

      }

      // Slice objects cannot be copyable or movable,
      // as they are supposed to be connected to other Slice objects
      Slice& operator=(const Slice&) = delete;
      Slice(Slice&&) = delete;
      Slice& operator=(Slice&&) = delete;

      const AbstractSlicedTube& tube_vector() const
      {
        return _tubevector;
      }

      virtual std::shared_ptr<AbstractSlice> duplicate() const
      {
        return std::make_shared<Slice>(*this);
      }

      virtual size_t size() const
      {
        return codomain().size();
      }

      bool is_gate() const
      {
        return t0_tf().is_degenerated();
      }

      bool is_empty() const
      {
        return input_gate().is_empty() || output_gate().is_empty();
      }

      bool is_unbounded() const
      {
        return codomain().is_unbounded();
      }

      bool contains(const TrajectoryVector& value) const
      {
        return true; // todo
      }

      const std::shared_ptr<Slice<T>> prev_slice() const
      {
        return std::static_pointer_cast<Slice<T>>(prev_abstract_slice());
      }

      std::shared_ptr<Slice<T>> prev_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice&>(*this).prev_slice());
      }

      const std::shared_ptr<Slice<T>> next_slice() const
      {
        return std::static_pointer_cast<Slice<T>>(next_abstract_slice());
      }

      std::shared_ptr<Slice<T>> next_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice&>(*this).next_slice());
      }

      const T& codomain() const
      {
        return _codomain;
      }

      T input_gate() const
      {
        T gate = codomain();
        if(prev_slice())
          gate &= prev_slice()->codomain();
        return gate;
      }

      T output_gate() const
      {
        T gate = codomain();
        if(next_slice())
          gate &= next_slice()->codomain();
        return gate;
      }

      void set(const T& x)
      {
        assert((size_t)codomain().size() == size());
        _codomain = x;
        if(is_gate())
          _codomain &= prev_slice()->codomain() & next_slice()->codomain();
      }

      void set_component(size_t i, const Interval& xi)
      {
        assert((size_t)codomain().size() == size());
        _codomain[i] = xi;
        if(is_gate())
          _codomain[i] &= prev_slice()->codomain()[i] & next_slice()->codomain()[i];
      }

      friend std::ostream& operator<<(std::ostream& os, const Slice& x)
      {
        os << x.t0_tf()
           << "↦" << x.codomain()
           << std::flush;
        return os;
      }


    protected:

      T _codomain;
  };

} // namespace codac

#endif