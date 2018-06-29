/* ============================================================================
 *  tubex-lib - TubeSlice class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TubeSlice_HEADER
#define TubeSlice_HEADER

#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DynamicalItem.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
{
  class TubeVector;

  class TubeSlice : public DynamicalItem
  {
    public:

    /** Base: **/

      // Definition
      TubeSlice(const ibex::Interval& domain, int dim = 1);
      TubeSlice(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      TubeSlice(const TubeSlice& x);
      ~TubeSlice();
      TubeSlice& operator=(const TubeSlice& x);
      const ibex::Interval domain() const; // todo: output const Interval& (reference)
      int dim() const;

      // Slices structure
      TubeSlice* prevSlice() const;
      TubeSlice* nextSlice() const;
      static void chainSlices(TubeSlice *first_slice, TubeSlice *second_slice);
      const ibex::IntervalVector inputGate() const;
      const ibex::IntervalVector outputGate() const;
      const ConvexPolygon polygon(int i, const TubeSlice& v) const; // todo: store polygon in cache memory?
      TubeVector* tubeReference() const;

      // Access values
      const ibex::IntervalVector codomain() const; // todo: output const Interval& (reference)
      const ibex::IntervalVector box() const;
      double volume() const;
      const ibex::IntervalVector operator[](double t) const;
      const ibex::IntervalVector operator[](const ibex::Interval& search_domain) const;
      const ibex::IntervalVector interpol(double t, const TubeSlice& derivative) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeSlice& derivative) const;
      ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const TubeSlice& x) const;
      bool operator!=(const TubeSlice& x) const;
      bool isSubset(const TubeSlice& x) const;
      bool isStrictSubset(const TubeSlice& x) const;
      bool isEmpty() const;
      bool encloses(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void setEmpty();
      void setEnvelope(const ibex::IntervalVector& envelope);
      void setInputGate(const ibex::IntervalVector& input_gate);
      void setOutputGate(const ibex::IntervalVector& output_gate);
      TubeSlice& inflate(double rad);

      // Operators
      TubeSlice& operator+=(const TrajectoryVector& x);
      TubeSlice& operator+=(const TubeSlice& x);
      TubeSlice& operator-=(const TrajectoryVector& x);
      TubeSlice& operator-=(const TubeSlice& x);
      TubeSlice& operator*=(const TrajectoryVector& x);
      TubeSlice& operator*=(const TubeSlice& x);
      TubeSlice& operator/=(const TrajectoryVector& x);
      TubeSlice& operator/=(const TubeSlice& x);
      TubeSlice& operator|=(const TrajectoryVector& x);
      TubeSlice& operator|=(const TubeSlice& x);
      TubeSlice& operator&=(const TrajectoryVector& x);
      TubeSlice& operator&=(const TubeSlice& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeSlice& x);
      const std::string className() const { return "TubeSlice"; };

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition
      void setTubeReference(TubeVector *tube_ref);
      void setDomain(const ibex::Interval& domain);

      // Slices structure

      // Access values
      const ibex::IntervalVector codomainBox() const; // todo: output const Interval& (reference)
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Setting values

      // Integration
      //const std::pair<ibex::IntervalVector,ibex::IntervalVector>& getPartialPrimitiveValue() const;
      //std::pair<ibex::IntervalVector,ibex::IntervalVector> getPartialPrimitiveValue(const ibex::Interval& t) const;
      
    /** Class variables **/

      ibex::Interval m_domain;
      ibex::IntervalVector m_codomain = ibex::IntervalVector(1); // by default: 1-dim codomain
      ibex::IntervalVector *m_input_gate = NULL, *m_output_gate = NULL;
      TubeSlice *m_prev_slice = NULL, *m_next_slice = NULL;
      TubeVector *m_tube_ref = NULL; // a reference to the tube owning the node (used for data-structure's auto updates)

      friend class TubeVector;
      friend void serializeTubeVector(std::ofstream& bin_file, const TubeVector& tube, int version_number);
      friend void deserializeTubeVector(std::ifstream& bin_file, TubeVector& tube);
  };
}

#endif