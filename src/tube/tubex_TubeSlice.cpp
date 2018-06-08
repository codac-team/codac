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
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeSlice::TubeSlice(const Interval& domain, const Interval& codomain) : TubeComponent(domain, codomain)
    {
      m_input_gate = new Interval();
      m_output_gate = new Interval();
      set(codomain);
    }

    TubeSlice::TubeSlice(const TubeSlice& x) : TubeComponent(x)
    {
      m_input_gate = new Interval();
      m_output_gate = new Interval();
      *this = x;
    }

    TubeSlice::~TubeSlice()
    {
      // Gates are deleted if not shared with other slices

      if(m_prev_slice == NULL)
      {
        delete m_input_gate;
        m_input_gate = NULL;
      }

      if(m_next_slice == NULL)
      {
        delete m_output_gate;
        m_output_gate = NULL;
      }
    }

    TubeSlice& TubeSlice::operator=(const TubeSlice& x)
    {
      TubeComponent::operator=(x);
      *m_input_gate = *x.m_input_gate;
      *m_output_gate = *x.m_output_gate;
      flagFutureDataUpdateFromLeaf();
      return *this;
    }

    // Slices structure
    
    bool TubeSlice::isSlice() const
    {
      return true;
    }

    TubeSlice* TubeSlice::getSlice(int slice_id)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeSlice*>(this)->getSlice(slice_id));
    }

    const TubeSlice* TubeSlice::getSlice(int slice_id) const
    {
      DomainException::check(*this, slice_id);
      return this;
    }

    TubeSlice* TubeSlice::getSlice(double t)
    {
      return const_cast<TubeSlice*>(static_cast<const TubeSlice*>(this)->getSlice(t));
    }

    const TubeSlice* TubeSlice::getSlice(double t) const
    {
      DomainException::check(*this, t);
      return this;
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

    TubeSlice* TubeSlice::prevSlice() const
    {
      return m_prev_slice;
    }

    TubeSlice* TubeSlice::nextSlice() const
    {
      return m_next_slice;
    }

    void TubeSlice::chainSlices(TubeSlice *first_slice, TubeSlice *second_slice)
    {
      if(first_slice != NULL)
        first_slice->m_next_slice = second_slice;

      if(second_slice != NULL)
        second_slice->m_prev_slice = first_slice;

      if(first_slice != NULL && second_slice != NULL)
      {
        *first_slice->m_output_gate &= *second_slice->m_input_gate;
        delete second_slice->m_input_gate;
        second_slice->m_input_gate = first_slice->m_output_gate;

        first_slice->flagFutureDataUpdateFromLeaf();
        second_slice->flagFutureDataUpdateFromLeaf();
      }
    }

    const Interval TubeSlice::inputGate() const
    {
      return *m_input_gate;
    }
    
    const Interval TubeSlice::outputGate() const
    {
      return *m_output_gate;
    }
    
    TubeNode* TubeSlice::getParentOf(TubeComponent* component)
    {
      return NULL;
    }
    
    // Access values

    const Interval& TubeSlice::codomain() const
    {
      return m_codomain;
    }
    
    const IntervalVector TubeSlice::box() const
    {
      IntervalVector box(2);
      box[0] = domain();
      box[1] = codomain();
      return box;
    }
    
    double TubeSlice::volume() const
    {
      checkData();
      return m_volume;
    }

    const Interval TubeSlice::operator[](int slice_id) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      DomainException::check(*this, slice_id);
      return m_codomain;
    }

    const Interval TubeSlice::operator[](double t) const
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

    const Interval TubeSlice::operator[](const Interval& t) const
    {
      // Write access is not allowed for this operator:
      // a further call to checkDataTree() is needed when values change,
      // this call cannot be garanteed with a direct access to m_codomain
      // For write access: use set()
      DomainException::check(*this, t);
      if(t.is_degenerated())
        return (*this)[t.lb()];
      return m_codomain;
    }

    const Interval TubeSlice::interpol(double t, const TubeSlice& derivative) const
    {
      return interpol(Interval(t), derivative);

      #if false // old implementation (faster?) todo:check
        DomainException::check(*this, derivative);
        EmptyException::check(derivative);
        return (outputGate() - (m_domain.ub() - t) * derivative.codomain())
             & (inputGate() + (t - m_domain.lb()) * derivative.codomain());
      #endif
    }

    const Interval TubeSlice::interpol(const Interval& t, const TubeSlice& derivative) const
    {
      Interval y;
      CtcDeriv ctc;
      Interval t_ = t;
      ctc.contract(*this, derivative, t_, y);
      return y;
    }

    Interval TubeSlice::invert(const Interval& y, const Interval& search_domain) const
    {
      if(!m_domain.intersects(search_domain) || !m_codomain.intersects(y))
        return Interval::EMPTY_SET;

      else
        return search_domain & m_domain;
    }

    void TubeSlice::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      return invert(y, v_t, search_domain, true);
    }

    const pair<Interval,Interval> TubeSlice::eval(const Interval& t) const
    {
      Interval intersection = t & m_domain;

      if(intersection.is_empty())
        return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      else
      {
        Interval lb = Interval::EMPTY_SET;
        Interval ub = Interval::EMPTY_SET;

        if(intersection.contains(m_domain.lb()))
        {
          lb |= inputGate().lb();
          ub |= inputGate().ub();
        }

        if(intersection.contains(m_domain.ub()))
        {
          lb |= outputGate().lb();
          ub |= outputGate().ub();
        }

        if(!intersection.is_degenerated())
        {
          lb |= m_codomain.lb();
          ub |= m_codomain.ub();
        }

        return make_pair(lb, ub);
      }
    }

    // Tests
    
    bool TubeSlice::operator==(const TubeSlice& x) const
    {
      return TubeComponent::operator==(x) && 
             inputGate() == x.inputGate() &&
             outputGate() == x.outputGate();
    }
    
    bool TubeSlice::operator!=(const TubeSlice& x) const
    {
      return TubeComponent::operator!=(x) ||
             inputGate() != x.inputGate() ||
             outputGate() != x.outputGate();
    }

    bool TubeSlice::isSubset(const TubeSlice& x) const
    {
      StructureException::check(*this, x);
      return TubeComponent::isSubset(x)
          && inputGate().is_subset(x.inputGate())
          && outputGate().is_subset(x.outputGate());
    }

    bool TubeSlice::isStrictSubset(const TubeSlice& x) const
    {
      return isSubset(x) && (*this) != x;
    }
    
    bool TubeSlice::isEmpty() const
    {
      return TubeComponent::isEmpty() || inputGate().is_empty() || outputGate().is_empty();
    }

    bool TubeSlice::encloses(const Trajectory& x) const
    {
      return TubeComponent::encloses(x)
          && inputGate().contains(x[m_domain.lb()])
          && outputGate().contains(x[m_domain.ub()]);
    }

    // Setting values

    void TubeSlice::set(const Interval& y)
    {
      setEnvelope(y);
      setInputGate(y);
      setOutputGate(y);
    }
    
    void TubeSlice::setEmpty()
    {
      set(Interval::EMPTY_SET);
    }

    void TubeSlice::setEnvelope(const Interval& envelope)
    {
      m_codomain = envelope;
      *m_input_gate &= m_codomain;
      *m_output_gate &= m_codomain;

      flagFutureDataUpdateFromLeaf();
    }

    void TubeSlice::setInputGate(const Interval& input_gate)
    {
      *m_input_gate = input_gate;
      *m_input_gate &= m_codomain;

      if(prevSlice() != NULL)
        *m_input_gate &= prevSlice()->codomain();

      flagFutureDataUpdateFromLeaf();
    }

    void TubeSlice::setOutputGate(const Interval& output_gate)
    {
      *m_output_gate = output_gate;
      *m_output_gate &= m_codomain;

      if(nextSlice() != NULL)
        *m_output_gate &= nextSlice()->codomain();

      flagFutureDataUpdateFromLeaf();
    }
    
    TubeComponent& TubeSlice::inflate(double rad)
    {
      // todo: simplify with simple addition
      Interval e(-rad,rad);
      setEnvelope(m_codomain + e);
      setInputGate(*m_input_gate + e);
      setOutputGate(*m_output_gate + e);
    }
    
    // String

    ostream& operator<<(ostream& str, const TubeSlice& x)
    {
      str << "Slice " << x.domain() << "↦(" << x.inputGate() << ")" << x.codomain() << "(" << x.outputGate() << ")" << flush;
      return str;
    }

    // Integration


  // Protected methods

    void TubeSlice::setTubeReference(Tube *tube_ref)
    {
      m_tube_ref = tube_ref;
    }

    // Slices structure

    void TubeSlice::updateSlicesNumber()
    {
      m_slices_number = 1;
    }

    // Access values

    void TubeSlice::invert(const Interval& y, vector<ibex::Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
    {
      Interval inversion = invert(y, search_domain);
      if(!inversion.is_empty())
        v_t.push_back(inversion);
    }

    // Setting values
    
    void TubeSlice::checkData() const
    {
      if(!m_data_update_needed)
        return;

      // Volume
      {
        if(m_codomain.is_empty()) // ibex::diam(EMPTY_SET) is not 0, todo: check this
          m_volume = 0.;

        else if(m_codomain.is_unbounded())
          m_volume = INFINITY;

        else
          m_volume = m_domain.diam() * m_codomain.diam();
      }

      m_data_update_needed = false;
    }

    void TubeSlice::flagFutureDataUpdateFromRoot(int slice_id) const
    {
      // slice_id is not used in this class
      m_data_update_needed = true;
      m_primitive_update_needed = true;
    }

    void TubeSlice::flagFutureDataUpdateFromLeaf() const
    {
      if(!m_data_update_needed)
      {
        m_data_update_needed = true;
        m_primitive_update_needed = true;

        if(m_tube_ref != NULL)
        {
          int slice_id = m_tube_ref->input2index(m_domain.mid());
          m_tube_ref->m_component->flagFuturePrimitiveUpdateFromRoot(slice_id);
          m_tube_ref->m_component->flagFutureDataUpdateFromRoot(slice_id);
        }
      }
    }

    // Integration

    void TubeSlice::checkPartialPrimitive() const
    {
      if(!m_primitive_update_needed)
        throw Exception("TubeSlice::checkPartialPrimitive", "primitive value not set");
      // should be set from Tube class
    }

    void TubeSlice::flagFuturePrimitiveUpdateFromRoot(int slice_id) const
    {
      // slice_id is not used in this class
      m_primitive_update_needed = true;
    }

    void TubeSlice::flagFuturePrimitiveUpdateFromLeaf() const
    {
      if(!m_primitive_update_needed)
      {
        m_primitive_update_needed = true;

        if(m_tube_ref != NULL)
        {
          int slice_id = m_tube_ref->input2index(m_domain.mid());
          m_tube_ref->m_component->flagFuturePrimitiveUpdateFromRoot(slice_id);
        }
      }
    }

    const pair<Interval,Interval>& TubeSlice::getPartialPrimitiveValue() const
    {
      checkPartialPrimitive();
      return m_partial_primitive;
    }

    pair<Interval,Interval> TubeSlice::getPartialPrimitiveValue(const Interval& t) const
    {
      if(t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
      {
        checkPartialPrimitive();
        return m_partial_primitive;
      }

      else
      {
        throw Exception("TubeSlice::getPartialPrimitiveValue", "unexpected case");
      }
    }
}