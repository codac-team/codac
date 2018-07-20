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
#include "tubex_SlicingException.h"
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
      : TubeSlice(x.domain(), x.codomain()) // in order to instantiate gates
    {
      *this = x;
    }

    TubeSlice::~TubeSlice()
    {
      // Links to other slices are destroyed

      if(m_prev_slice != NULL) m_prev_slice->m_next_slice = NULL;
      if(m_next_slice != NULL) m_next_slice->m_prev_slice = NULL;

      // Gates are deleted if not shared with other slices

      if(m_prev_slice == NULL) delete m_input_gate;
      if(m_next_slice == NULL) delete m_output_gate;
    }

    const TubeSlice& TubeSlice::operator=(const TubeSlice& x)
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

    TubeSlice* TubeSlice::prev_slice()
    {
      return const_cast<TubeSlice*>(static_cast<const TubeSlice&>(*this).prev_slice());
    }

    const TubeSlice* TubeSlice::prev_slice() const
    {
      return m_prev_slice;
    }

    TubeSlice* TubeSlice::next_slice()
    {
      return const_cast<TubeSlice*>(static_cast<const TubeSlice&>(*this).next_slice());
    }

    const TubeSlice* TubeSlice::next_slice() const
    {
      return m_next_slice;
    }

    void TubeSlice::chain_slices(TubeSlice *first_slice, TubeSlice *second_slice)
    {
      if(first_slice != NULL)
        first_slice->m_next_slice = second_slice;

      if(second_slice != NULL)
        second_slice->m_prev_slice = first_slice;

      if(first_slice != NULL && second_slice != NULL)
      {
        DimensionException::check(*first_slice, *second_slice);

        if(second_slice->m_input_gate != NULL)
          *first_slice->m_output_gate &= *second_slice->m_input_gate;
        second_slice->m_input_gate = first_slice->m_output_gate;
      }
    }

    const IntervalVector TubeSlice::input_gate() const
    {
      return *m_input_gate;
    }
    
    const IntervalVector TubeSlice::output_gate() const
    {
      return *m_output_gate;
    }

    const TubeVector* TubeSlice::tube_reference() const
    {
      return m_tube_ref;
    }
    
    // Access values

    const IntervalVector TubeSlice::codomain() const
    {
      return codomain_box();
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
      // Write access is not allowed for this operator
      // For write access: use set()
      DomainException::check(*this, t);

      if(t == m_domain.lb())
        return input_gate();

      else if(t == m_domain.ub())
        return output_gate();

      return m_codomain;
    }

    const IntervalVector TubeSlice::operator[](const Interval& t) const
    {
      // Write access is not allowed for this operator
      // For write access: use set()
      DomainException::check(*this, t);
      if(t.is_degenerated())
        return (*this)[t.lb()];
      return m_codomain;
    }

    const IntervalVector TubeSlice::interpol(double t, const TubeSlice& v) const
    {
      DomainException::check(*this, v);
      DimensionException::check(*this, v);
      return interpol(Interval(t), v);
    }

    const IntervalVector TubeSlice::interpol(const Interval& t, const TubeSlice& v) const
    {
      DomainException::check(*this, t);
      DomainException::check(*this, v);
      DimensionException::check(*this, v);

      if(domain().is_subset(t))
        return codomain();

      else if(t.is_degenerated())
        return (output_gate() - (m_domain.ub() - t) * v.codomain())
             & (input_gate() + (t - m_domain.lb()) * v.codomain());

      else
      {
        CtcDeriv ctc_deriv;
        IntervalVector interpol(dim(), Interval::EMPTY_SET);

        for(int i = 0 ; i < dim() ; i++)
        {
          IntervalVector slice_box(2);
          slice_box[0] = t & domain();
          slice_box[1] = codomain()[i];
          interpol[i] |= (polygon(i, v) & slice_box)[1];
        }

        return interpol;
      }
    }

    const Interval TubeSlice::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeSlice v(domain(), Interval::ALL_REALS); // todo: optimize this
      return invert(y, v, search_domain);
    }

    const Interval TubeSlice::invert(const IntervalVector& y, const TubeSlice& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);

      // todo: use enclosed bounds also? in order to speed up computations

      if(!m_domain.intersects(search_domain))
        return Interval::EMPTY_SET;

      else if((m_domain & search_domain) == m_domain && m_codomain.is_subset(y))
        return m_domain;

      else if(search_domain == m_domain.lb())
      {
        if(y.intersects(input_gate()))
          return m_domain.lb();
        else
          return Interval::EMPTY_SET;
      }

      else if(search_domain == m_domain.ub())
      {
        if(y.intersects(output_gate()))
          return m_domain.ub();
        else
          return Interval::EMPTY_SET;
      }

      else
      {
        Interval t = Interval::EMPTY_SET;

        for(int i = 0 ; i < dim() ; i++)
        {
          ConvexPolygon p = polygon(i, v);
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
          lb |= input_gate().lb();
          ub |= input_gate().ub();
        }

        if(intersection.contains(m_domain.ub()))
        {
          lb |= output_gate().lb();
          ub |= output_gate().ub();
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
             input_gate() == x.input_gate() &&
             output_gate() == x.output_gate();
    }
    
    bool TubeSlice::operator!=(const TubeSlice& x) const
    {
      DimensionException::check(*this, x);
      return domain() != x.domain() ||
             codomain() != x.codomain() ||
             input_gate() != x.input_gate() ||
             output_gate() != x.output_gate();
    }

    bool TubeSlice::is_subset(const TubeSlice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return codomain().is_subset(x.codomain())
          && input_gate().is_subset(x.input_gate())
          && output_gate().is_subset(x.output_gate());
    }

    bool TubeSlice::is_strict_subset(const TubeSlice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return is_subset(x) && (*this) != x;
    }
    
    bool TubeSlice::is_empty() const
    {
      return m_codomain.is_empty() || input_gate().is_empty() || output_gate().is_empty();
    }

    bool TubeSlice::encloses(const TrajectoryVector& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return x[m_domain].is_subset(m_codomain)
          && input_gate().contains(x[m_domain.lb()])
          && output_gate().contains(x[m_domain.ub()]);
    }

    // Setting values

    void TubeSlice::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);
      set_envelope(y);
      set_input_gate(y);
      set_output_gate(y);
    }
    
    void TubeSlice::set_empty()
    {
      IntervalVector empty_box(dim());
      set(empty_box);
    }

    void TubeSlice::set_envelope(const IntervalVector& envelope)
    {
      DimensionException::check(*this, envelope);
      m_codomain = envelope;
      *m_input_gate &= m_codomain;
      *m_output_gate &= m_codomain;
    }

    void TubeSlice::set_input_gate(const IntervalVector& input_gate)
    {
      DimensionException::check(*this, input_gate);
      *m_input_gate = input_gate;
      *m_input_gate &= m_codomain;

      if(prev_slice() != NULL)
        *m_input_gate &= prev_slice()->codomain();
    }

    void TubeSlice::set_output_gate(const IntervalVector& output_gate)
    {
      DimensionException::check(*this, output_gate);
      *m_output_gate = output_gate;
      *m_output_gate &= m_codomain;

      if(next_slice() != NULL)
        *m_output_gate &= next_slice()->codomain();
    }
    
    const TubeSlice& TubeSlice::inflate(double rad)
    {
      IntervalVector e(dim(), Interval(-rad,rad));
      set_envelope(m_codomain + e);
      set_input_gate(*m_input_gate + e);
      set_output_gate(*m_output_gate + e);
    }
    
    // String

    ostream& operator<<(ostream& str, const TubeSlice& x)
    {
      str << "Slice " << x.domain() << "↦(" << x.input_gate() << ")" << x.codomain() << "(" << x.output_gate() << ")" << flush;
      return str;
    }


  // Protected methods

    void TubeSlice::set_tube_ref(TubeVector *tube_ref)
    {
      m_tube_ref = tube_ref;
    }
    
    void TubeSlice::set_domain(const Interval& domain)
    {
      DomainException::check(domain);
      m_domain = domain;
    }

    // Slices structure

    // Access values

    const IntervalVector TubeSlice::codomain_box() const
    {
      return m_codomain;
    }

    // Setting values
    
}