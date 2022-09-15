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

#ifndef __CODAC2_TUBE_H__
#define __CODAC2_TUBE_H__

#include <list>
#include <memory>
#include "codac_TFnc.h"
#include "codac2_TSlice.h"
#include "codac_Tube.h" // to be removed
#include "codac_TubeVector.h" // to be removed
#include "codac2_AbstractSlicedTube.h"
#include "codac2_AbstractConstTube.h"
#include "codac2_TDomain.h"
#include "codac_ConvexPolygon.h"

namespace codac2
{
  using codac::TFnc;
  using codac::BoolInterval;

  template<class T>
  class Slice;
  template<class T>
  class TubeEvaluation;
  template<class T>
  class TubeComponent;


  template<class T>
  class Tube : public AbstractSlicedTube, public AbstractConstTube<T,Tube<T>>
  {
    public:

      explicit Tube(const std::shared_ptr<TDomain>& tdomain) :
        Tube(tdomain, T())
      {
        
      }

      explicit Tube(const std::shared_ptr<TDomain>& tdomain, const TFnc& f) :
        Tube(tdomain, T())
      {
        assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");
        if constexpr(std::is_same<T,Interval>::value)
          assert(f.image_dim() == 1);

        for(auto& s : *this)
        {
          if constexpr(std::is_same<T,Interval>::value)
            s.set(f.eval(s.t0_tf()));

          else
            s.set(f.eval_vector(s.t0_tf()));
        }
      }

      explicit Tube(const std::shared_ptr<TDomain>& tdomain, const T& default_value) :
        AbstractSlicedTube(tdomain)
      {
        for(std::list<TSlice>::iterator it = _tdomain->_tslices.begin();
          it != _tdomain->_tslices.end(); ++it)
        {
          it->_slices.insert(
            std::pair<const AbstractSlicedTube*,std::shared_ptr<Slice<T>>>(this,
              std::make_shared<Slice<T>>(default_value, *this, it)));
        }
      }

      explicit Tube(const Tube& x) :
        AbstractSlicedTube(x.tdomain())
      {
        for(std::list<TSlice>::iterator it = _tdomain->_tslices.begin();
          it != _tdomain->_tslices.end(); ++it)
        {
          it->_slices.insert(
            std::pair<const AbstractSlicedTube*,std::shared_ptr<Slice<T>>>(this,
              std::make_shared<Slice<T>>(it->_slices.at(&x)->size(), *this, it)));
        }
      }

      ~Tube()
      {
        for(auto& s : _tdomain->_tslices)
          s._slices.erase(this);
      }
      
      virtual Interval t0_tf() const
      {
        return AbstractSlicedTube::t0_tf();
      }

      size_t size() const
      {
        // todo: define size() method in Interval class
        if constexpr(std::is_same<T,Interval>::value)
          return 1;
        else
          return first_slice().size();
      }

      size_t nb_slices() const
      {
        return _tdomain->nb_tslices();
      }

      virtual const std::shared_ptr<AbstractSlice>& first_abstract_slice_ptr() const
      {
        return _tdomain->tslices().front().slices().at(this);
      }

      virtual const std::shared_ptr<AbstractSlice>& last_abstract_slice_ptr() const
      {
        return _tdomain->tslices().back().slices().at(this);
      }

      const std::shared_ptr<Slice<T>> first_slice_ptr() const
      {
        return std::static_pointer_cast<Slice<T>>(first_abstract_slice_ptr());
      }

      const Slice<T>& first_slice() const
      {
        return *first_slice_ptr();
      }

      Slice<T>& first_slice()
      {
        return const_cast<Slice<T>&>(
          static_cast<const Tube&>(*this).first_slice());
      }

      const std::shared_ptr<Slice<T>> last_slice_ptr() const
      {
        return std::static_pointer_cast<Slice<T>>(last_abstract_slice_ptr());
      }

      const Slice<T>& last_slice() const
      {
        return *last_slice_ptr();
      }

      Slice<T>& last_slice()
      {
        return const_cast<Slice<T>&>(
          static_cast<const Tube&>(*this).last_slice());
      }

      bool is_empty() const
      {
        for(const auto& s : *this)
          if(s.is_empty())
            return true;
        return false;
      }

      bool is_unbounded() const
      {
        for(const auto& s : *this)
          if(s.is_unbounded())
            return true;
        return false;
      }

      BoolInterval contains(const TrajectoryVector& x) const
      {
        assert(x.tdomain() == tdomain()->t0_tf());

        BoolInterval result = BoolInterval::YES;
        for(const auto& s : *this)
        {
          BoolInterval b = s.contains(x);
          if(b == BoolInterval::NO) return BoolInterval::NO;
          else if(b == BoolInterval::MAYBE) result = BoolInterval::MAYBE; 
        }

        return result;
      }

