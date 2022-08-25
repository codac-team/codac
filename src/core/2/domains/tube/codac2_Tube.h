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
#include "codac2_AbstractSlicedTube.h"
#include "codac2_AbstractConstTube.h"
#include "codac2_TDomain.h"

namespace codac2
{
  using codac::TFnc;

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

      explicit Tube(size_t n, const std::shared_ptr<TDomain>& tdomain) :
        AbstractSlicedTube(tdomain)
      {
        for(std::list<TSlice>::iterator it = _tdomain->_tslices.begin();
          it != _tdomain->_tslices.end(); ++it)
        {
          it->_slices.insert(
            std::pair<const AbstractSlicedTube*,std::shared_ptr<Slice<T>>>(this,
              std::make_shared<Slice<T>>(n, *this, it)));
        }
      }

      explicit Tube(size_t n, const std::shared_ptr<TDomain>& tdomain, const TFnc& f) :
        Tube(n, tdomain)
      {
        assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");
        assert((size_t)f.image_dim() == n);

        for(auto& s : *this)
          s.set(f.eval_vector(s.t0_tf()));
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

      virtual const std::shared_ptr<AbstractSlice>& first_abstract_slice() const
      {
        return _tdomain->tslices().front().slices().at(this);
      }

      virtual const std::shared_ptr<AbstractSlice>& last_abstract_slice() const
      {
        return _tdomain->tslices().back().slices().at(this);
      }

      const std::shared_ptr<Slice<T>> first_slice_ptr() const
      {
        return std::static_pointer_cast<Slice<T>>(first_abstract_slice());
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
        return std::static_pointer_cast<Slice<T>>(last_abstract_slice());
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

      bool contains(const TrajectoryVector& value) const
      {
        return true;
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

      TubeComponent<T> operator[](size_t i)
      {
        assert(i >= 0 && i < size());
        return TubeComponent<T>(*this, i);
      }
    
      codac::TubeVector to_codac1() const
      {
        codac::TubeVector x(t0_tf(), size());
        for(const auto& s : *this)
          if(!s.t0_tf().is_unbounded())
            x.set(s.codomain(), s.t0_tf());
        for(const auto& s : *this) // setting gate (were overwritten)
          if(s.t0_tf().is_degenerated())
            x.set(s.codomain(), s.t0_tf());
        return x;
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
          
          iterator(const Tube& tube_vector, base_container::iterator it) : 
            base_container::iterator(it), _tube_vector(tube_vector) { }

          reference operator*()
          {
            return static_cast<reference>(*((*this)->_slices.at(&_tube_vector)));
          }

        protected:

          const Tube& _tube_vector;
      };

      iterator begin() { return iterator(*this, _tdomain->_tslices.begin()); }
      iterator end()   { return iterator(*this, _tdomain->_tslices.end()); }


      struct const_iterator : public base_container::const_iterator
      {
        using iterator_category = typename base_container::const_iterator::iterator_category;
        using difference_type   = typename base_container::const_iterator::difference_type;

        using value_type        = Slice<T>;
        using pointer           = const Slice<T>*;
        using reference         = const Slice<T>&;

        public:
          
          const_iterator(const Tube& tube_vector, base_container::const_iterator it) : 
            base_container::const_iterator(it), _tube_vector(tube_vector) { }

          reference operator*() const
          {
            return static_cast<reference>(*((*this)->_slices.at(&_tube_vector)));
          }

        protected:

          const Tube& _tube_vector;
      };

      const_iterator begin() const { return const_iterator(*this, _tdomain->_tslices.cbegin()); }
      const_iterator end() const   { return const_iterator(*this, _tdomain->_tslices.cend()); }
  };


  template<class T>
  class TubeEvaluation
  {
    public:

      const TubeEvaluation& operator=(const T& x)
      {
        // Sampling the tube only if affectation is performed
        // (i.e. this is not done in the constructor)
        std::list<TSlice>::iterator it_lb = _tubevector->_tdomain->sample(_t.lb(), false);
        std::list<TSlice>::iterator it_ub = _tubevector->_tdomain->sample(_t.ub(), _t.is_degenerated());

        do
        {
          std::static_pointer_cast<Slice<T>>(it_lb->_slices.at(_tubevector))->set(x);
          it_lb++;
        } while(it_lb != it_ub);

        return *this;
      }

      explicit operator T()
      {
        return _tubevector->eval(_t);
      }

      friend std::ostream& operator<<(std::ostream& os, const TubeEvaluation<T>& x)
      {
        os << x._tubevector->eval(x._t) << std::flush;
        return os;
      }
      

    protected:

      explicit TubeEvaluation(const Tube<T>* tubevector, double t) :
        _t(Interval(t)), _tubevector(tubevector)
      {

      }

      explicit TubeEvaluation(const Tube<T>* tubevector, const Interval& t) :
        _t(t), _tubevector(tubevector)
      {

      }

      const Interval _t;
      const Tube<T>* _tubevector;
  };


  template<class T>
  class TubeComponent //: public AbstractConstTube<Interval,TubeComponent<T>>
  {
    protected:

      TubeComponent(Tube<T>& tubevector, size_t i) :
        _i(i), _tubevector(tubevector)
      {
        assert(i >= 0 && i < tubevector.size());
      }

    public:

      TubeComponent(const TubeComponent<T>& tubevector_i) :
        _i(tubevector_i._i), _tubevector(tubevector_i._tubevector)
      {

      }
      
      size_t size() const
      {
        return 1;
      }

      const std::shared_ptr<TDomain>& tdomain() const
      {
        return _tubevector.tdomain();
      }
      
      Interval t0_tf() const
      {
        return _tubevector.t0_tf();
      }
      
      Interval codomain() const
      {
        Interval codomain(Interval::EMPTY_SET);
        for(const auto& s : _tubevector)
          codomain |= s.codomain()[_i];
        return codomain;
      }

      bool contains(const Trajectory& value) const
      {
        assert(false);
        return true;
      }
      
      void set(const Interval& codomain)
      {
        for(auto& s : _tubevector)
          s.set_component(_i, codomain);
      }

      const TubeComponent<T>& operator=(const TubeComponent<T>& x)
      {
        assert(x.tdomain() == tdomain());
        for(auto& s : _tubevector)
          s.set_component(_i, std::static_pointer_cast<Slice<T>>(s._it_tslice->_slices.at(&x._tubevector))->codomain()[x._i]);
        return *this;
      }

      const TubeComponent<T>& operator=(std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> rel)
      {
        assert(rel.second.tdomain() == tdomain());
        for(auto& s : _tubevector)
          s.set_component(_i, rel.first(std::static_pointer_cast<Slice<T>>(s._it_tslice->_slices.at(&rel.second._tubevector))->codomain()[rel.second._i]));
        return *this;
      }

      friend std::ostream& operator<<(std::ostream& os, const TubeComponent<T>& x)
      {
        os << "Component " << x._i << " of: " << x._tubevector << std::flush;
        return os;
      }

      std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> cos(const TubeComponent<T>& x)
      {
        return std::make_pair(static_cast<Interval(*)(const Interval&)>(ibex::cos), x);
      }
      
      codac::Tube to_codac1() const
      {
        codac::Tube x(t0_tf());
        for(const auto& s : _tubevector)
          if(!s.t0_tf().is_unbounded())
            x.set(s.codomain()[_i], s.t0_tf());
        for(const auto& s : _tubevector) // setting gate (were overwritten)
          if(s.t0_tf().is_degenerated())
            x.set(s.codomain()[_i], s.t0_tf());
        return x;
      }
      

    protected:

      size_t _i;
      Tube<T>& _tubevector;

      template<typename U>
      friend class Tube;
  };

  template<class T>
  std::pair<std::function<Interval(const Interval&)>,const TubeComponent<T>> cos(const TubeComponent<T>& x);

} // namespace codac

#endif