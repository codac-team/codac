/* ============================================================================
 *  tubex-lib - Slice class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Slice.h"
#include "tubex_DomainException.h"
#include "tubex_SlicingException.h"
#include "tubex_EmptyException.h"
#include "tubex_DimensionException.h"
#include "tubex_CtcDeriv.h"
#include "tubex_Arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Slice::Slice(const Interval& domain, int dim)
      : Slice(domain, IntervalVector(dim))
    {
      DomainException::check(domain);
      DimensionException::check(dim);
    }

    Slice::Slice(const Interval& domain, const IntervalVector& codomain)
      : m_domain(domain), m_codomain(codomain)
    {
      DomainException::check(domain);
      DimensionException::check(codomain.size());
      
      m_input_gate = new IntervalVector(codomain);
      m_output_gate = new IntervalVector(codomain);
    }

    Slice::Slice(const Slice& x)
      : Slice(x.domain(), x.codomain()) // in order to instantiate gates
    {
      *this = x;
    }

    Slice::~Slice()
    {
      // Links to other slices are destroyed

      if(m_prev_slice != NULL) m_prev_slice->m_next_slice = NULL;
      if(m_next_slice != NULL) m_next_slice->m_prev_slice = NULL;

      // Gates are deleted if not shared with other slices

      if(m_prev_slice == NULL) delete m_input_gate;
      if(m_next_slice == NULL) delete m_output_gate;
    }

    const Slice& Slice::operator=(const Slice& x)
    {
      m_domain = x.m_domain;
      m_codomain = x.m_codomain;
      *m_input_gate = *x.m_input_gate;
      *m_output_gate = *x.m_output_gate;
      return *this;
    }
    
    const Interval Slice::domain() const
    {
      return m_domain;
    }

    int Slice::dim() const
    {
      return m_codomain.size();
    }

    // Slices structure

    Slice* Slice::prev_slice()
    {
      return const_cast<Slice*>(static_cast<const Slice&>(*this).prev_slice());
    }

    const Slice* Slice::prev_slice() const
    {
      return m_prev_slice;
    }

    Slice* Slice::next_slice()
    {
      return const_cast<Slice*>(static_cast<const Slice&>(*this).next_slice());
    }

    const Slice* Slice::next_slice() const
    {
      return m_next_slice;
    }

    void Slice::chain_slices(Slice *first_slice, Slice *second_slice)
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

    const IntervalVector Slice::input_gate() const
    {
      return *m_input_gate;
    }
    
    const IntervalVector Slice::output_gate() const
    {
      return *m_output_gate;
    }

    const TubeVector* Slice::tube_reference() const
    {
      return m_tube_ref;
    }
    
    // Access values

    const IntervalVector Slice::codomain() const
    {
      return codomain_box();
    }
    
    const IntervalVector Slice::box() const
    {
      IntervalVector box(m_codomain.size() + 1);
      box[0] = domain();
      for(int i = 0 ; i < m_codomain.size() ; i++)
        box[i + 1] = m_codomain[i];
      return box;
    }
    
    double Slice::volume() const
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

    const IntervalVector Slice::operator()(double t) const
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

    const IntervalVector Slice::operator()(const Interval& t) const
    {
      // Write access is not allowed for this operator
      // For write access: use set()
      DomainException::check(*this, t);
      if(t.is_degenerated())
        return (*this)(t.lb());
      return m_codomain;
    }

    const IntervalVector Slice::interpol(double t, const Slice& v) const
    {
      DomainException::check(*this, v);
      DimensionException::check(*this, v);
      return interpol(Interval(t), v);
    }

    const IntervalVector Slice::interpol(const Interval& t, const Slice& v) const
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

    const Interval Slice::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      Slice v(domain(), Interval::ALL_REALS); // todo: optimize this
      return invert(y, v, search_domain);
    }

    const Interval Slice::invert(const IntervalVector& y, const Slice& v, const Interval& search_domain) const
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

    const pair<IntervalVector,IntervalVector> Slice::eval(const Interval& t) const
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
    
    bool Slice::operator==(const Slice& x) const
    {
      DimensionException::check(*this, x);
      return domain() == x.domain() &&
             codomain() == x.codomain() &&
             input_gate() == x.input_gate() &&
             output_gate() == x.output_gate();
    }
    
    bool Slice::operator!=(const Slice& x) const
    {
      DimensionException::check(*this, x);
      return domain() != x.domain() ||
             codomain() != x.codomain() ||
             input_gate() != x.input_gate() ||
             output_gate() != x.output_gate();
    }

    bool Slice::is_subset(const Slice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return codomain().is_subset(x.codomain())
          && input_gate().is_subset(x.input_gate())
          && output_gate().is_subset(x.output_gate());
    }

    bool Slice::is_strict_subset(const Slice& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return is_subset(x) && (*this) != x;
    }
    
    bool Slice::is_empty() const
    {
      return m_codomain.is_empty() || input_gate().is_empty() || output_gate().is_empty();
    }

    bool Slice::encloses(const TrajectoryVector& x) const
    {
      DomainException::check(*this, x);
      DimensionException::check(*this, x);
      return x(m_domain).is_subset(m_codomain)
          && input_gate().contains(x(m_domain.lb()))
          && output_gate().contains(x(m_domain.ub()));
    }

    // Setting values

    void Slice::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);
      m_codomain = y;

      *m_input_gate = y;
      if(prev_slice() != NULL) *m_input_gate &= prev_slice()->codomain();

      *m_output_gate = y;
      if(next_slice() != NULL) *m_output_gate &= next_slice()->codomain();
    }

    void Slice::set_all_reals()
    {
      set_all_reals(0, dim() - 1);
    }

    void Slice::set_all_reals(int start_index, int end_index)
    {
      // todo: check index
      IntervalVector all_reals(end_index - start_index + 1);

      ibex_overloaded_put(m_codomain, start_index, all_reals);

      ibex_overloaded_put(*m_input_gate, start_index, all_reals);
      if(prev_slice() != NULL) *m_input_gate &= prev_slice()->codomain();

      ibex_overloaded_put(*m_output_gate, start_index, all_reals);
      if(next_slice() != NULL) *m_output_gate &= next_slice()->codomain();
    }
    
    void Slice::set_empty()
    {
      IntervalVector empty_box(dim());
      set(empty_box);
    }

    void Slice::set_envelope(const IntervalVector& envelope)
    {
      DimensionException::check(*this, envelope);
      m_codomain = envelope;
      *m_input_gate &= m_codomain;
      *m_output_gate &= m_codomain;
    }

    void Slice::set_input_gate(const IntervalVector& input_gate)
    {
      DimensionException::check(*this, input_gate);
      *m_input_gate = input_gate;
      *m_input_gate &= m_codomain;

      if(prev_slice() != NULL)
        *m_input_gate &= prev_slice()->codomain();
    }

    void Slice::set_output_gate(const IntervalVector& output_gate)
    {
      DimensionException::check(*this, output_gate);
      *m_output_gate = output_gate;
      *m_output_gate &= m_codomain;

      if(next_slice() != NULL)
        *m_output_gate &= next_slice()->codomain();
    }
    
    const Slice& Slice::inflate(double rad)
    {
      IntervalVector e(dim(), Interval(-rad,rad));
      set_envelope(m_codomain + e);
      set_input_gate(*m_input_gate + e);
      set_output_gate(*m_output_gate + e);
    }
    
    // String

    ostream& operator<<(ostream& str, const Slice& x)
    {
      str << "Slice " << x.domain() << "↦(" << x.input_gate() << ")" << x.codomain() << "(" << x.output_gate() << ")" << flush;
      return str;
    }


  // Protected methods

    void Slice::resize(int n)
    {
      m_codomain.resize(n);
      m_input_gate->resize(n);
      m_output_gate->resize(n);
    }

    void Slice::set_tube_ref(TubeVector *tube_ref)
    {
      m_tube_ref = tube_ref;
    }
    
    void Slice::set_domain(const Interval& domain)
    {
      DomainException::check(domain);
      m_domain = domain;
    }

    // Slices structure

    // Access values

    const IntervalVector Slice::codomain_box() const
    {
      return m_codomain;
    }

    // Setting values
    
}