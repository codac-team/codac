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

      explicit Slice(const T& codomain, const AbstractSlicedTube& tube_vector, const std::list<TSlice>::iterator& it_tslice) :
        AbstractSlice(tube_vector,it_tslice), _codomain(codomain)
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
        // todo: define size() method in Interval class
        if constexpr(std::is_same<T,Interval>::value)
          return 1;
        else
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

      const std::shared_ptr<Slice<T>> prev_slice_ptr() const
      {
        return std::static_pointer_cast<Slice<T>>(prev_abstract_slice_ptr());
      }

      std::shared_ptr<Slice<T>> prev_slice_ptr()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice&>(*this).prev_slice_ptr());
      }

      const std::shared_ptr<Slice<T>> next_slice_ptr() const
      {
        return std::static_pointer_cast<Slice<T>>(next_abstract_slice_ptr());
      }

      std::shared_ptr<Slice<T>> next_slice_ptr()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice&>(*this).next_slice_ptr());
      }

      const T& codomain() const
      {
        return _codomain;
      }

      T input_gate() const
      {
        T gate = codomain();
        if(prev_slice_ptr())
          gate &= prev_slice_ptr()->codomain();
        return gate;
      }

      T output_gate() const
      {
        T gate = codomain();
        if(next_slice_ptr())
          gate &= next_slice_ptr()->codomain();
        return gate;
      }

      void set(const T& x)
      {
        assert((size_t)codomain().size() == size());
        _codomain = x;
        if(is_gate())
          _codomain &= prev_slice_ptr()->codomain() & next_slice_ptr()->codomain();
      }

      void set_component(size_t i, const Interval& xi)
      {
        assert((size_t)codomain().size() == size());
        _codomain[i] = xi;
        if(is_gate())
          _codomain[i] &= prev_slice_ptr()->codomain()[i] & next_slice_ptr()->codomain()[i];
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