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

namespace tubex
{
  class Tube;
  class Trajectory;

  class Slice : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      Slice(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      Slice(const Slice& x);
      ~Slice();
      int size() const;
      const Slice& operator=(const Slice& x);
      const ibex::Interval domain() const;

      // Slices structure
      Slice* prev_slice();
      const Slice* prev_slice() const;
      Slice* next_slice();
      const Slice* next_slice() const;
      static void chain_slices(Slice *first_slice, Slice *second_slice);
      const ibex::Interval input_gate() const;
      const ibex::Interval output_gate() const;
      const ConvexPolygon polygon(const Slice& v) const; // todo: store polygon in cache memory?
      const Tube* tube_reference() const;

      // Access values
      const ibex::Interval codomain() const;
      const ibex::IntervalVector box() const;
      double volume() const;
      const ibex::Interval operator()(double t) const;
      const ibex::Interval operator()(const ibex::Interval& search_domain) const;
      const ibex::Interval interpol(double t, const Slice& v) const;
      const ibex::Interval interpol(const ibex::Interval& t, const Slice& v) const;
      const ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::Interval& y, const Slice& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const Slice& x) const;
      bool operator!=(const Slice& x) const;
      bool is_subset(const Slice& x) const;
      bool is_strict_subset(const Slice& x) const;
      bool is_empty() const;
      bool encloses(const Trajectory& x) const;

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

      void set_tube_ref(Tube *tube_ref);
      void set_domain(const ibex::Interval& domain);
      const ibex::IntervalVector codomain_box() const;

      /** Class variables **/

        ibex::Interval m_domain;
        ibex::Interval m_codomain = ibex::Interval::ALL_REALS;
        ibex::Interval *m_input_gate = NULL, *m_output_gate = NULL;
        Slice *m_prev_slice = NULL, *m_next_slice = NULL;
        Tube *m_tube_ref = NULL; // a reference to the tube owning the node (used for data-structure's auto updates)

      friend class Tube;
      friend void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);
  };
}

#endif