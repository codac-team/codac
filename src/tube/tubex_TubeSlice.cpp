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
#include "tubex_DomainException.h"
#include "tubex_StructureException.h"
#include "tubex_EmptyException.h"

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
    
    bool TubeSlice::isSlice() const
    {
      return true;
    }
    
    TubeSlice* TubeSlice::getSlice(int slice_id)
    {
      DomainException::check(*this, slice_id);
      return this;
    }

    const TubeSlice* TubeSlice::getSlice(int slice_id) const
    {
      return const_cast<TubeSlice*>(getSlice(slice_id));
    }

    TubeSlice* TubeSlice::getSlice(double t)
    {
      DomainException::check(*this, t);
      return this;
    }

    const TubeSlice* TubeSlice::getSlice(double t) const
    {
      return const_cast<TubeSlice*>(getSlice(t));
    }

    void TubeSlice::getSlices(vector<const TubeSlice*>& v_slices) const
    {
      v_slices.push_back(this);
    }
    
    int TubeSlice::input2index(double t) const
    {
      DomainException::check(*this, t);
      return 0;
    }
    
    const IntervalVector TubeSlice::sliceBox(int slice_id) const
    {
      IntervalVector box(2);
      box[0] = m_domain;
      box[1] = m_codomain;
      return box;
    }
    
    const Interval& TubeSlice::sliceDomain(int slice_id) const
    {
      return m_domain;
    }
    
    const Interval& TubeSlice::sliceDomain(double t) const
    {
      return m_domain;
    }

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

    const Interval& TubeSlice::codomain() const
    {
      return m_codomain;
    }

    double TubeSlice::volume() const
    {
      return m_volume;
    }

    Interval TubeSlice::interpol(double t, const TubeSlice& derivative) const
    {
      DomainException::check(*this, t);
      StructureException::check(*this, derivative);
      EmptyException::check(derivative);
      Interval deriv_value = derivative.codomain();
      return Interval::EMPTY_SET; // todo: gates in computations
    }
    
    Interval TubeSlice::invert(const Interval& y, const Interval& t) const
    {
      if(!m_domain.intersects(t) || !m_codomain.intersects(y))
        return Interval::EMPTY_SET;

      else
        return t & m_domain;
    }
    
    double TubeSlice::maxThickness(int& first_id_max_thickness) const
    {
      first_id_max_thickness = 0;
      return m_codomain.diam();
    }

    const pair<Interval,Interval> TubeSlice::eval(const Interval& t) const
    {
      if(!t.intersects(m_domain))
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);
      return make_pair(m_codomain, m_codomain);
    }

    // Tests

    bool TubeSlice::isInteriorSubset(const TubeNode& outer_tube) const
    {
      StructureException::check(*this, outer_tube);
      return m_codomain.is_interior_subset(outer_tube.codomain());
    }

    bool TubeSlice::encloses(const Trajectory& x) const
    {
      return x[m_domain].is_interior_subset(m_codomain);
    }
    
    // String

    ostream& operator<<(ostream& str, const TubeSlice& x)
    {
      cout << "TubeSlice: t=" << x.domain() << ", y=" << x.codomain() << flush;
      return str;
    }
}