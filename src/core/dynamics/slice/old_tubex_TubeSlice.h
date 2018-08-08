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

#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DynamicalItem.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
{
  class TubeVector;
  class TrajectoryVector;

  class Slice : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      Slice(const ibex::Interval& domain, int dim = 1);
      Slice(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      Slice(const Slice& x);
      ~Slice();
      const Slice& operator=(const Slice& x);
      const ibex::Interval domain() const;
      int dim() const;

      // Slices structure
      Slice* prev_slice();
      const Slice* prev_slice() const;
      Slice* next_slice();
      const Slice* next_slice() const;
      static void chain_slices(Slice *first_slice, Slice *second_slice);
      const ibex::IntervalVector input_gate() const;
      const ibex::IntervalVector output_gate() const;
      const ConvexPolygon polygon(int i, const Slice& v) const; // todo: store polygon in cache memory?
      const TubeVector* tube_reference() const;

      // Access values
      const ibex::IntervalVector codomain() const;
      const ibex::IntervalVector box() const;
      double volume() const;
      const ibex::IntervalVector operator()(double t) const;
      const ibex::IntervalVector operator()(const ibex::Interval& search_domain) const;
      const ibex::IntervalVector interpol(double t, const Slice& v) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const Slice& v) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const Slice& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const Slice& x) const;
      bool operator!=(const Slice& x) const;
      bool is_subset(const Slice& x) const;
      bool is_strict_subset(const Slice& x) const;
      bool is_empty() const;
      bool encloses(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set_empty();
      void set_all_reals();
      void set_all_reals(int start_index, int end_index);
      void set_envelope(const ibex::IntervalVector& envelope);
      void set_input_gate(const ibex::IntervalVector& input_gate);
      void set_output_gate(const ibex::IntervalVector& output_gate);
      const Slice& inflate(double rad);

      // Operators
      Slice& operator+=(const TrajectoryVector& x);
      Slice& operator+=(const Slice& x);
      Slice& operator-=(const TrajectoryVector& x);
      Slice& operator-=(const Slice& x);
      Slice& operator*=(const TrajectoryVector& x);
      Slice& operator*=(const Slice& x);
      Slice& operator/=(const TrajectoryVector& x);
      Slice& operator/=(const Slice& x);
      Slice& operator|=(const TrajectoryVector& x);
      Slice& operator|=(const Slice& x);
      Slice& operator&=(const TrajectoryVector& x);
      Slice& operator&=(const Slice& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Slice& x);
      const std::string class_name() const { return "Slice"; };
      

    protected:

      void resize(int n);
      void set_tube_ref(TubeVector *tube_ref);
      void set_domain(const ibex::Interval& domain);
      const ibex::IntervalVector codomain_box() const;

    /** Class variables **/

      ibex::Interval m_domain;
      ibex::IntervalVector m_codomain = ibex::IntervalVector(1); // by default: 1-dim codomain
      ibex::IntervalVector *m_input_gate = NULL, *m_output_gate = NULL;
      Slice *m_prev_slice = NULL, *m_next_slice = NULL;
      TubeVector *m_tube_ref = NULL; // a reference to the tube owning the node (used for data-structure's auto updates)

      friend class Tube;
      friend class TubeVector;
      friend void deserialize_tubevector(std::ifstream& bin_file, TubeVector& tube);
  };
}

#endif