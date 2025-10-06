/** 
 *  \file codac2_SlicedTube.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_TDomain.h"
#include "codac2_Slice.h"
#include "codac2_SlicedTubeBase.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_CtcDeriv.h"
#include "codac2_Tube_operator.h"

namespace codac2
{
  template<typename T>
  class SlicedTube : public SlicedTubeBase
  {
    public:

      explicit SlicedTube(const std::shared_ptr<TDomain>& tdomain,
        const T& codomain)
        : SlicedTubeBase(tdomain)
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*this, it, codomain)
          });
      }

      explicit SlicedTube(const std::shared_ptr<TDomain>& tdomain,
        const AnalyticFunction<typename ExprType<T>::Type>& f)
        : SlicedTubeBase(tdomain)
      {
        assert_release(f.args().size() == 1
          && "function's inputs must be limited to one system variable");
      
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*this, it, f.eval((Interval)*it))
          });
      }

      template<typename V>
        requires std::is_same_v<typename Wrapper<V>::Domain,T>
      explicit SlicedTube(const std::shared_ptr<TDomain>& tdomain,
        const SampledTraj<V>& f)
        : SlicedTubeBase(tdomain)
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*this, it, f((Interval)*it))
          });
      }

      SlicedTube(const SlicedTube<T>& x)
        : SlicedTubeBase(x.tdomain())
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*x(it), *this)
          });
      }

      inline SlicedTube& operator=(const SlicedTube& x)
      {
        assert_release(_tdomain == x._tdomain);

        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          (*this)(it)->set(x(it)->codomain(), false);

        return *this;
      }
      
      inline Index size() const
      {
        return first_slice()->size();
      }

      virtual std::pair<Index,Index> shape() const
      {
        if constexpr(std::is_same_v<typename ExprType<T>::Type,ScalarType>)
          return {1,1};
        else
          return {first_slice()->codomain().rows(),first_slice()->codomain().cols()};
      }

      inline double volume() const
      {
        double volume = 0.;
        for(const auto& s : *this)
          if(!s.is_gate())
            volume += s.volume();
        return volume;
      }

      inline std::shared_ptr<Slice<T>> first_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).first_slice());
      }

      inline std::shared_ptr<const Slice<T>> first_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SlicedTubeBase::first_slice());
      }

      inline std::shared_ptr<Slice<T>> last_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).last_slice());
      }

      inline std::shared_ptr<const Slice<T>> last_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SlicedTubeBase::last_slice());
      }

      inline bool is_empty() const
      {
        // Fast evaluation by considering gates first, then envelopes,
        // which allows to quickly identify an empty set
        for(const auto& s : *this)
          if(s.is_gate() && s.is_empty())
            return true;
        for(const auto& s : *this)
          if(!s.is_gate() && s.is_empty())
            return true;
        return false;
      }

      inline bool is_unbounded() const
      {
        for(const auto& s : *this)
          if(s.is_unbounded())
            return true;
        return false;
      }

      inline T codomain() const
      {
        T x = first_slice()->codomain();
        for(const auto& s : *this)
          x |= s.codomain();
        return x;
      }
      
      inline std::shared_ptr<Slice<T>> operator()(const std::list<TSlice>::iterator& it)
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).operator()(it));
      }
      
      inline std::shared_ptr<const Slice<T>> operator()(const std::list<TSlice>::const_iterator& it) const
      {
        return std::static_pointer_cast<const Slice<T>>(
          it->slices().at(this));
      }
      
      inline std::shared_ptr<Slice<T>> operator()(const std::list<TSlice>::reverse_iterator& it)
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).operator()(it));
      }
      
      inline std::shared_ptr<const Slice<T>> operator()(const std::list<TSlice>::const_reverse_iterator& it) const
      {
        return std::static_pointer_cast<const Slice<T>>(
          it->slices().at(this));
      }
      
      inline T operator()(double t) const
      {
        if(!tdomain()->t0_tf().contains(t))
          return all_reals_codomain();

        auto it_t = _tdomain->tslice(t);
        assert(it_t != _tdomain->end());
        T x = (*this)(it_t)->codomain();
        if(!it_t->is_gate() && t==it_t->lb() && it_t!=_tdomain->begin())
          x &= (*this)(--it_t)->codomain();
        return x;
      }
      
      inline T operator()(const Interval& t) const
      {
        if(!tdomain()->t0_tf().is_superset(t))
          return all_reals_codomain();

        if(t.is_degenerated())
          return (*this)(t.lb());

        auto t_ = t & _tdomain->t0_tf();
        
        auto it = _tdomain->tslice(t_.lb());
        T codomain = (*this)(it)->codomain();

        while(it != std::next(_tdomain->tslice(t_.ub())))
        {
          if(it->lb() == t_.ub()) break;
          codomain |= (*this)(it)->codomain();
          it++;
        }

        return codomain;
      }

      std::pair<T,T> enclosed_bounds(const Interval& t) const
      {
        auto x = this->empty_codomain();
        auto bounds = std::make_pair(x,x);

        if(t.lb() < _tdomain->t0_tf().lb() || t.ub() > _tdomain->t0_tf().ub())
        {
          x.init(Interval(-oo,0));
          bounds.first |= x;
          x.init(Interval(0,oo));
          bounds.second |= x;
        }

        Interval t_inter = t & _tdomain->t0_tf();
        auto si = (*this)(_tdomain->tslice(t_inter.lb()));

        while(si && si->t0_tf().lb() <= t_inter.ub())
        {
          auto slice_bounds = si->enclosed_bounds(t_inter & si->t0_tf());
          bounds.first |= slice_bounds.first;
          bounds.second |= slice_bounds.second;
          si = si->next_slice();
        }

        return bounds;
      }

      inline void set(const T& codomain)
      {
        assert_release(codomain.size() == this->size());

        for(auto& s : *this)
          if(!s.is_gate())
            s.set(codomain, false);

        for(auto& s : *this)
          if(s.is_gate())
            s.set(codomain, false);
      }

      inline void set(const T& codomain, double t)
      {
        assert_release(codomain.size() == this->size());
        (*this)(_tdomain->sample(t,true))->set(codomain);
      }

      inline void set(const T& codomain, const Interval& t)
      {
        auto it_lb = _tdomain->sample(t.lb(), t.is_degenerated());
        std::list<TSlice>::iterator it_ub;
        
        if(!t.is_degenerated())
        {
          it_ub = _tdomain->sample(t.ub(), false);

          if(it_ub->lb() == t.ub())
            it_ub--; // pointing to the tslice [..,t.ub()]

          if(it_lb->ub() == t.lb())
            it_lb++;
        }

        else
          it_ub = it_lb;

        do
        {
          (*this)(it_lb)->set(codomain);
        } while(it_lb != it_ub && (++it_lb) != _tdomain->end());
      }

      inline void set_ith_slice(const T& codomain, Index i)
      {
        Index j = 0;
        for(auto& si : *this)
          if(j++ == i)
          {
            si.set(codomain);
            break;
          }
      }

      const SlicedTube<T>& inflate(double rad)
      {
        assert_release(rad >= 0.);

        for(auto& s : *this)
          if(!s.is_gate())
            s.set(T(s.codomain()).inflate(rad), false);

        for(auto& s : *this)
          if(s.is_gate())
            s.set(T(s.codomain()).inflate(rad), false);

        return *this;
      }

      SlicedTube<Interval> operator[](Index i) const
      {
        assert_release(i >= 0 && i < size());
        SlicedTube<Interval> xi(tdomain(), Interval());
        for(auto it = tdomain()->begin() ; it != tdomain()->end() ; it++)
          xi(it)->codomain() = (*this)(it)->codomain()[i];
        return xi;
      }

      SlicedTube<IntervalVector> subvector(Index i, Index j) const
      {
        assert_release(i >= 0 && i <= j && j < size());
        SlicedTube<IntervalVector> xij(tdomain(), IntervalVector(j-i+1));
        for(auto it = tdomain()->begin() ; it != tdomain()->end() ; it++)
          xij(it)->codomain() = (*this)(it)->codomain().subvector(i,j);
        return xij;
      }

      inline bool operator==(const SlicedTube& x) const
      {
        if(!TDomain::are_same(tdomain(), x.tdomain()))
          return false;

        auto it_this = _tdomain->begin();
        auto it_x = x.tdomain()->cbegin();

        while(it_this != _tdomain->end())
          if(*(*this)(it_this++) != *x(it_x++))
            return false;

        return true;
      }

      inline SlicedTube operator&=(const SlicedTube& x)
      {
        assert(TDomain::are_same(tdomain(), x.tdomain()));
        auto it_this = _tdomain->begin();
        auto it_x = x.tdomain()->cbegin();

        while(it_this != _tdomain->end())
        {
          auto s = (*this)(it_this);
          s->set(s->codomain() & x(it_x)->codomain());
          it_this++; it_x++;
        }

        assert(it_x == x.tdomain()->cend());
        return *this;
      }

      friend inline std::ostream& operator<<(std::ostream& os, const SlicedTube<T>& x)
      {
        os << x.t0_tf()
           << "↦" << x.codomain()
           << ", " << x.nb_slices()
           << " slice" << (x.nb_slices() > 1 ? "s" : "")
           << std::flush;
        return os;
      }

      AnalyticFunction<typename ExprType<T>::Type> as_function() const
      {
        ScalarVar t;
        return {{t},
          AnalyticExprWrapper<typename ExprType<T>::Type>(
            std::make_shared<AnalyticOperationExpr<
              TubeOp<SlicedTube<T>>,typename ExprType<T>::Type,ScalarType>>(*this,t))
        };
      }

      // Integral related methods

      T integral(const Interval& t) const;
      T integral(const Interval& t1, const Interval& t2) const;
      std::pair<T,T> partial_integral(const Interval& t) const;
      std::pair<T,T> partial_integral(const Interval& t1, const Interval& t2) const;

      inline SlicedTube<T> primitive() const
      {
        auto x0 = all_reals_codomain();
        x0.init(0.);
        return primitive(x0);
      }

      inline SlicedTube<T> primitive(const T& x0) const
      {
        auto x = all_reals_codomain();
        auto p = SlicedTube<T>(this->tdomain(), x);
        p.set(x0, this->tdomain()->t0_tf().lb()); // may create an unwanted gate
        CtcDeriv c;
        c.contract(p,*this);
        return p;
      }


    public:

      using base_container = std::list<TSlice>;

      struct iterator : public base_container::iterator
      {
        public:
          
          iterator(SlicedTube& x, base_container::iterator it)
            : base_container::iterator(it), _x(x) { }

          std::shared_ptr<Slice<T>> operator->()
          {
            return _x(*this);
          }

          Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          SlicedTube& _x;
      };

      iterator begin() { return { *this, _tdomain->begin() }; }
      iterator end()   { return { *this, _tdomain->end() }; }

      struct reverse_iterator : public base_container::reverse_iterator
      {
        public:
          
          reverse_iterator(SlicedTube& x, base_container::reverse_iterator it)
            : base_container::reverse_iterator(it), _x(x) { }

          std::shared_ptr<Slice<T>> operator->()
          {
            return _x(*this);
          }

          Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          SlicedTube& _x;
      };

      reverse_iterator rbegin() { return { *this, _tdomain->rbegin() }; }
      reverse_iterator rend()   { return { *this, _tdomain->rend() }; }

      struct const_iterator : public base_container::const_iterator
      {
        public:
          
          const_iterator(const SlicedTube& x, base_container::const_iterator it)
            : base_container::const_iterator(it), _x(x) { }

          std::shared_ptr<const Slice<T>> operator->()
          {
            return _x(*this);
          }

          const Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          const SlicedTube& _x;
      };

      const_iterator begin() const { return { *this, _tdomain->cbegin() }; }
      const_iterator end() const   { return { *this, _tdomain->cend() }; }

      struct const_reverse_iterator : public base_container::const_reverse_iterator
      {
        public:
          
          const_reverse_iterator(const SlicedTube& x, base_container::const_reverse_iterator it)
            : base_container::const_reverse_iterator(it), _x(x) { }

          std::shared_ptr<const Slice<T>> operator->()
          {
            return _x(*this);
          }

          const Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          const SlicedTube& _x;
      };

      const_reverse_iterator rbegin() const { return { *this, _tdomain->crbegin() }; }
      const_reverse_iterator rend() const   { return { *this, _tdomain->crend() }; }

    protected:

      inline T all_reals_codomain() const
      {
        T x = first_slice()->codomain();
        x.init();
        return x;
      }

      inline T empty_codomain() const
      {
        T x = first_slice()->codomain();
        x.set_empty();
        return x;
      }
  };


  // Template deduction guide:

  template<typename T>
  SlicedTube(const std::shared_ptr<TDomain>& tdomain, const AnalyticFunction<T>& f) -> 
    SlicedTube<typename Wrapper<T>::Domain>;
  
  template<typename T>
  SlicedTube(const std::shared_ptr<TDomain>& tdomain, const SampledTraj<T>& f) -> 
    SlicedTube<typename Wrapper<T>::Domain>;
}

#include "codac2_SlicedTube_integral_impl.h"