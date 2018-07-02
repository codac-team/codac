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
#include "tubex_DimensionException.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeSlice::TubeSlice(const Interval& domain, int dim)
      : TubeSlice(domain, IntervalVector(dim))
    {
      DomainException::check(domain);
      DimensionException::check(dim);
    }

    TubeSlice::TubeSlice(const Interval& domain, const IntervalVector& codomain)
      : m_domain(domain), m_codomain(codomain)
    {
      DomainException::check(domain);
      DimensionException::check(codomain.size());
      
      m_input_gate = new IntervalVector(codomain);
      m_output_gate = new IntervalVector(codomain);
    }

    TubeSlice::TubeSlice(const TubeSlice& x)
      : TubeSlice(x.domain(), x.codomain())
    {
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
      m_domain = x.m_domain;
      m_codomain = x.m_codomain;
      *m_input_gate = *x.m_input_gate;
      *m_output_gate = *x.m_output_gate;
      return *this;
    }
    
    const Interval TubeSlice::domain() const
    {
      return m_domain;
    }

    int TubeSlice::dim() const
    {
      return m_codomain.size();
    }

    // Slices structure

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
        DimensionException::check(*first_slice, *second_slice);

        *first_slice->m_output_gate &= *second_slice->m_input_gate;
        //delete second_slice->m_input_gate; // todo: check remaining unused gates
        second_slice->m_input_gate = first_slice->m_output_gate;
      }
    }

    const IntervalVector TubeSlice::inputGate() const
    {
      return *m_input_gate;
    }
    
    const IntervalVector TubeSlice::outputGate() const
    {
      return *m_output_gate;
    }

    TubeVector* TubeSlice::tubeReference() const
    {
      return m_tube_ref;
    }
    
    // Access values

    const IntervalVector TubeSlice::codomain() const
    {
      return codomainBox();
    }
    
    const IntervalVector TubeSlice::box() const
    {
      IntervalVector box(m_codomain.size() + 1);
      box[0] = domain();
      for(int i = 0 ; i < m_codomain.size() ; i++)
        box[i + 1] = m_codomain[i];
      return box;
    }
    
    double TubeSlice::volume() const
    {
      return m_codomain.volume() * m_domain.diam();

      // todo: check the following:
      //if(m_codomain.is_empty()) // ibex::diam(EMPTY_SET) is not 0, todo: check this
      //  return 0.;
      //
      //else if(m_codomain.is_unbounded())
      //  return INFINITY;
      //
      //else
      //  return m_domain.diam() * m_codomain.diam();
    }

    const IntervalVector TubeSlice::operator[](double t) const
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

    const IntervalVector TubeSlice::operator[](const Interval& t) const
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

    const IntervalVector TubeSlice::interpol(double t, const TubeSlice& derivative) const
    {
      DomainException::check(*this, t);
      DomainException::check(*this, derivative);
      DimensionException::check(*this, derivative);
      EmptyException::check(derivative);

      return interpol(Interval(t), derivative);

      #if false // todo: check this old implementation (faster?)
        DomainException::check(*this, derivative);
        EmptyException::check(derivative);
        return (outputGate() - (m_domain.ub() - t) * derivative.codomain())
             & (inputGate() + (t - m_domain.lb()) * derivative.codomain());
      #endif
    }

    const IntervalVector TubeSlice::interpol(const Interval& t, const TubeSlice& derivative) const
    {
      DomainException::check(*this, t);
      DomainException::check(*this, derivative);
      DimensionException::check(*this, derivative);
      EmptyException::check(derivative);
      
      IntervalVector interpol(dim(), Interval::EMPTY_SET);

      if(domain().is_subset(t))
          interpol |= codomain();

      else
      {
        CtcDeriv ctc_deriv;

        for(int i = 0 ; i < dim() ; i++)
        {
          IntervalVector slice_box(2);
          slice_box[0] = t & domain();
          slice_box[1] = codomain()[i];
          interpol[i] |= (polygon(i, derivative) & slice_box)[1];
        }
      }

      return interpol;
    }

    const Interval TubeSlice::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeSlice derivative(domain(), Interval::ALL_REALS); // todo: optimize this
      return invert(y, derivative, search_domain);
    }

    const Interval TubeSlice::invert(const IntervalVector& y, const TubeSlice& derivative, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);

      if(!m_domain.intersects(search_domain))
        return Interval::EMPTY_SET;

      else if((m_domain & search_domain) == m_domain && m_codomain.is_subset(y))
        return m_domain;

      else if(search_domain == m_domain.lb())
      {
        if(y.intersects(inputGate()))
          return m_domain.lb();
        else
          return Interval::EMPTY_SET;
      }

      else if(search_domain == m_domain.ub())
      {
        if(y.intersects(outputGate()))
          return m_domain.ub();
        else
          return Interval::EMPTY_SET;
      }

      else
      {
        Interval t = Interval::EMPTY_SET;

        for(int i = 0 ; i < dim() ; i++)
        {
          ConvexPolygon p = polygon(i, derivative);
          IntervalVector box(2);
          box[0] = search_domain; box[1] = y[i];
          box = p & box;

          if(box[0].is_empty())
            return Interval::EMPTY_SET;

          t |= box[0];
        }

        return t;
      }
    }

    const pair<IntervalVector,IntervalVector> TubeSlice::eval(const Interval& t) const
    {
      Interval intersection = t & m_domain;
      IntervalVector empty_box(dim(), Interval::EMPTY_SET);

      if(intersection.is_empty())
        return make_pair(empty_box, empty_box);

      else
      {
        IntervalVector lb = empty_box;
        IntervalVector ub = empty_box;

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

        if(!intersection.is_degenerated()
          || (intersection != m_domain.lb() && intersection != m_domain.ub()))
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
      DimensionException::check(*this, x);
      return domain() == x.domain() &&
             codomain() == x.codomain() &&
             inputGate() == x.inputGate() &&
             outputGate() == x.outputGate();
    }
    
    bool TubeSlice::operator!=(const TubeSlice& x) const
    {
      DimensionException::check(*this, x);
      return domain() != x.domain() ||
             codomain() != x.codomain() ||
             inputGate() != x.inputGate() ||
             outputGate() != x.outputGate();
    }

    bool TubeSlice::isSubset(const TubeSlice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return codomain().is_subset(x.codomain())
          && inputGate().is_subset(x.inputGate())
          && outputGate().is_subset(x.outputGate());
    }

    bool TubeSlice::isStrictSubset(const TubeSlice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return isSubset(x) && (*this) != x;
    }
    
    bool TubeSlice::isEmpty() const
    {
      return m_codomain.is_empty() || inputGate().is_empty() || outputGate().is_empty();
    }

    bool TubeSlice::encloses(const TrajectoryVector& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return x[m_domain].is_subset(m_codomain)
          && inputGate().contains(x[m_domain.lb()])
          && outputGate().contains(x[m_domain.ub()]);
    }

    // Setting values

    void TubeSlice::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);
      setEnvelope(y);
      setInputGate(y);
      setOutputGate(y);
    }
    
    void TubeSlice::setEmpty()
    {
      IntervalVector empty_box(dim());
      set(empty_box);
    }

    void TubeSlice::setEnvelope(const IntervalVector& envelope)
    {
      DimensionException::check(*this, envelope);
      m_codomain = envelope;
      *m_input_gate &= m_codomain;
      *m_output_gate &= m_codomain;
    }

    void TubeSlice::setInputGate(const IntervalVector& input_gate)
    {
      DimensionException::check(*this, input_gate);
      *m_input_gate = input_gate;
      *m_input_gate &= m_codomain;

      if(prevSlice() != NULL)
        *m_input_gate &= prevSlice()->codomain();
    }

    void TubeSlice::setOutputGate(const IntervalVector& output_gate)
    {
      DimensionException::check(*this, output_gate);
      *m_output_gate = output_gate;
      *m_output_gate &= m_codomain;

      if(nextSlice() != NULL)
        *m_output_gate &= nextSlice()->codomain();
    }
    
    TubeSlice& TubeSlice::inflate(double rad)
    {
      // todo: simplify with simple addition
      IntervalVector e(dim(), Interval(-rad,rad));
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

    void TubeSlice::setTubeReference(TubeVector *tube_ref)
    {
      m_tube_ref = tube_ref;
    }
    
    void TubeSlice::setDomain(const Interval& domain)
    {
      DomainException::check(domain);
      m_domain = domain;
    }

    // Slices structure

    // Access values

    const IntervalVector TubeSlice::codomainBox() const
    {
      return m_codomain;
    }

    // Setting values

    // Integration

    /*void TubeSlice::checkPartialPrimitive() const
    {
      if(!m_primitive_update_needed)
        throw Exception("TubeSlice::checkPartialPrimitive", "primitive value not set");
      // should be set from TubeVector class
    }

    const pair<IntervalVector,IntervalVector>& TubeSlice::getPartialPrimitiveValue() const
    {
      //checkPartialPrimitive();
      //return m_partial_primitive;
    }

    pair<IntervalVector,IntervalVector> TubeSlice::getPartialPrimitiveValue(const Interval& t) const
    {
      if(t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
      {
        //checkPartialPrimitive();
        //return m_partial_primitive;
      }

      else
      {
        throw Exception("TubeSlice::getPartialPrimitiveValue", "unexpected case");
      }
    }*/
}