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
#include "codac_ConvexPolygon.h"
#include "codac_DynCtc.h"
#include <codac2_Domain.h>

#define EPSILON_CONTAINS ibex::next_float(0.) * 1000. //!< epsilon limit of the contains() algorithm

namespace codac2
{
  using codac::Interval;
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
        Slice(s, s._tubevector)
      {
        
      }

      Slice(const Slice& s, const AbstractSlicedTube& tubevector) :
        AbstractSlice(tubevector, s._it_tslice), _codomain(s._codomain)
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

      double volume() const
      {
        if constexpr(std::is_same<T,Interval>::value)
          return codomain().diam();
        else
          return codomain().volume();
      }

      bool is_gate() const
      {
        return t0_tf().is_degenerated();
      }

      bool is_empty() const
      {
        if(is_gate())
          return _codomain.is_empty();
        else
          return input_gate().is_empty() || output_gate().is_empty();
      }

      bool is_unbounded() const
      {
        return codomain().is_unbounded();
      }

      BoolInterval contains(const TrajectoryVector& x) const
      {
        if constexpr(!std::is_same<T,codac::IntervalVector>::value)
        {
          assert(false && "not implemented");
          return BoolInterval::MAYBE;
        }

        else
        {
          if(!t0_tf().intersects(x.tdomain()))
            return BoolInterval::MAYBE;

          BoolInterval is_contained = BoolInterval::YES;
          if(!t0_tf().is_subset(x.tdomain()))
            is_contained = BoolInterval::MAYBE;

          Interval t_ = t0_tf() & x.tdomain();
          T traj_tdomain(x(t_));
          // Using x(Interval(double)) for reliable evaluation:
          T traj_input(x(Interval(t_.lb())));
          T traj_output(x(Interval(t_.ub())));

          if(_codomain.intersects(traj_tdomain) == BoolInterval::NO
          || input_gate().intersects(traj_input) == BoolInterval::NO
          || output_gate().intersects(traj_output) == BoolInterval::NO)
            return BoolInterval::NO;

          else
          {
            if(!traj_input.is_subset(input_gate()) || !traj_output.is_subset(output_gate()))
              return BoolInterval::MAYBE;

            else if(traj_tdomain.is_subset(_codomain))
              return is_contained;

            else // too much pessimism for the trajectory evaluation on t0_tf()
            {
              // Bisections are performed to reach an accurate evaluation

              std::list<Interval> s_subtdomains;
              s_subtdomains.push_front(t_);

              while(!s_subtdomains.empty())
              {
                Interval t = s_subtdomains.front();
                s_subtdomains.pop_front();

                T thinner_eval(x(t));

                if(!_codomain.intersects(thinner_eval))
                {
                  return BoolInterval::NO;
                }

                else if(!thinner_eval.is_subset(_codomain))
                {
                  if(t.diam() < EPSILON_CONTAINS)
                    return BoolInterval::MAYBE;

                  s_subtdomains.push_front(Interval(t.lb(), t.lb() + t.diam() / 2.));
                  s_subtdomains.push_front(Interval(t.lb() + t.diam() / 2., t.ub()));
                }
              }

              return is_contained;
            }
          }
        }
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
        if(!prev_slice_ptr())
          return codomain();

        else
        {
          if(prev_slice_ptr()->is_gate())
            return prev_slice_ptr()->codomain();
          else
            return codomain() & prev_slice_ptr()->codomain();
        }
      }

      T output_gate() const
      {
        if(!next_slice_ptr())
          return codomain();

        else
        {
          if(next_slice_ptr()->is_gate())
            return next_slice_ptr()->codomain();
          else
            return codomain() & next_slice_ptr()->codomain();
        }
      }

      void set(const T& x, bool propagate = true)
      {
        if constexpr(!std::is_same<T,Interval>::value) { // 'if' to be removed with virtual set classes
          assert((size_t)codomain().size() == size());
        }

        _codomain = x;

        if(prev_slice_ptr())
        {
          if constexpr(!std::is_same<T,Interval>::value) { // 'if' to be removed with virtual set classes
            assert((size_t)prev_slice_ptr()->codomain().size() == size());
          }
          if(is_gate())
            _codomain &= prev_slice_ptr()->codomain();
          else if(prev_slice_ptr()->is_gate())
            prev_slice_ptr()->_codomain &= _codomain;
        }

        if(next_slice_ptr())
        {
          if constexpr(!std::is_same<T,Interval>::value) { // 'if' to be removed with virtual set classes
            assert((size_t)next_slice_ptr()->codomain().size() == size());
          }
          if(is_gate())
            _codomain &= next_slice_ptr()->codomain();
          else if(next_slice_ptr()->is_gate())
            next_slice_ptr()->_codomain &= _codomain;
        }

        if(propagate && is_empty())
          set_empty(true, codac::TimePropag::FORWARD | codac::TimePropag::BACKWARD);
      }

      void set_empty(bool propagate = true, codac::TimePropag t_propa = codac::TimePropag::FORWARD | codac::TimePropag::BACKWARD)
      {
        _codomain.set_empty();

        if(propagate)
        {
          if(t_propa & codac::TimePropag::BACKWARD && prev_slice_ptr())
            prev_slice_ptr()->set_empty(true, codac::TimePropag::BACKWARD);
          if(t_propa & codac::TimePropag::FORWARD && next_slice_ptr())
            next_slice_ptr()->set_empty(true, codac::TimePropag::FORWARD);
        }

        else if(!is_gate())
        {
          if(prev_slice_ptr() && prev_slice_ptr()->is_gate())
            prev_slice_ptr()->set_empty();
          if(next_slice_ptr() && next_slice_ptr()->is_gate())
            next_slice_ptr()->set_empty();
        }
      }

      void set_unbounded()
      {
        if constexpr(std::is_same<T,codac::IntervalVector>::value)
          _codomain = T(size());
        else
          _codomain = T();
        
        //if constexpr(std::is_same<T,Interval>::value || std::is_same<T,codac::ConvexPolygon>::value) // 'if' to be removed with virtual set classes
        //  _codomain = T();
        //else
        //  _codomain = T(size());
      }

      void set_component(size_t i, const Interval& xi)
      {
        assert((size_t)codomain().size() == size());
        _codomain[i] = xi;
        if(is_gate())
        {
          if(prev_slice_ptr())
            _codomain[i] &= prev_slice_ptr()->codomain()[i];
          if(next_slice_ptr())
            _codomain[i] &= next_slice_ptr()->codomain()[i];
        }
      }

      const Slice<T>& inflate(double rad)
      {
        assert(rad >= 0. && "cannot inflate negative value");
        _codomain.inflate(rad);
        return *this;
      }

      bool operator==(const Slice& x) const
      {
        return _codomain == x._codomain;
      }

      bool operator!=(const Slice& x) const
      {
        return _codomain != x._codomain;
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