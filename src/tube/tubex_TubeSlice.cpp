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
  TubeSlice::TubeSlice(const Interval& domain, const Interval& image) : TubeNode(domain, image)
  {

  }

  TubeSlice::~TubeSlice()
  {
    if(m_input_gate != NULL) delete m_input_gate;
  }

  TubeSlice* TubeSlice::nextSlice() const
  {
    return m_next_slice;
  }

  void TubeSlice::setNextSlice(TubeSlice *next_slice)
  {
    m_next_slice = next_slice;
    m_next_slice->m_input_gate = m_output_gate;
  }

  TubeSlice* TubeSlice::prevSlice() const
  {
    return m_prev_slice;
  }

  void TubeSlice::setPrevSlice(TubeSlice *prev_slice)
  {
    m_prev_slice = prev_slice;
    m_prev_slice->m_output_gate = m_input_gate;
  }

  const Interval TubeSlice::inputGate() const
  {
    if(m_input_gate != NULL)
      return *m_input_gate;

    else if(m_prev_slice != NULL)
      return codomain() & m_prev_slice->codomain();

    else
      return codomain();
  }
  
  const Interval TubeSlice::outputGate() const
  {
    if(m_output_gate != NULL)
      return *m_output_gate;

    else if(m_next_slice != NULL)
      return codomain() & m_next_slice->codomain();

    else
      return codomain();
  }
  
  void TubeSlice::setInputGate(const Interval& input_gate)
  {
    if(m_input_gate == NULL)
      m_input_gate = new Interval(input_gate);

    else
      *m_input_gate = input_gate;
  }
  
  void TubeSlice::setOuputGate(const Interval& output_gate)
  {
    if(m_output_gate == NULL)
      m_output_gate = new Interval(output_gate);

    else
      *m_output_gate = output_gate;
  }
}