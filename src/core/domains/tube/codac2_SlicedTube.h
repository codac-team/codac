/** 
 *  \file codac2_SlicedTube.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_SlicedTubeBase.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_Tube_operator.h"
#include "codac2_CtcDeriv.h"
#include "codac2_Scalar.h"
#include "codac2_AnalyticTraj.h"

namespace codac2
{
  /**
   * \class SlicedTube
   * \brief Tube represented over a sliced temporal domain
   *
   * A ``SlicedTube<T>`` is a tube whose temporal domain is represented by a
   * shared ``TDomain`` made of ``TSlice`` objects.
   *
   * For each temporal slice of this partition, the tube stores a ``Slice<T>``
   * describing the codomain of the tube over that temporal support.
   *
   * The codomain type ``T`` is typically ``Interval`` or ``IntervalVector``,
   * or any custom domain implemented by the user.
   *
   * \tparam T codomain type of the tube
   */
  template<typename T>
  class SlicedTube : public SlicedTubeBase
  {
    public:

      /**
       * \brief Creates a sliced tube with constant codomain over all temporal slices
       *
       * \param tdomain shared temporal domain of the tube
       * \param codomain codomain assigned to each slice
       */
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

      /**
       * \brief Creates a sliced tube by evaluating an analytic function on each temporal slice
       *
       * The function is evaluated on each temporal interval of the associated
       * ``TDomain``.
       *
       * \param tdomain shared temporal domain of the tube
       * \param f analytic function of one scalar variable
       */
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

      /**
       * \brief Creates a sliced tube by evaluating an analytic trajectory on each temporal slice
       *
       * The function is evaluated on each temporal interval of the associated
       * ``TDomain``.
       *
       * \param tdomain shared temporal domain of the tube
       * \param x analytic trajectory
       */
      template<typename V>
        requires std::is_same_v<typename ExprType<T>::Type,V>
      explicit SlicedTube(const std::shared_ptr<TDomain>& tdomain,
        const AnalyticTraj<V>& x)
        : SlicedTubeBase(tdomain)
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*this, it, x.eval((Interval)*it))
          });
      }

      /**
       * \brief Creates a sliced tube from a sampled trajectory
       *
       * \tparam V sampled value type
       * \param tdomain shared temporal domain of the tube
       * \param x sampled trajectory evaluated on each temporal slice
       */
      template<typename V>
        requires std::is_same_v<typename Wrapper<V>::Domain,T>
      explicit SlicedTube(const std::shared_ptr<TDomain>& tdomain,
        const SampledTraj<V>& x)
        : SlicedTubeBase(tdomain)
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*this, it, x((Interval)*it))
          });
      }

      /**
       * \brief Copy constructor
       *
       * The copied tube shares the same ``TDomain`` as the source tube.
       *
       * \param x tube to be copied
       */
      SlicedTube(const SlicedTube<T>& x)
        : SlicedTubeBase(x.tdomain())
      {
        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          it->_slices.insert({
            this,
            std::make_shared<Slice<T>>(*x.slice(it), *this)
          });
      }

      /**
       * \brief Assignment operator
       *
       * Both tubes are expected to share the same temporal domain object.
       *
       * \param x source tube
       * \return reference to this tube
       */
      inline SlicedTube& operator=(const SlicedTube& x)
      {
        assert_release(_tdomain == x._tdomain);

        for(auto it = _tdomain->begin(); it != _tdomain->end(); ++it)
          slice(it)->set(x.slice(it)->codomain(), false);

        return *this;
      }
      
      /**
       * \brief Returns the codomain dimension
       *
       * \return dimension of the codomain type ``T``
       */
      inline Index size() const
      {
        return first_slice()->size();
      }

      /**
       * \brief Returns the matrix shape of this tube codomain
       *
       * For scalar codomains, the returned shape is ``{1,1}``.
       *
       * \return pair ``(rows, cols)``
       */
      virtual std::pair<Index,Index> shape() const
      {
        if constexpr(std::is_same_v<typename ExprType<T>::Type,ScalarType>)
          return {1,1};
        else
          return {first_slice()->codomain().rows(),first_slice()->codomain().cols()};
      }

      /**
       * \brief Returns the volume of this tube
       *
       * Only non-gate slices contribute to the volume.
       *
       * \return sum of the volumes of all non-gate slices
       */
      inline double volume() const
      {
        double volume = 0.;
        for(const auto& s : *this)
          if(!s.is_gate())
            volume += s.volume();
        return volume;
      }

      /**
       * \brief Returns the first slice of this tube
       *
       * \return shared pointer to the first slice
       */
      inline std::shared_ptr<Slice<T>> first_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).first_slice());
      }

      /**
       * \brief Returns the first slice of this tube
       *
       * \return shared pointer to the first slice
       */
      inline std::shared_ptr<const Slice<T>> first_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SlicedTubeBase::first_slice());
      }

      /**
       * \brief Returns the last slice of this tube
       *
       * \return shared pointer to the last slice
       */
      inline std::shared_ptr<Slice<T>> last_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).last_slice());
      }

      /**
       * \brief Returns the last slice of this tube
       *
       * \return shared pointer to the last slice
       */
      inline std::shared_ptr<const Slice<T>> last_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SlicedTubeBase::last_slice());
      }
      
      /**
       * \brief Returns the slice attached to a temporal iterator
       *
       * \param it iterator to a ``TSlice`` of the shared ``TDomain``
       * \return shared pointer to the corresponding slice
       */
      inline std::shared_ptr<Slice<T>> slice(const std::list<TSlice>::iterator& it)
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).slice(it));
      }
      
      /**
       * \brief Returns the slice attached to a temporal iterator
       *
       * \param it constant iterator to a ``TSlice`` of the shared ``TDomain``
       * \return shared pointer to the corresponding slice
       */
      inline std::shared_ptr<const Slice<T>> slice(const std::list<TSlice>::const_iterator& it) const
      {
        return std::static_pointer_cast<const Slice<T>>(
          it->slices().at(this));
      }
      
      /**
       * \brief Returns the slice attached to a reverse temporal iterator
       *
       * \param it reverse iterator to a ``TSlice`` of the shared ``TDomain``
       * \return shared pointer to the corresponding slice
       */
      inline std::shared_ptr<Slice<T>> slice(const std::list<TSlice>::reverse_iterator& it)
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const SlicedTube&>(*this).slice(it));
      }
      
      /**
       * \brief Returns the slice attached to a reverse temporal iterator
       *
       * \param it constant reverse iterator to a ``TSlice`` of the shared ``TDomain``
       * \return shared pointer to the corresponding slice
       */
      inline std::shared_ptr<const Slice<T>> slice(const std::list<TSlice>::const_reverse_iterator& it) const
      {
        return std::static_pointer_cast<const Slice<T>>(
          it->slices().at(this));
      }
      
      /**
       * \brief Returns the slice attached to a temporal slice pointer
       *
       * This overload is mainly intended for language bindings.
       *
       * \param ptr pointer to a temporal slice of the shared ``TDomain``
       * \return shared pointer to the corresponding slice
       */
      inline std::shared_ptr<Slice<T>> slice(std::shared_ptr<TSlice> ptr)
      {
        // Used in Python binding
        auto it = std::find_if(_tdomain->begin(), _tdomain->end(),
                    [&](TSlice& t){ return &t == ptr.get(); });
        assert(it != _tdomain->end());
        return slice(it);
      }

      /**
       * \brief Tests whether this tube is empty
       *
       * A fast evaluation is done by considering gates first,
       * then envelopes, which allows to quickly identify an empty set
       *
       * \return ``true`` if at least one slice is empty, ``false`` otherwise
       */
      inline bool is_empty() const
      {
        for(const auto& s : *this)
          if(s.is_gate() && s.is_empty())
            return true;
        for(const auto& s : *this)
          if(!s.is_gate() && s.is_empty())
            return true;
        return false;
      }

      /**
       * \brief Tests whether this tube is unbounded
       *
       * \return ``true`` if at least one slice is unbounded, ``false`` otherwise
       */
      inline bool is_unbounded() const
      {
        for(const auto& s : *this)
          if(s.is_unbounded())
            return true;
        return false;
      }

      /**
       * \brief Returns the global codomain of this tube
       *
       * The returned codomain is the union of the codomains of all slices.
       *
       * \return global codomain enclosure
       */
      inline T codomain() const
      {
        T x = first_slice()->codomain();
        for(const auto& s : *this)
          x |= s.codomain();
        return x;
      }
      
      template<typename Func>
      inline T eval_common(const Interval& t, const Func& apply_eval) const
      {
        if(!tdomain()->t0_tf().is_superset(t))
          return all_reals_value();

        auto it = _tdomain->tslice(t.lb());
        assert(it != _tdomain->end());
        T codomain = apply_eval(it, t & *it);

        while(it != std::next(_tdomain->tslice(t.ub())))
        {
          if(it->lb() == t.ub()) break;
          codomain |= apply_eval(it, t & *it);
          it++;
        }

        return codomain;
      }

      /**
       * \brief Evaluates this tube over a temporal interval
       *
       * \param t temporal interval
       * \return enclosure of \f$[x]([t])\f$
       */
      T operator()(const Interval& t) const
      {
        return eval_common(t,
          [this](auto it, const Interval& t_) {
            return this->slice(it)->operator()(t_);
          });
      }

      /**
       * \brief Returns the optimal evaluation over a temporal interval using a derivative tube
       *
       * \param t temporal interval
       * \param v derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return enclosure of \f$[x]([t])\f$
       */
      T operator()(const Interval& t, const SlicedTube<T>& v) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        return eval_common(t,
          [this,&v](auto it, const Interval& t_) {
            return this->slice(it)->operator()(t_, *v.slice(it));

          });
      }

      /**
       * \brief Returns enclosed lower and upper bounds over a temporal interval
       *
       * \param t temporal interval
       * \return pair made of enclosed lower and upper bounds
       */
      std::pair<T,T> enclosed_bounds(const Interval& t) const
      {
        auto x = this->empty_value();
        auto bounds = std::make_pair(x,x);

        if(t.lb() < _tdomain->t0_tf().lb() || t.ub() > _tdomain->t0_tf().ub())
        {
          x.init(Interval(-oo,0));
          bounds.first |= x;
          x.init(Interval(0,oo));
          bounds.second |= x;
        }

        Interval t_inter = t & _tdomain->t0_tf();
        auto si = slice(_tdomain->tslice(t_inter.lb()));

        while(si && si->t0_tf().lb() <= t_inter.ub())
        {
          auto slice_bounds = si->enclosed_bounds(t_inter & si->t0_tf());
          bounds.first |= slice_bounds.first;
          bounds.second |= slice_bounds.second;
          si = si->next_slice();
        }

        return bounds;
      }

      /**
       * \brief Sets all codomains of this tube to the same value
       *
       * \param codomain new codomain
       */
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

      /**
       * \brief Sets the codomain at one temporal instant
       *
       * The temporal domain may be refined so that ``t`` becomes an explicit gate.
       *
       * \param codomain new codomain
       * \param t temporal instant
       */
      inline void set(const T& codomain, double t)
      {
        assert_release(codomain.size() == this->size());
        slice(_tdomain->sample(t,true))->set(codomain);
      }

      /**
       * \brief Sets the codomain over a temporal interval
       *
       * The temporal domain may be refined so that the bounds of ``t`` become
       * explicit gates.
       *
       * \param codomain new codomain
       * \param t temporal interval
       */
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
          slice(it_lb)->set(codomain);
        } while(it_lb != it_ub && (++it_lb) != _tdomain->end());
      }

      /**
       * \brief Sets the codomain of the \f$i\f$-th stored slice
       *
       * \param codomain new codomain
       * \param i slice index
       */
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

      /**
       * \brief Inflates this tube by a constant radius
       *
       * \tparam V radius type
       * \param rad inflation radius
       * \return reference to this tube
       */
      template<typename V>
        requires (std::is_same_v<typename Wrapper<V>::Domain,T> || std::is_same_v<V,double>)
      const SlicedTube<T>& inflate(const V& rad)
      {
        // todo: faster implementation with iterators
        for(auto& s : *this)
          if(!s.is_gate())
            s.set(T(s.codomain()).inflate(rad), false);

        for(auto& s : *this)
          if(s.is_gate())
            s.set(T(s.codomain()).inflate(rad), false);

        return *this;
      }

      /**
       * \brief Inflates this tube by a time-varying sampled radius
       *
       * \tparam V sampled radius type
       * \param rad sampled inflation radius
       * \return reference to this tube
       */
      template<typename V>
        requires (std::is_same_v<typename Wrapper<V>::Domain,T> || std::is_same_v<V,double>)
      const SlicedTube<T>& inflate(const SampledTraj<V>& rad)
      {
        // todo: faster implementation with iterators
        assert_release(tdomain()->t0_tf().is_subset(rad.tdomain()));

        for(auto& s : *this)
          if(!s.is_gate())
            s.set(T(s.codomain()).inflate(rad(s.t0_tf()).ub()), false);

        for(auto& s : *this)
          if(s.is_gate())
            s.set(T(s.codomain()).inflate(rad(s.t0_tf()).ub()), false);

        return *this;
      }

      /**
       * \brief Extracts one scalar component of this tube
       *
       * \param i component index
       * \return scalar sliced tube corresponding to the \f$i\f$-th component
       */
      SlicedTube<Interval> operator[](Index i) const
      {
        assert_release(i >= 0 && i < size());
        SlicedTube<Interval> xi(tdomain(), Interval());
        for(auto it = tdomain()->begin() ; it != tdomain()->end() ; it++)
          xi.slice(it)->codomain() = slice(it)->codomain()[i];
        return xi;
      }

      /**
       * \brief Extracts a subvector of this tube
       *
       * \param i first component index
       * \param j last component index
       * \return sliced tube associated with the subvector \f$[i,j]\f$
       */
      SlicedTube<IntervalVector> subvector(Index i, Index j) const
      {
        assert_release(i >= 0 && i <= j && j < size());
        SlicedTube<IntervalVector> xij(tdomain(), IntervalVector(j-i+1));
        for(auto it = tdomain()->begin() ; it != tdomain()->end() ; it++)
          xij.slice(it)->codomain() = slice(it)->codomain().subvector(i,j);
        return xij;
      }

      /**
       * \brief Compares two sliced tubes
       *
       * Two sliced tubes are equal if they share the same temporal partition and
       * if all corresponding codomains are equal.
       *
       * \param x tube to compare with
       * \return ``true`` if both tubes are equal, ``false`` otherwise
       */
      inline bool operator==(const SlicedTube& x) const
      {
        if(!TDomain::are_same(tdomain(), x.tdomain()))
          return false;

        auto it_this = _tdomain->begin();
        auto it_x = x.tdomain()->cbegin();

        while(it_this != _tdomain->end())
          if(*slice(it_this++) != *x.slice(it_x++))
            return false;

        return true;
      }

      /**
       * \brief Intersects this tube with another one
       *
       * Both tubes are expected to be defined over the same temporal partition.
       *
       * \param x tube to intersect with
       * \return updated tube
       */
      inline SlicedTube& operator&=(const SlicedTube& x)
      {
        assert(TDomain::are_same(tdomain(), x.tdomain()));
        auto it_this = _tdomain->begin();
        auto it_x = x.tdomain()->cbegin();

        while(it_this != _tdomain->end())
        {
          auto s = slice(it_this);
          s->set(s->codomain() & x.slice(it_x)->codomain());
          it_this++; it_x++;
        }

        assert(it_x == x.tdomain()->cend());
        return *this;
      }

      /**
       * \brief Stream output for a sliced tube
       *
       * \param os output stream
       * \param x sliced tube to display
       * \return reference to the output stream
       */
      friend inline std::ostream& operator<<(std::ostream& os, const SlicedTube<T>& x)
      {
        os << x.t0_tf()
           << "->" << (x.is_empty() ? x.empty_value() : x.codomain())
           << ", " << x.nb_slices()
           << " slice" << (x.nb_slices() > 1 ? "s" : "")
           << std::flush;
        return os;
      }

      /**
       * \brief Returns an analytic wrapper of this tube, allowing to
       * evaluate this tube as an operator in an analytic expression.
       *
       * \return analytic function associated with this tube
       */
      AnalyticFunction<typename ExprType<T>::Type> as_function() const
      {
        ScalarVar t;
        return {{t},
          AnalyticExprWrapper<typename ExprType<T>::Type>(
            std::make_shared<AnalyticOperationExpr<
              TubeOp<SlicedTube<T>>,typename ExprType<T>::Type,ScalarType>>(*this,t))
        };
      }

      // Inversion related methods

      /**
       * \defgroup codac2_slicedtube_inversion Inversion of sliced tubes
       */

      template<typename Func>
      Interval invert_common(const T& y, const Interval& t, const Func& apply_invert) const
      {
        assert_release(y.size() == this->size());

        if(t.is_empty() || y.is_empty())
          return Interval::empty();

        else if(!t.is_subset(_tdomain->t0_tf()))
          return Interval();

        Interval invert = Interval::empty();
        Interval t_ = t & _tdomain->t0_tf();

        assert(!t_.is_empty() && !t_.is_unbounded());
        for(auto it = _tdomain->tslice(t_.lb()) ;
          it != _tdomain->end() && it->lb() <= t_.ub() ; it++)
        {
          if(!t_.is_degenerated() && it->is_gate())
            continue;
          
          assert(it != _tdomain->end());
          invert |= apply_invert(it, t & *it);
        }

        return invert;
      }

      template<typename Func>
      void invert_common_subsets(const T& y, std::vector<Interval> &v_t,
        const Interval& t, const Func& apply_invert) const
      {
        assert_release(y.size() == this->size());
        v_t.clear();

        if(t.is_empty() || y.is_empty())
          return;

        else if(!t.is_subset(_tdomain->t0_tf()))
        {
          v_t.push_back(Interval());
          return;
        }

        Interval invert = Interval::empty();
        Interval t_ = t & _tdomain->t0_tf();

        assert(!t_.is_empty() && !t_.is_unbounded());
        for(auto it = _tdomain->tslice(t_.lb()) ;
          it != _tdomain->end() && it->lb() < t_.ub() ; it++)
        {
          if(it->is_gate())
            continue;

          assert(it != _tdomain->end());
          Interval local_invert = apply_invert(it, t & *it);

          if(local_invert.is_empty() && !invert.is_empty())
          {
            v_t.push_back(invert);
            invert.set_empty();
          }

          else
            invert |= local_invert;
        }

        if(!invert.is_empty())
          v_t.push_back(invert);
      }

      /**
       * \brief Returns the interval inversion \f$\left[[x]^{-1}([y])\right]\f$.
       *
       * If the inversion results in several pre-images, their union is returned.
       *
       * \param y interval codomain
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      Interval invert(const T& y) const
      {
        return invert(y,t0_tf());
      }

      /**
       * \ingroup codac2_slicedtube_inversion
       * \brief Returns the interval inversion \f$\left[[x]^{-1}([y])\right]\f$.
       *
       * If the inversion results in several pre-images, their union is returned.
       *
       * \param y interval codomain
       * \param t (optional) temporal domain on which the inversion will be performed
       * \return the hull of \f$[x]^{-1}([y])\f$
       */
      Interval invert(const T& y, const Interval& t) const
      {
        return invert_common(y, t,
          [this,&y](auto it, const Interval& t_) {
            return this->slice(it)->invert(y, t_);
          });
      }

      /**
       * \brief Computes the set of continuous values of the inversion \f$[x]^{-1}([y])\f$.
       *
       * \param y interval codomain
       * \param v_t vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot)\f$
       */
      void invert(const T& y, std::vector<Interval> &v_t) const
      {
        invert(y,v_t,t0_tf());
      }

      /**
       * \ingroup codac2_slicedtube_inversion
       * \brief Computes the set of continuous values of the inversion \f$[x]^{-1}([y])\f$.
       *
       * \param y interval codomain
       * \param v_t vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot)\f$
       * \param t (optional) temporal domain on which the inversion will be performed
       */
      void invert(const T& y, std::vector<Interval> &v_t, const Interval& t) const
      {
        return invert_common_subsets(y, v_t, t,
          [this,&y](auto it, const Interval& t_) {
            return this->slice(it)->invert(y, t_);
          });
      }

      /**
       * \brief Returns the optimal interval inversion \f$\left[[x]^{-1}([y])\right]\f$.
       *
       * The knowledge of the derivative tube \f$[v](\cdot)\f$ allows a finer inversion.
       * If the inversion results in several pre-images, their union is returned.
       *
       * \param y interval codomain
       * \param v derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \return hull of \f$[x]^{-1}([y])\f$
       */
      Interval invert(const T& y, const SlicedTube<T>& v) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        return invert(y,v,t0_tf());
      }

      /**
       * \ingroup codac2_slicedtube_inversion
       * \brief Returns the optimal interval inversion \f$\left[[x]^{-1}([y])\right]\f$.
       *
       * The knowledge of the derivative tube \f$[v](\cdot)\f$ allows a finer inversion.
       * If the inversion results in several pre-images, their union is returned.
       *
       * \param y interval codomain
       * \param v derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param t (optional) temporal domain on which the inversion will be performed
       * \return hull of \f$[x]^{-1}([y])\f$
       */
      Interval invert(const T& y, const SlicedTube<T>& v, const Interval& t) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        return invert_common(y, t,
          [this,&v,&y](auto it, const Interval& t_) {
            return this->slice(it)->invert(y, *v.slice(it), t_);
          });
      }

      /**
       * \brief Computes the set of continuous values of the optimal inversion \f$[x]^{-1}([y])\f$.
       *
       * The knowledge of the derivative tube \f$[v](\cdot)\f$ allows finer inversions.
       *
       * \param y interval codomain
       * \param v_t vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot), \dot{x}(\cdot)\in[v](\cdot)\f$
       * \param v derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       */
      void invert(const T& y, std::vector<Interval> &v_t, const SlicedTube<T>& v) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        invert(y,v_t,v,t0_tf());
      }

      /**
       * \ingroup codac2_slicedtube_inversion
       * \brief Computes the set of continuous values of the optimal inversion \f$[x]^{-1}([y])\f$.
       *
       * The knowledge of the derivative tube \f$[v](\cdot)\f$ allows finer inversions.
       *
       * \param y interval codomain
       * \param v_t vector of the sub-tdomains \f$[t_k]\f$ for which
       *            \f$\forall t\in[t_k] \mid x(t)\in[y], x(\cdot)\in[x](\cdot), \dot{x}(\cdot)\in[v](\cdot)\f$
       * \param v derivative tube such that \f$\dot{x}(\cdot)\in[v](\cdot)\f$
       * \param t (optional) temporal domain on which the inversion will be performed
       */
      void invert(const T& y, std::vector<Interval> &v_t, const SlicedTube<T>& v, const Interval& t) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        return invert_common_subsets(y, v_t, t,
          [this,&v,&y](auto it, const Interval& t_) {
            return this->slice(it)->invert(y, *v.slice(it), t_);
          });
      }

      // Integral related methods

      /**
       * \defgroup codac2_slicedtube_integrals Integration and primitive operations on sliced tubes
       * \brief The following methods are valid for tubes defined for ``Interval`` or
       * ``IntervalVector`` codomains. The returned values are integral enclosures of
       * same type (respectively, ``Interval`` or ``IntervalVector``).
       */

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns an enclosure of the integrals of this tube from \f$t_0\f$ to \f$[t]\f$.
       *
       * This method computes an enclosure of
       * \f[
       *   \left\{ \int_{t_0}^{\tau} [x](s)\,ds \;\middle|\; \tau\in[t] \right\}.
       * \f]
       *
       * It is obtained from ``partial_integral(t)`` by taking the hull between
       * the lower bound of the lower enclosure and the upper bound of the upper
       * enclosure.
       *
       * \param t temporal interval \f$[t]\f$
       * \return enclosure of the integrals of this tube over ``t``
       */
      T integral(const Interval& t) const;

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns an enclosure of the integrals of this tube between the
       * time intervals\f$[t_1]\f$ and \f$[t_2]\f$.
       *
       * This method computes an enclosure of
       * \f[
       *   \left\{ \int_{\tau_1}^{\tau_2} [x](s)\,ds
       *   \;\middle|\; \tau_1\in[t_1],\ \tau_2\in[t_2] \right\}.
       * \f]
       *
       * The result is obtained by subtracting the partial integral enclosures at
       * ``t1`` and ``t2``.
       *
       * \param t1 first temporal interval \f$[t_1]\f$
       * \param t2 second temporal interval \f$[t_2]\f$
       * \return enclosure of the integrals of this tube between ``t1`` and ``t2``
       */
      T integral(const Interval& t1, const Interval& t2) const;

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns lower and upper enclosures of the integrals of this tube
       * \f$[x](\cdot)=[x^-(\cdot),x^+(\cdot)]\f$ from \f$t_0\f$ to \f$[t]\f$.
       *
       * This method returns a pair \f$([p^-],[p^+])\f$ such that:
       * \f[
       *   [p^-]\supset
       *        \left\{ \int_{t_0}^{\tau} x^-(s)\,ds
       *        \;\middle|\; \tau\in[t]
       *        \right\}
       *   \mathrm{~~and~~}
       *   [p^+]\supset
       *        \left\{\int_{t_0}^{\tau} x^+(s)\,ds
       *        \;\middle|\; \tau\in[t]
       *        \right\}.
       * \f]
       *
       * This representation preserves more information than ``integral(t)``,
       * which only returns the hull of these partial integral bounds.
       *
       * \param t temporal interval \f$[t]\f$
       * \return pair of lower and upper partial integral enclosures over ``t``
       */
      std::pair<T,T> partial_integral(const Interval& t) const;

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns lower and upper enclosures of the integrals of this tube
       * \f$[x](\cdot)=[x^-(\cdot),x^+(\cdot)]\f$ between \f$[t_1]\f$ and \f$[t_2]\f$.
       *
       * This method returns a pair obtained by subtracting the partial integral
       * enclosures at ``t1`` from those at ``t2``. The returned pair \f$([p^-],[p^+])\f$
       * is such that:
       * \f[
       *   [p^-]\supset
       *        \left\{ \int_{\tau_1}^{\tau_2} x^-(s)\,ds
       *        \;\middle|\; \tau_1\in[t_1],\ \tau_2\in[t_2]
       *        \right\},
       * \f]
       * and
       * \f[
       *   [p^+]\supset
       *        \left\{\int_{\tau_1}^{\tau_2} x^+(s)\,ds
       *        \;\middle|\; \tau_1\in[t_1],\ \tau_2\in[t_2]
       *        \right\}.
       * \f]
       *
       * \param t1 first temporal interval \f$[t_1]\f$
       * \param t2 second temporal interval \f$[t_2]\f$
       * \return pair of lower and upper enclosures of the integrals
       */
      std::pair<T,T> partial_integral(const Interval& t1, const Interval& t2) const;

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns a primitive of this tube with zero initial condition.
       *
       * This is a shorthand for ``primitive(x0)`` with \f$x_0 = 0\f$.
       *
       * In other words, the returned tube encloses solutions of
       * \f[
       *   \dot{p}(\cdot) \in [x](\cdot),
       *   \qquad p(t_0)=0.
       * \f]
       *
       * \return primitive tube with zero initial condition
       */
      inline SlicedTube<T> primitive() const
      {
        auto x0 = all_reals_value();
        x0.init(0.);
        return primitive(x0);
      }

      /**
       * \ingroup codac2_slicedtube_integrals
       * \brief Returns a primitive of this tube with prescribed initial condition
       *
       * This method constructs a tube ``p`` on the same temporal domain as this
       * tube, imposes the initial condition \f$p(t_0)=x_0\f$, and contracts
       * ``p`` with this tube through the derivative relation using ``CtcDeriv``.
       *
       * In other words, the returned tube encloses solutions of
       * \f[
       *   \dot{p}(\cdot) \in [x](\cdot),
       *   \qquad p(t_0)=x_0.
       * \f]
       *
       * \note The current implementation may create an explicit gate at the
       *       initial time \f$t_0\f$ if it is not already present in the
       *       temporal partition.
       *
       * \param x0 initial condition at \f$t_0\f$
       * \return primitive tube satisfying the prescribed initial condition
       */
      inline SlicedTube<T> primitive(const T& x0) const
      {
        auto x = all_reals_value();
        auto p = SlicedTube<T>(this->tdomain(), x);
        p.set(x0, this->tdomain()->t0_tf().lb()); // may create an unwanted gate
        CtcDeriv c;
        c.contract(p,*this);
        return p;
      }

      /**
       * \brief Returns the unbounded codomain value associated with ``T``
       *
       * \return unbounded value of type ``T``
       */
      inline T all_reals_value() const
      {
        T x = first_slice()->codomain();
        x.init();
        return x;
      }

      /**
       * \brief Returns the empty codomain value associated with ``T``
       *
       * \return empty value of type ``T``
       */
      inline T empty_value() const
      {
        T x = first_slice()->codomain();
        x.set_empty();
        return x;
      }

      /**
       * \brief Returns the approximated midpoint trajectory of this tube
       *
       * \tparam V sampled value type
       * \return midpoint sampled trajectory
       */
      inline SampledTraj<typename Scalar<T>::Type> mid() const
      {
        SampledTraj<typename Scalar<T>::Type> m;
        double t0 = _tdomain->t0_tf().lb();
        m.set((*this)(t0).mid(), t0);
        for(const auto& s : *this)
          if(!s.is_gate())
            m.set(s.output_gate().mid(),s.t0_tf().ub());
        return m;
      }


    public:

      using base_container = std::list<TSlice>;

      /**
       * \brief Mutable iterator over the slices of this tube
       */
      struct iterator : public base_container::iterator
      {
        public:
          
          iterator(SlicedTube& x, base_container::iterator it)
            : base_container::iterator(it), _x(x) { }

          std::shared_ptr<Slice<T>> operator->()
          {
            return _x.slice(*this);
          }

          Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          SlicedTube& _x;
      };

      /**
       * \brief Returns an iterator to the first temporal slice
       */
      iterator begin() { return { *this, _tdomain->begin() }; }

      /**
       * \brief Returns an iterator past the last temporal slice
       */
      iterator end()   { return { *this, _tdomain->end() }; }

      /**
       * \brief Mutable reverse iterator over the slices of this tube
       */
      struct reverse_iterator : public base_container::reverse_iterator
      {
        public:
          
          reverse_iterator(SlicedTube& x, base_container::reverse_iterator it)
            : base_container::reverse_iterator(it), _x(x) { }

          std::shared_ptr<Slice<T>> operator->()
          {
            return _x.slice(*this);
          }

          Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          SlicedTube& _x;
      };

      /**
       * \brief Returns a reverse iterator to the last temporal slice
       */
      reverse_iterator rbegin() { return { *this, _tdomain->rbegin() }; }

      /**
       * \brief Returns a reverse iterator past the first temporal slice
       */
      reverse_iterator rend()   { return { *this, _tdomain->rend() }; }

      /**
       * \brief Constant iterator over the slices of this tube
       */
      struct const_iterator : public base_container::const_iterator
      {
        public:
          
          const_iterator(const SlicedTube& x, base_container::const_iterator it)
            : base_container::const_iterator(it), _x(x) { }

          std::shared_ptr<const Slice<T>> operator->()
          {
            return _x.slice(*this);
          }

          const Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          const SlicedTube& _x;
      };

      /**
       * \brief Returns a constant iterator to the first temporal slice
       */
      const_iterator begin() const { return { *this, _tdomain->cbegin() }; }

      /**
       * \brief Returns a constant iterator past the last temporal slice
       */
      const_iterator end() const   { return { *this, _tdomain->cend() }; }

      /**
       * \brief Constant reverse iterator over the slices of this tube
       */
      struct const_reverse_iterator : public base_container::const_reverse_iterator
      {
        public:
          
          const_reverse_iterator(const SlicedTube& x, base_container::const_reverse_iterator it)
            : base_container::const_reverse_iterator(it), _x(x) { }

          std::shared_ptr<const Slice<T>> operator->()
          {
            return _x.slice(*this);
          }

          const Slice<T>& operator*()
          {
            return *operator->();
          }

        protected:

          const SlicedTube& _x;
      };

      /**
       * \brief Returns a constant reverse iterator to the last temporal slice
       */
      const_reverse_iterator rbegin() const { return { *this, _tdomain->crbegin() }; }

      /**
       * \brief Returns a constant reverse iterator past the first temporal slice
       */
      const_reverse_iterator rend() const   { return { *this, _tdomain->crend() }; }
  };


  // Template deduction guide:

  template<typename T>
  SlicedTube(const std::shared_ptr<TDomain>& tdomain, const AnalyticFunction<T>& f) -> 
    SlicedTube<typename Wrapper<T>::Domain>;
  
  template<typename T>
  SlicedTube(const std::shared_ptr<TDomain>& tdomain, const SampledTraj<T>& f) -> 
    SlicedTube<typename Wrapper<T>::Domain>;
  
  template<typename T>
  SlicedTube(const std::shared_ptr<TDomain>& tdomain, const AnalyticTraj<T>& f) -> 
    SlicedTube<typename Wrapper<T>::Domain>;


  // Ctc

  template<typename... X>
  inline void CtcBase<X...>::contract(SlicedTube<X>&... x) const
  {
    auto&& x0 = std::get<0>(std::forward_as_tuple(x...));
    for(auto it = x0.begin(); it != x0.end(); ++it)
      contract((x.slice(it)->codomain())...);
  }

  template<typename... X>
  inline void CtcBase<X...>::contract_tube(SlicedTube<X>&... x) const
  {
    std::cout << "CtcInverse::contract_tube(...) is deprecated; \
      use CtcInverse::contract(...) with SlicedTube arguments instead" << std::endl;
    return contract(x...);
  }
}

#include "codac2_SlicedTube_integral_impl.h"
