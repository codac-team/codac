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
      set(codomain);
    }

    TubeSlice::TubeSlice(const TubeSlice& x) : TubeNode(x)
    {
      *this = x;
    }

    TubeSlice::~TubeSlice()
    {
      
    }

    TubeSlice& TubeSlice::operator=(const TubeSlice& x)
    {
      TubeNode::operator=(x);
      return *this;
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
    
    void TubeSlice::getTubeNodes(vector<const TubeNode*> &v_nodes) const
    {
      v_nodes.push_back(static_cast<const TubeNode*>(this));
    }

    TubeSlice* TubeSlice::prevSlice() const
    {
      return m_prev_slice;
    }

    TubeSlice* TubeSlice::nextSlice() const
    {
      return m_next_slice;
    }

    void TubeSlice::chainSlices(TubeSlice *first_slice, TubeSlice *second_slice, Interval *gate)
    {
      if(first_slice != NULL)
      {
        first_slice->m_next_slice = second_slice;

        if(gate != NULL)
        {
          if(first_slice->m_output_gate != NULL)
            throw Exception("TubeSlice::chainSlices", "gate already existing");
          first_slice->m_output_gate = gate;
        }

        else if(second_slice != NULL && first_slice->m_output_gate == NULL)
          first_slice->m_output_gate = second_slice->m_input_gate;
      }

      if(second_slice != NULL)
      {
        second_slice->m_prev_slice = first_slice;

        if(gate != NULL)
        {
          if(second_slice->m_input_gate != NULL)
            throw Exception("TubeSlice::chainSlices", "gate already existing");
          second_slice->m_input_gate = gate;
        }

        else if(first_slice != NULL && second_slice->m_input_gate == NULL)
          second_slice->m_input_gate = first_slice->m_output_gate;
      }
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
    
    // Access values

    const Interval& TubeSlice::codomain() const
    {
      return m_codomain;
    }
    
    double TubeSlice::volume() const
    {
      return m_volume;
    }

    const Interval& TubeSlice::operator[](int slice_id) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      DomainException::check(*this, slice_id);
      return m_codomain;
    }

    Interval TubeSlice::operator[](double t) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      DomainException::check(*this, t);

      if(t == m_domain.lb())
        return inputGate();

      else if(t == m_domain.ub())
        return outputGate();

      return m_codomain;
    }

    Interval TubeSlice::operator[](const Interval& t) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      DomainException::check(*this, t);
      return m_codomain;
    }
/*
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
    */

    // Setting values

    void TubeSlice::set(const Interval& y)
    {
      m_codomain = y;
      m_enclosed_bounds = make_pair(Interval(m_codomain.lb()), Interval(m_codomain.ub()));

      m_volume = m_domain.diam();

      if(m_codomain.is_empty()) // ibex::diam(EMPTY_SET) is not 0, todo: check this
        m_volume = 0.;

      else if(m_codomain.is_unbounded())
        m_volume = INFINITY;

      else
        m_volume *= m_codomain.diam();
    }
    
    void TubeSlice::setEmpty()
    {
      set(Interval::EMPTY_SET);
    }
    
    /*
    // String

    ostream& operator<<(ostream& str, const TubeSlice& x)
    {
      cout << "TubeSlice: t=" << x.domain() << ", y=" << x.codomain() << flush;
      return str;
    }
    */
}