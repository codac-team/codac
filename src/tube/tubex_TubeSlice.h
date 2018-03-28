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
      TubeSlice* prevSlice() const;
      void setPrevSlice(TubeSlice *prev_slice);
      TubeSlice* nextSlice() const;
      void setNextSlice(TubeSlice *next_slice);
      const ibex::Interval inputGate() const;
      const ibex::Interval outputGate() const;
      void setInputGate(const ibex::Interval& input_gate);
      void setOuputGate(const ibex::Interval& output_gate);

      // Access values
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

    protected:

    /** Class variables **/

      TubeSlice *m_prev_slice = NULL, *m_next_slice = NULL;
      ibex::Interval *m_input_gate = NULL, *m_output_gate = NULL;
  };
}

#endif