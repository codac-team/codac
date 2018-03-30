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

#ifndef TUBESLICE_HEADER
#define TUBESLICE_HEADER

#include "tubex_TubeNode.h"

namespace tubex
{
  class TubeSlice : public TubeNode
  {
    public:

    /** Base: **/

      // Definition
      TubeSlice(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      ~TubeSlice();

      // Slices structure
      bool isSlice() const;
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      const ibex::IntervalVector sliceBox(int slice_id) const;
      const ibex::Interval& sliceDomain(int slice_id) const;
      const ibex::Interval& sliceDomain(double t) const;
      TubeSlice* prevSlice() const;
      void setPrevSlice(TubeSlice *prev_slice);
      TubeSlice* nextSlice() const;
      void setNextSlice(TubeSlice *next_slice);
      const ibex::Interval inputGate() const;
      const ibex::Interval outputGate() const;
      void setInputGate(const ibex::Interval& input_gate);
      void setOuputGate(const ibex::Interval& output_gate);

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      ibex::Interval interpol(double t, const TubeSlice& derivative) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      double maxThickness(int& first_id_max_thickness) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool isInteriorSubset(const TubeNode& outer_tube) const;
      bool encloses(const Trajectory& x) const;

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeSlice& x);

    protected:

    /** Class variables **/

      TubeSlice *m_prev_slice = NULL, *m_next_slice = NULL;
      ibex::Interval *m_input_gate = NULL, *m_output_gate = NULL;
  };
}

#endif