/** 
 *  \file codac2_Slice.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_SliceBase.h"

namespace codac2
{
  template<class T>
  class SlicedTube;

  template<class T>
  class Slice : public SliceBase,
    protected T
    // T class inheritance is protected, because modifying a
    // Slice's codomain can affect adjacent gates if they exist
  {
    public:

      explicit Slice(const SlicedTubeBase& tube, const std::list<TSlice>::iterator& it_tslice, const T& codomain)
        : SliceBase(tube, it_tslice), T(codomain)
      { }

      Slice(const Slice& s, const SlicedTubeBase& tube)
        : SliceBase(tube, s._it_tslice), T(s.codomain())
      { }

      // Slice objects cannot be copyable or movable,
      // as they are supposed to be connected to other Slice objects
      Slice(const Slice& s) = delete;
      Slice& operator=(const Slice&) = delete;
      Slice(Slice&&) = delete;
      Slice& operator=(Slice&&) = delete;

      inline const SlicedTube<T>& tube() const
      {
        return static_cast<const SlicedTube<T>&>(_tube);
      }

      inline virtual std::shared_ptr<SliceBase> copy() const
      {
        return std::make_shared<Slice>(*this, this->_tube);
      }

      inline Index size() const
      {
        return this->T::size();
      }

      inline T& codomain()
      {
        return (T&)(*this);
      }

      inline const T& codomain() const
      {
        return (const T&)(*this);
      }

      inline bool is_gate() const
      {
        return t0_tf().is_degenerated();
      }

      inline std::shared_ptr<const Slice<T>> prev_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SliceBase::prev_slice());
      }

      inline std::shared_ptr<Slice<T>> prev_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice<T>&>(*this).prev_slice());
      }

      inline std::shared_ptr<const Slice<T>> next_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SliceBase::next_slice());
      }

      inline std::shared_ptr<Slice<T>> next_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice<T>&>(*this).next_slice());
      }

      inline T input_gate() const
      {
        if(!prev_slice())
          return codomain();

        else
        {
          if(prev_slice()->is_gate())
            return prev_slice()->codomain();
          else
            return codomain() & prev_slice()->codomain();
        }
      }

      inline T output_gate() const
      {
        if(!next_slice())
          return codomain();

        else
        {
          if(next_slice()->is_gate())
            return next_slice()->codomain();
          else
            return codomain() & next_slice()->codomain();
        }
      }

      inline std::pair<T,T> enclosed_bounds(const Interval& t) const
      {
        T x = *this; x.set_empty(); 
        auto bounds = std::make_pair(x,x);

        if(t.lb() < t0_tf().lb() || t.ub() > t0_tf().ub())
        {
          x.init(Interval(-oo,0));
          bounds.first |= x;
          x.init(Interval(0,oo));
          bounds.second |= x;
        }

        if(t.contains(t0_tf().lb()))
        {
          bounds.first |= input_gate().lb();
          bounds.second |= input_gate().ub();
        }

        if(t.contains(t0_tf().ub()))
        {
          bounds.first |= output_gate().lb();
          bounds.second |= output_gate().ub();
        }

        if(t.is_subset(t0_tf()) && t != t0_tf().lb() && t != t0_tf().ub())
        {
          bounds.first |= this->lb();
          bounds.second |= this->ub();
        }

        return bounds;
      }

      inline void set(const T& x, bool propagate = true)
      {
        assert_release(x.size() == this->size());
        codomain() = x;
        if(propagate)
          update_adjacent_codomains();
      }

      inline void init()
      {
        this->T::init();
        // Nothing to propagate to adjacent codomains
      }

      inline void set_empty()
      {
        set_empty(true);
      }

      inline bool operator==(const Slice& x) const
      {
        return codomain() == x.codomain();
      }

      friend inline std::ostream& operator<<(std::ostream& os, const Slice& x)
      {
        os << x.t0_tf()
           << "↦" << x.codomain()
           << std::flush;
        return os;
      }

    protected:

      template<typename T_>
      friend class SlicedTube;

      inline void set_empty(bool propagate)
      {
        this->T::set_empty();
        if(propagate)
          update_adjacent_codomains();
      }

      inline void update_adjacent_codomains()
      {
        if(prev_slice())
        {
          assert(prev_slice()->size() == this->size());
          if(is_gate())
            codomain() &= prev_slice()->codomain();
          else if(prev_slice()->is_gate())
            prev_slice()->codomain() &= codomain();
        }

        if(next_slice())
        {
          assert(next_slice()->size() == this->size());
          if(is_gate())
            codomain() &= next_slice()->codomain();
          else if(next_slice()->is_gate())
            next_slice()->codomain() &= codomain();
        }
      }
  };
}