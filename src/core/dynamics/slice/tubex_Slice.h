/* ============================================================================
 *  tubex-lib - Slice class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBESLICE_H__
#define __TUBEX_TUBESLICE_H__

#include "tubex_Tube.h"
#include "tubex_Trajectory.h"
#include "tubex_DynamicalItem.h"
#include "tubex_ConvexPolygon.h"
#include "tubex_TubeTreeSynthesis.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  /**
   * \class Slice
   * \brief Slice of a one dimensional tube and made of an envelope and two gates
   */

  #define EPSILON_CONTAINS ibex::next_float(0.) * 1000.

  class Tube;
  class Trajectory;

  class Slice : public DynamicalItem
  {
    public:

    // Base:

      // Definition

      /**
       * \brief Creates a slice \f$\llbracket x\rrbracket\f$
       *
       * \param domain Interval domain \f$[t^k_0,t^k_f]\f$
       * \param codomain Interval value of the slice (all reals \f$[-\infty,\infty]\f$ by default)
       */
      Slice(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);

      /**
       * \brief Creates a copy of the slice \f$\llbracket x\rrbracket\f$
       *
       * \param x Slice to be duplicated
       */
      Slice(const Slice& x);

      /**
       * \brief Slice destructor
       */
      ~Slice();

      /**
       * \brief Returns the dimension of the slice (always 1)
       *
       * \return 1
       */
      int size() const;

      /**
       * \brief Returns a copy of a Slice
       *
       * \param x the Slice object to be copied
       * \return a new Slice object with same domain and codomain
       */
      const Slice& operator=(const Slice& x);

      /**
       * \brief Returns the temporal definition domain of this slice
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      const ibex::Interval domain() const;

      // Slices structure

      /**
       * \brief Points to the previous slice \f$\llbracket x\rrbracket(k-1)\f$
       *
       * \return a pointer to a Slice object, or NULL is this slice is the first one
       */
      Slice* prev_slice();

      /**
       * \brief Points to the previous slice \f$\llbracket x\rrbracket(k-1)\f$
       *
       * \return a const pointer to a Slice object, or NULL is this slice is the first one
       */
      const Slice* prev_slice() const;

      /**
       * \brief Points to the next slice \f$\llbracket x\rrbracket(k+1)\f$
       *
       * \return a pointer to a Slice object, or NULL is this slice is the last one
       */
      Slice* next_slice();

      /**
       * \brief Points to the next slice \f$\llbracket x\rrbracket(k+1)\f$
       *
       * \return a const pointer to a Slice object, or NULL is this slice is the last one
       */
      const Slice* next_slice() const;

      /**
       * \brief Returns the value of the input gate \f$\llbracket x\rrbracket(t_0)\f$ of this slice
       *
       * \return the interval input gate
       */
      const ibex::Interval input_gate() const;

      /**
       * \brief Returns the value of the output gate \f$\llbracket x\rrbracket(t_f)\f$ of this slice
       *
       * \return the interval output gate
       */
      const ibex::Interval output_gate() const;

      /**
       * \brief Computes a convex polygon that optimaly encloses the values of the slice
       *        according to the knowledge of the derivative slice \f$\llbracket v\rrbracket\f$
       *
       * \todo Store the polygon in cache memory?
       *
       * \param v the derivative slice
       * \return a ConvexPolygon object
       */
      const ConvexPolygon polygon(const Slice& v) const;

      // Accessing values

      /**
       * \brief Returns the envelope of the slice
       *
       * \return the Interval object \f$\llbracket x\rrbracket([t_0,t_f])\f$
       */
      const ibex::Interval codomain() const;

      /**
       * \brief Computes the two-dimensional box \f$[t_0,t_f]\times\llbracket x\rrbracket([t_0,t_f])\f$
       *
       * \return the slice box
       */
      const ibex::IntervalVector box() const;

      /**
       * \brief Returns the volume of this slice
       *
       * \note returns POS_INFINITY if the codomain is unbounded
       * \note returns 0 if the slice is flat (and so without wrapping effect)
       *
       * \return volume defined as \f$w([t_0,t_f])\times w(\llbracket x\rrbracket([t_0,t_f]))\f$
       */
      double volume() const;

      /**
       * \brief Returns the evaluation of this slice at \f$t\f$
       *
       * \note The returned value is either the input gate, the envelope or the output gate
       *
       * \param t the temporal key (double, must belong to the Slice domain)
       * \return Interval value of \f$\llbracket x\rrbracket(t)\f$
       */
      const ibex::Interval operator()(double t) const;

      /**
       * \brief Returns the interval evaluation of this slice over \f$[t]\f$
       *
       * \param t the subdomain (Interval, must be a subset of the Slice domain)
       * \return Interval envelope \f$\llbracket x\rrbracket([t])\f$
       */
      const ibex::Interval operator()(const ibex::Interval& t) const;

      /**
       * \brief Returns the interval evaluations of the bounds of the slice over \f$[t]\f$
       *
       * \note It includes the lower and upper bounds of the gates
       *
       * \param t the subdomain (Interval, must be a subset of the Slice domain)
       * \return the pair \f$\big(\llbracket\underline{x^-}\rrbracket([t]),\llbracket\overline{x^+}\rrbracket([t])\big)\f$
       */
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Returns the optimal evaluation of this slice at \f$t\f$,
       *        based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the temporal key (double, must belong to the Slice domain)
       * \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \return Interval value of \f$\llbracket x\rrbracket(t)\f$
       */
      const ibex::Interval interpol(double t, const Slice& v) const;

      /**
       * \brief Returns the optimal evaluation of this slice over \f$[t]\f$,
       *        based on the derivative information \f$\dot{x}(\cdot)\f$
       *
       * \todo Change the name of this method?
       *
       * \param t the subdomain (Interval, must be a subset of the Slice domain)
       * \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \return Interval value of \f$\llbracket x\rrbracket([t])\f$
       */
      const ibex::Interval interpol(const ibex::Interval& t, const Slice& v) const;

      /**
       * \brief Returns the interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
       *
       * \param y the interval codomain
       * \param search_domain the optional interval domain on which the inversion will be performed
       * \return the hull of \f$\llbracket x\rrbracket^{-1}([y])\f$
       */
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Returns the optimal interval inversion \f$\llbracket x\rrbracket^{-1}([y])\f$
       *
       * \note The knowledge of the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$ allows a finer inversion
       *
       * \param y the interval codomain
       * \param v the derivative slice such that \f$\dot{x}(\cdot)\in\llbracket v\rrbracket(\cdot)\f$
       * \param search_domain the optional interval domain on which the inversion will be performed
       * \return the hull of \f$\llbracket x\rrbracket^{-1}([y])\f$
       */
      const ibex::Interval invert(const ibex::Interval& y, const Slice& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;

      // Tests

      /**
       * \brief Returns true if this slice is equal to \f$\llbracket x\rrbracket(\cdot)\f$
       *
       * \note Equality is obtained if the slices share
       *       the same gates, codomain and domain
       *
       * \param x the Slice object
       * \return true in case of equality
       */
      bool operator==(const Slice& x) const;

      /**
       * \brief Returns true if this slice is different from \f$\llbracket x\rrbracket(\cdot)\f$
       *
       * \note The two slices are different if they do not share
       *       the same gates, codomain or domain
       *
       * \param x the Slice object
       * \return true in case of difference
       */
      bool operator!=(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a subset of \f$\llbracket x\rrbracket(\cdot)\f$
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of subset
       */
      bool is_subset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a subset of \f$\llbracket x\rrbracket(\cdot)\f$,
       *        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of strict subset
       */
      bool is_strict_subset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a subset of the interior of \f$\llbracket x\rrbracket(\cdot)\f$
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of interior subset
       */
      bool is_interior_subset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a subset of the interior of \f$\llbracket x\rrbracket(\cdot)\f$,
       *        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of strict interior subset
       */
      bool is_strict_interior_subset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a superset of \f$\llbracket x\rrbracket(\cdot)\f$
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of superset
       */
      bool is_superset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is a superset of \f$\llbracket x\rrbracket(\cdot)\f$,
       *        and not \f$\llbracket x\rrbracket(\cdot)\f$ itself
       *
       * \note The two slices must have the same definition domain
       *
       * \param x the Slice object
       * \return true in case of strict superset
       */
      bool is_strict_superset(const Slice& x) const;

      /**
       * \brief Returns true if this slice is empty
       *
       * \note If the input gate, the envelope or the output gate is empty,
       *       then the slice is considered empty
       *
       * \return true in case of emptiness
       */
      bool is_empty() const;

      /**
       * \brief Returns true if this slice contains the trajectory \f$x(\cdot)\f$
       *
       * \note Due to the reliable numerical representation of a trajectory, some wrapping effect
       *       may appear for its evaluations (either if it is defined by a map of values
       *       or an analytical function). Hence, this "contains" test may not be able to
       *       conclude, if the thin envelope of \f$x(\cdot)\f$ overlaps a boundary of the tube.
       * \note The domain of this slice must be a subset of the domain of \f$x(\cdot)\f$
       *
       * \return BoolInterval::YES (or BoolInterval::NO) if this slice contains \f$x(\cdot)\f$
       *         (or does not contain) and BoolInterval::MAYBE in case of ambiguity
       */
      const ibex::BoolInterval contains(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set_empty();
      void set_envelope(const ibex::Interval& envelope);
      void set_input_gate(const ibex::Interval& input_gate);
      void set_output_gate(const ibex::Interval& output_gate);
      const Slice& inflate(double rad);

      // Operators
      Slice& operator+=(const Trajectory& x);
      Slice& operator+=(const Slice& x);
      Slice& operator-=(const Trajectory& x);
      Slice& operator-=(const Slice& x);
      Slice& operator*=(const Trajectory& x);
      Slice& operator*=(const Slice& x);
      Slice& operator/=(const Trajectory& x);
      Slice& operator/=(const Slice& x);
      Slice& operator|=(const Trajectory& x);
      Slice& operator|=(const Slice& x);
      Slice& operator&=(const Trajectory& x);
      Slice& operator&=(const Slice& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Slice& x);
      const std::string class_name() const { return "Slice"; };
      

    protected:

      void set_domain(const ibex::Interval& domain);
      const ibex::IntervalVector codomain_box() const;
      static void chain_slices(Slice *first_slice, Slice *second_slice);

      // Class variables:

        ibex::Interval m_domain;
        ibex::Interval m_codomain = ibex::Interval::ALL_REALS;
        ibex::Interval *m_input_gate = NULL, *m_output_gate = NULL;
        Slice *m_prev_slice = NULL, *m_next_slice = NULL;
        mutable TubeTreeSynthesis *m_synthesis_reference = NULL;

      friend class Tube;
      friend class TubeTreeSynthesis;
      friend void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);
  };
}

#endif