      T codomain() const
      {
        T codomain = first_slice().codomain();
        codomain.set_empty();
        for(const auto& s : *this)
          codomain |= s.codomain();
        return codomain;
      }
      
      TubeEvaluation<T> operator()(double t)
      {
        return TubeEvaluation<T>(this, t);
      }
      
      TubeEvaluation<T> operator()(const Interval& t)
      {
        return TubeEvaluation<T>(this, t);
      }
      
      T eval(double t) const
      {
        return std::static_pointer_cast<Slice<T>>(_tdomain->iterator_tslice(t)->_slices.at(this))->codomain();
      }
      
      T eval(const Interval& t) const
      {
        std::list<TSlice>::iterator it = _tdomain->iterator_tslice(t.lb());
        T codomain = std::static_pointer_cast<Slice<T>>(it->_slices.at(this))->codomain();

        while(it != _tdomain->iterator_tslice(t.ub()))
        {
          codomain |= std::static_pointer_cast<Slice<T>>(it->_slices.at(this))->codomain();
          it++;
        }

        return codomain;
      }

      void set(const T& codomain)
      {
        assert((size_t)codomain.size() == size());
        for(auto& s : *this)
          s.set(codomain);
      }

      void set(const T& codomain, double t)
      {
        assert((size_t)codomain.size() == size());
        std::list<TSlice>::iterator it = _tdomain->sample(t);
        std::static_pointer_cast<Slice<T>>(it->_slices.at(this))->set(codomain);
      }

      const Tube<T>& inflate(double rad)
      {
        for(auto& s : *this)
          if(!s.is_gate())
            s.inflate(rad);
        for(auto& s : *this)
          if(s.is_gate())
            s.inflate(rad);
        return *this;
      }

      TubeComponent<T> operator[](size_t i)
      {
        assert(i >= 0 && i < size());
        return TubeComponent<T>(*this, i);
      }

      friend std::ostream& operator<<(std::ostream& os, const Tube<T>& x)
      {
        x.AbstractConstTube<T,Tube<T>>::print(os);
        // Adding information related to sliced structure
        os << ", " << x.nb_slices()
           << " slice" << (x.nb_slices() > 1 ? "s" : "")
           << std::flush;
        return os;
      }


    public:

      using base_container = std::list<TSlice>;

      struct iterator : public base_container::iterator
      {
        using iterator_category = typename base_container::iterator::iterator_category;
        using difference_type   = typename base_container::iterator::difference_type;

        using value_type        = Slice<T>;
        using pointer           = Slice<T>*;
        using reference         = Slice<T>&;

        public:
          
          iterator(const Tube& x, base_container::iterator it) : 
            base_container::iterator(it), _x(x) { }

          reference operator*()
          {
            return static_cast<reference>(*((*this)->_slices.at(&_x)));
          }

        protected:

          const Tube& _x;
      };

      iterator begin() { return iterator(*this, _tdomain->_tslices.begin()); }
      iterator end()   { return iterator(*this, _tdomain->_tslices.end()); }

      struct reverse_iterator : public base_container::reverse_iterator
      {
        using iterator_category = typename base_container::reverse_iterator::iterator_category;
        using difference_type   = typename base_container::reverse_iterator::difference_type;

        using value_type        = Slice<T>;
        using pointer           = Slice<T>*;
        using reference         = Slice<T>&;

        public:
          
          reverse_iterator(const Tube& x, base_container::reverse_iterator it) : 
            base_container::reverse_iterator(it), _x(x) { }

          reference operator*()
          {
            return static_cast<reference>(*((*this)->_slices.at(&_x)));
          }

        protected:

          const Tube& _x;
      };

      reverse_iterator rbegin() { return reverse_iterator(*this, _tdomain->_tslices.rbegin()); }
      reverse_iterator rend()   { return reverse_iterator(*this, _tdomain->_tslices.rend()); }

      struct const_iterator : public base_container::const_iterator
      {
        using iterator_category = typename base_container::const_iterator::iterator_category;
        using difference_type   = typename base_container::const_iterator::difference_type;

        using value_type        = Slice<T>;
        using pointer           = const Slice<T>*;
        using reference         = const Slice<T>&;

        public:
          
          const_iterator(const Tube& x, base_container::const_iterator it) : 
            base_container::const_iterator(it), _x(x) { }

          reference operator*() const
          {
            return static_cast<reference>(*((*this)->_slices.at(&_x)));
          }

        protected:

          const Tube& _x;
      };

      const_iterator begin() const { return const_iterator(*this, _tdomain->_tslices.cbegin()); }
      const_iterator end() const   { return const_iterator(*this, _tdomain->_tslices.cend()); }
  };

  codac::Tube to_codac1(const Tube<Interval>& x);
  codac::TubeVector to_codac1(const Tube<IntervalVector>& x);


  #include "codac2_TubeEvaluation.h"
  #include "codac2_TubeComponent.h"
  
} // namespace codac

#endif