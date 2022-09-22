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
#include "codac_BoolInterval.h"

#define EPSILON_CONTAINS ibex::next_float(0.) * 1000. //!< epsilon limit of the contains() algorithm

namespace codac2
{
  using codac::Interval;
  using codac::IntervalVector;
  using codac::TrajectoryVector;
  using codac::BoolInterval;

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

      BoolInterval contains(const TrajectoryVector& x) const
      {
        assert(t0_tf().is_subset(x.tdomain()));

        /*IntervalVector traj_tdomain = x(t0_tf());
        // Using x(Interval(double)) for reliable evaluation:
        IntervalVector traj_input = x(Interval(t0_tf().lb()));
        IntervalVector traj_output = x(Interval(t0_tf().ub()));

        if(_codomain.intersects(traj_tdomain) == BoolInterval::NO
        || input_gate().intersects(traj_input) == BoolInterval::NO
        || output_gate().intersects(traj_output) == BoolInterval::NO)
          return BoolInterval::NO;

        else
        {
          if(!input_gate().is_superset(traj_input) || !output_gate().is_superset(traj_output))
            return BoolInterval::MAYBE;

          else if(_codomain.is_superset(traj_tdomain))
            return BoolInterval::YES;

          else // too much pessimism for the trajectory evaluation on t0_tf()
          {
            // Bisections are performed to reach an accurate evaluation

            std::list<Interval> s_subtdomains;
            s_subtdomains.push_front(t0_tf());

            while(!s_subtdomains.empty())
            {
              Interval t = s_subtdomains.front();
              s_subtdomains.pop_front();

              IntervalVector thinner_eval = x(t);

              if(!_codomain.intersects(thinner_eval))
              {
                return BoolInterval::NO;
              }

              else if(!_codomain.is_superset(thinner_eval))
              {
                if(t.diam() < EPSILON_CONTAINS)
                  return BoolInterval::MAYBE;

                s_subtdomains.push_front(Interval(t.lb(), t.lb() + t.diam() / 2.));
                s_subtdomains.push_front(Interval(t.lb() + t.diam() / 2., t.ub()));
              }
            }

            return BoolInterval::YES;
          }
        }*/

        // todo
        return BoolInterval::YES;
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
        if(!is_gate() && prev_slice_ptr())
          gate &= prev_slice_ptr()->codomain();
        return gate;
      }

      T output_gate() const
      {
        T gate = codomain();
        if(!is_gate() && next_slice_ptr())
          gate &= next_slice_ptr()->codomain();
        return gate;
      }

      void set(const T& x)
      {
        if constexpr(!std::is_same<T,Interval>::value)
          assert((size_t)codomain().size() == size());
        _codomain = x;
        if(is_gate())
          _codomain &= prev_slice_ptr()->codomain() & next_slice_ptr()->codomain();
      }

      void set_empty()
      {
        _codomain.set_empty();
        if(!is_gate())
        {
          if(prev_slice_ptr()->is_gate())
            prev_slice_ptr()->set_empty();
          if(next_slice_ptr()->is_gate())
            next_slice_ptr()->set_empty();
        }
      }

      void set_component(size_t i, const Interval& xi)
      {
        assert((size_t)codomain().size() == size());
        _codomain[i] = xi;
        if(is_gate())
          _codomain[i] &= prev_slice_ptr()->codomain()[i] & next_slice_ptr()->codomain()[i];
      }

      const Slice<T>& inflate(double rad)
      {
        assert(rad >= 0. && "cannot inflate negative value");
        _codomain.inflate(rad);
        return *this;
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