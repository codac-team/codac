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

#include "tubex_TubeSlice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeSlice::TubeSlice(const Interval& domain, const Interval& codomain) : TubeNode(domain, codomain)
    {

    }

    TubeSlice::~TubeSlice()
    {
      if(m_input_gate != NULL)
      {
        delete m_input_gate;
        m_input_gate = NULL;
      }

      if(m_output_gate != NULL)
      {
        delete m_output_gate;
        m_output_gate = NULL;
      }

      if(m_prev_slice != NULL)
        m_prev_slice->m_next_slice = NULL;

      if(m_next_slice != NULL)
        m_next_slice->m_prev_slice = NULL;
    }

    // Slices structure

    TubeSlice* TubeSlice::prevSlice() const
    {
      return m_prev_slice;
    }

    void TubeSlice::setPrevSlice(TubeSlice *prev_slice)
    {
      m_prev_slice = prev_slice;
    }

    TubeSlice* TubeSlice::nextSlice() const
    {
      return m_next_slice;
    }

    void TubeSlice::setNextSlice(TubeSlice *next_slice)
    {
      m_next_slice = next_slice;
    }

    const Interval TubeSlice::inputGate() const
    {
      if(m_input_gate != NULL)
        return *m_input_gate;

      else if(m_prev_slice != NULL)
        return m_codomain & m_prev_slice->m_codomain;

      else
        return m_codomain;
    }
    
    const Interval TubeSlice::outputGate() const
    {
      if(m_output_gate != NULL)
        return *m_output_gate;

      else if(m_next_slice != NULL)
        return m_codomain & m_next_slice->m_codomain;

      else
        return m_codomain;
    }
    
    void TubeSlice::setInputGate(const Interval& input_gate)
    {
      if(m_input_gate == NULL)
      {
        m_input_gate = new Interval(input_gate);
        m_prev_slice->m_output_gate = m_input_gate;
      }

      else
        *m_input_gate = input_gate;
    }
    
    void TubeSlice::setOuputGate(const Interval& output_gate)
    {
      if(m_output_gate == NULL)
      {
        m_output_gate = new Interval(output_gate);
        m_next_slice->m_input_gate = m_output_gate;
      }

      else
        *m_output_gate = output_gate;
    }
    
    // Access values

    const pair<Interval,Interval> TubeSlice::eval(const Interval& t) const
    {
      return make_pair(m_codomain, m_codomain);
    }
}