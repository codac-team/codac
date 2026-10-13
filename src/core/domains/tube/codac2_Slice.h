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
#include "codac2_ConvexPolygon.h"
#include "codac2_CtcDeriv.h"
#include "codac2_cart_prod.h"
#include "codac2_trunc.h"

namespace codac2
{
  /**
   * \class Slice
   * \brief Codomain of a sliced tube over one temporal slice
   *
   * A ``Slice<T>`` represents the codomain of a ``SlicedTube<T>`` over one
   * temporal interval of a ``TDomain``.
   *
   * This object is attached to one temporal slice ``TSlice`` and stores a
   * codomain of type ``T``. The type ``T`` is typically ``Interval`` or
   * ``IntervalVector``, or any custom domain implemented by the user.
   *
   * The inheritance from ``T`` is protected so that modifications of the
   * codomain remain controlled by the ``Slice`` interface. Indeed, changing the
   * codomain of a slice may require propagating contractions to adjacent gates.
   *
   * \tparam T codomain type
   */
  template<class T>
  class Slice : public SliceBase,
    protected T
    // T class inheritance is protected, because modifying a
    // Slice's codomain can affect adjacent gates if they exist
  {
    public:

      /**
       * \brief Creates a slice attached to a tube over a temporal slice
       *
       * \param tube sliced tube owning this slice
       * \param it_tslice iterator to the associated temporal slice
       * \param codomain codomain associated with this slice
       */
      explicit Slice(const SlicedTubeBase& tube, const std::list<TSlice>::iterator& it_tslice, const T& codomain)
        : SliceBase(tube, it_tslice), T(codomain)
      { }

      /**
       * \brief Creates a slice from another one for a given tube
       *
       * This constructor duplicates the codomain and keeps the same temporal
       * support.
       *
       * \param s source slice
       * \param tube sliced tube owning the copied slice
       */
      Slice(const Slice& s, const SlicedTubeBase& tube)
        : SliceBase(tube, s._it_tslice), T(s.codomain())
      { }

      // Slice objects cannot be copyable or movable,
      // as they are supposed to be connected to other Slice objects
      Slice(const Slice& s) = delete;
      Slice& operator=(const Slice&) = delete;
      Slice(Slice&&) = delete;
      Slice& operator=(Slice&&) = delete;

      /**
       * \brief Returns the sliced tube owning this slice
       *
       * \return reference to the parent sliced tube
       */
      inline const SlicedTube<T>& tube() const
      {
        return static_cast<const SlicedTube<T>&>(_tube);
      }

      /**
       * \brief Duplicates this slice
       *
       * \return shared pointer to a copy of this slice
       */
      inline std::shared_ptr<SliceBase> copy() const override
      {
        return std::make_shared<Slice>(*this, this->_tube);
      }

      /**
       * \brief Returns the codomain dimension
       *
       * \return size of the codomain
       */
      inline Index size() const
      {
        return this->T::size();
      }

      /**
       * \brief Returns a mutable reference to the codomain
       *
       * \note Prefer ``set()`` when adjacency propagation is required.
       *
       * \return mutable reference to the codomain
       */
      inline T& codomain()
      {
        return static_cast<T&>(*this);
      }

      /**
       * \brief Returns a constant reference to the codomain
       *
       * \return constant reference to the codomain
       */
      inline const T& codomain() const
      {
        return static_cast<const T&>(*this);
      }

      /**
       * \brief Tests whether this slice is a gate
       *
       * A slice is considered a gate when its temporal support is degenerate.
       *
       * \return ``true`` if this slice is a gate, ``false`` otherwise
       */
      inline bool is_gate() const
      {
        return t0_tf().is_degenerated();
      }

      /**
       * \brief Returns the previous slice
       *
       * \return shared pointer to the previous slice, or ``nullptr`` if none exists
       */
      inline std::shared_ptr<const Slice<T>> prev_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SliceBase::prev_slice());
      }

      /**
       * \brief Returns the previous slice
       *
       * \return shared pointer to the previous slice, or ``nullptr`` if none exists
       */
      inline std::shared_ptr<Slice<T>> prev_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice<T>&>(*this).prev_slice());
      }

      /**
       * \brief Returns the next slice
       *
       * \return shared pointer to the next slice, or ``nullptr`` if none exists
       */
      inline std::shared_ptr<const Slice<T>> next_slice() const
      {
        return std::static_pointer_cast<const Slice<T>>(
          this->SliceBase::next_slice());
      }

      /**
       * \brief Returns the next slice
       *
       * \return shared pointer to the next slice, or ``nullptr`` if none exists
       */
      inline std::shared_ptr<Slice<T>> next_slice()
      {
        return std::const_pointer_cast<Slice<T>>(
          static_cast<const Slice<T>&>(*this).next_slice());
      }

      /**
       * \brief Returns the input gate of this slice
       *
       * If the previous temporal element is an explicit gate, its codomain is
       * returned. Otherwise, the returned value is the intersection between this
       * codomain and the codomain of the previous slice. If no previous slice
       * exists, the codomain of this slice is returned.
       *
       * \return enclosure of the input gate
       */
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

      /**
       * \brief Returns the output gate of this slice
       *
       * If the next temporal element is an explicit gate, its codomain is
       * returned. Otherwise, the returned value is the intersection between this
       * codomain and the codomain of the next slice. If no next slice exists,
       * the codomain of this slice is returned.
       *
       * \return enclosure of the output gate
       */
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

      /**
       * \brief Returns enclosed lower and upper bounds over a temporal interval
       *
       * This method returns a pair whose first component encloses the reachable
       * lower bounds and whose second component encloses the reachable upper
       * bounds over ``t``.
       *
       * \param t temporal interval
       * \return pair made of enclosed lower and upper bounds
       */
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

      /**
       * \brief Sets the codomain of this slice
       *
       * If ``propagate`` is set to ``true``, adjacent gates are updated in order
       * to preserve temporal consistency.
       *
       * \param x new codomain
       * \param propagate if set to ``true``, propagates the update to adjacent gates
       */
      inline void set(const T& x, bool propagate = true)
      {
        assert_release(x.size() == this->size());
        codomain() = x;
        if(propagate)
          update_adjacent_codomains();
      }

      /**
       * \brief Initializes this codomain to its unbounded value
       *
       * No propagation is performed on adjacent slices.
       */
      inline void init() override
      {
        this->T::init();
        // Nothing to propagate to adjacent codomains
      }

      /**
       * \brief Sets this codomain to the empty set
       *
       * Adjacent gates are updated accordingly.
       */
      inline void set_empty() override
      {
        set_empty(true);
      }

      /**
       * \brief Compares two slices
       *
       * The comparison only involves the codomain of the slices.
       *
       * \param x slice to compare with
       * \return ``true`` if both codomains are equal, ``false`` otherwise
       */
      inline bool operator==(const Slice& x) const
      {
        return codomain() == x.codomain();
      }

      /**
       * \brief Returns the polygonal enclosure associated with this scalar slice
       *
       * The enclosure is built from the codomain of this slice, its input/output
       * gates and the derivative information provided by ``v``.
       *
       * \param v derivative slice
       * \return polygonal enclosure of this slice
       */
      inline ConvexPolygon polygon_slice(const Slice<T>& v) const
        requires std::is_same_v<T,Interval>
      {
        const Interval& t = this->t0_tf();
        Interval input = this->input_gate(), output = this->output_gate();

        // /!\ .diam() method is not reliable (floating point result)
        // -> We need to compute the diameter with intervals
        Interval d = Interval(t.ub())-Interval(t.lb());

        Interval proj_output =  input + d * v;
        Interval proj_input  = output - d * v;

        return CtcDeriv::polygon_slice(
          t, *this,
          input, proj_input,
          output, proj_output,
          v);
      }

      /**
       * \brief Returns the polygonal enclosure associated with one component
       *        of a vector slice
       *
       * \param v derivative slice
       * \param i component index
       * \return polygonal enclosure of the \f$i\f$-th component
       */
      inline ConvexPolygon polygon_slice_i(const Slice<T>& v, Index i) const
        requires std::is_same_v<T,IntervalVector>
      {
        const Interval& t = this->t0_tf();

        // /!\ .diam() method is not reliable (floating point result)
        // -> We need to compute the diameter with intervals
        Interval d = Interval(t.ub())-Interval(t.lb());

        Interval input = this->input_gate()[i], output = this->output_gate()[i];
        Interval proj_output =  input + d * v[i];
        Interval proj_input  = output - d * v[i];

        return CtcDeriv::polygon_slice(
          t, (*this)[i],
          input, proj_input,
          output, proj_output,
          v[i]);
      }

      /**
       * \brief Returns the evaluation of this slice at \f$t\f$
       *
       * \param t temporal input (outside ``Slice``'s tdomain, result is unbounded)
       * \return value of \f$\llbracket x\rrbracket(t)\f$
       */
      inline T operator()(double t) const
      {
        if(t == t0_tf().lb())
          return input_gate();

        else if(t == t0_tf().ub())
          return output_gate();

        else if(t0_tf().contains(t))
          return codomain();

        else
          return all_reals_value();
      }

      /**
       * \brief Returns the evaluation of this slice over \f$[t]\f$
       *
       * \param t ``Interval`` temporal input (outside ``Slice``'s tdomain, result is unbounded)
       * \return value of \f$\llbracket x\rrbracket([t])\f$
       */
      inline T operator()(const Interval& t) const
      {
        if(t.is_degenerated())
          return operator()(t.lb());

        else if(t.is_subset(t0_tf()))
          return codomain();

        else
          return all_reals_value();
      }

      /**
       * \brief Returns the optimal evaluation of this slice at \f$t\f$,
       *        based on the derivative information \f$\llbracket v\rrbracket(\cdot)\f$
       *
       * \param t temporal input (outside ``Slice``'s tdomain, result is unbounded)
       * \param v derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \return ``Interval`` value of \f$\llbracket x\rrbracket(t)\f$
       */
      inline T operator()(double t, const Slice<T>& v) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        return operator()(Interval(t),v);
      }

      /**
       * \brief Returns the optimal evaluation of this slice over \f$[t]\f$,
       *        based on the derivative information \f$\llbracket v\rrbracket(\cdot)\f$
       *
       * \param t ``Interval`` temporal input (outside ``Slice``'s tdomain, result is unbounded)
       * \param v derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \return ``Interval`` value of \f$\llbracket x\rrbracket([t])\f$
       */
      inline T operator()(const Interval& t, const Slice<T>& v) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        if constexpr(std::is_same_v<T,Interval>)
          return untrunc((polygon_slice(v) & ConvexPolygon(cart_prod(t,trunc(codomain())))).box()[1]);

        else if constexpr(std::is_same_v<T,IntervalVector>)
        {
          T y = all_reals_value();
          IntervalVector codom = codomain();
          for(Index i = 0 ; i < size() ; i++)
            y[i] &= untrunc((polygon_slice_i(v,i) & ConvexPolygon(cart_prod(t,trunc(codom[i])))).box()[1]);
          return y;
        }
      }

      /**
       * \brief Returns the interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
       *
       * \param y ``Interval`` codomain
       * \param t optional interval tdomain on which the inversion will be performed
       * \return hull of \f$\llbracket x\rrbracket^{-1}([y])\cap[t]\f$
       */
      inline Interval invert(const T& y, const Interval& t = Interval()) const
      {
        if(t.is_empty())
          return Interval::empty();

        else if(t.is_strict_superset(t0_tf()))
          return Interval();

        else if((t0_tf() & t) == t0_tf() && codomain().is_subset(y))
          return t0_tf();

        else if(t == t0_tf().lb())
        {
          if(y.intersects(input_gate()))
            return t0_tf().lb();
          else
            return Interval::empty();
        }

        else if(t == t0_tf().ub())
        {
          if(y.intersects(output_gate()))
            return t0_tf().ub();
          else
            return Interval::empty();
        }

        else
        {
          if(y.intersects(codomain()))
            return t & t0_tf();
          else
            return Interval::empty();
        }
      }

      /**
       * \brief Returns the optimal interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
       *
       * \note The knowledge of the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$ allows a finer inversion.
       *
       * \param y ``Interval`` codomain
       * \param v derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \param t the optional interval tdomain on which the inversion will be performed
       * \return hull of \f$\llbracket x\rrbracket^{-1}([y])\cap[t]\f$
       */
      inline Interval invert(const T& y, const Slice<T>& v, const Interval& t = Interval()) const
        requires (std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>)
      {
        if(t.is_empty() || y.is_empty())
          return Interval::empty();

        else if(!t.is_subset(t0_tf()))
          return Interval();

        else
        {
          if constexpr(std::is_same_v<T,Interval>)
            return untrunc((polygon_slice(v) & ConvexPolygon(cart_prod(t,trunc(y)))).box()[0]);

          else if constexpr(std::is_same_v<T,IntervalVector>)
          {
            Interval t_(t);
            for(Index i = 0 ; i < size() ; i++)
              if(!t_.is_empty())
                t_ &= untrunc((polygon_slice_i(v,i) & ConvexPolygon(cart_prod(t_,trunc(y[i])))).box()[0]);
            return t_;
          }
        }
      }

      /**
       * \brief Stream output for a slice
       *
       * \param os output stream
       * \param x slice to be displayed
       * \return reference to the output stream
       */
      friend inline std::ostream& operator<<(std::ostream& os, const Slice& x)
      {
        os << x.t0_tf()
           << "->" << x.codomain()
           << std::flush;
        return os;
      }

      /**
       * \brief Returns the unbounded value associated with this codomain type
       *
       * \return unbounded value of type ``T``
       */
      inline T all_reals_value() const
      {
        T x = codomain();
        x.init();
        return x;
      }

      /**
       * \brief Returns the empty value associated with this codomain type
       *
       * \return empty value of type ``T``
       */
      inline T empty_value() const
      {
        T x = codomain();
        x.set_empty();
        return x;
      }
      

    protected:

      template<typename T_>
      friend class SlicedTube;

      /**
       * \brief Sets this codomain to the empty set
       *
       * \param propagate if set to ``true``, propagates the update to adjacent gates
       */
      inline void set_empty(bool propagate)
      {
        this->T::set_empty();
        if(propagate)
          update_adjacent_codomains();
      }

      /**
       * \brief Propagates codomain contractions to adjacent gates
       *
       * This method preserves consistency between a slice and its neighboring
       * gates when one of them is contracted.
       */
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