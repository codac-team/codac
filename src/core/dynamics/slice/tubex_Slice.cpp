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
#include <iomanip>
#include "tubex_Slice.h"
#include "tubex_CtcDeriv.h"
#include "tubex_arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Slice::Slice(const Interval& domain, const Interval& codomain)
      : m_domain(domain), m_codomain(codomain)
    {
      assert(valid_domain(domain));
      m_input_gate = new Interval(codomain);
      m_output_gate = new Interval(codomain);
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

    int Slice::size() const
    {
      return 1;
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
        if(second_slice->m_input_gate != NULL)
          *first_slice->m_output_gate &= *second_slice->m_input_gate;
        second_slice->m_input_gate = first_slice->m_output_gate;
      }
    }

    const Interval Slice::input_gate() const
    {
      return *m_input_gate;
    }
    
    const Interval Slice::output_gate() const
    {
      return *m_output_gate;
    }
    
    // Accessing values

    const Interval Slice::codomain() const
    {
      return m_codomain;
    }
    
    const IntervalVector Slice::box() const
    {
      IntervalVector box(2);
      box[0] = domain();
      box[1] = m_codomain;
      return box;
    }
    
    double Slice::volume() const
    {
      if(m_codomain.is_empty())
        return 0.;
      
      else if(m_codomain.is_unbounded())
        return POS_INFINITY;
      
      else
        return m_domain.diam() * m_codomain.diam();
    }

    const Interval Slice::operator()(double t) const
    {
      // Write access is not allowed for this operator
      // For write access: use set()
      assert(domain().contains(t));

      if(t == m_domain.lb())
        return input_gate();

      else if(t == m_domain.ub())
        return output_gate();

      return m_codomain;
    }

    const Interval Slice::operator()(const Interval& t) const
    {
      // Write access is not allowed for this operator
      // For write access: use set()
      assert(domain().is_superset(t));
      if(t.is_degenerated())
        return (*this)(t.lb());
      return m_codomain;
    }

    const Interval Slice::interpol(double t, const Slice& v) const
    {
      assert(domain() == v.domain());
      assert(domain().contains(t));
      return interpol(Interval(t), v);
    }

    const Interval Slice::interpol(const Interval& t, const Slice& v) const
    {
      assert(domain() == v.domain());
      assert(domain().is_superset(t));

      if(domain().is_subset(t))
        return codomain();

      else if(t.is_degenerated())
        return (output_gate() - (m_domain.ub() - t) * v.codomain())
             & (input_gate() + (t - m_domain.lb()) * v.codomain());

      else
      {
        IntervalVector input_box = box();
        input_box[0] &= t;
        return (polygon(v) & input_box)[1];
      }
    }

    const Interval Slice::invert(const Interval& y, const Interval& search_domain) const
    {
      Slice v(domain(), Interval::ALL_REALS); // todo: optimize this
      return invert(y, v, search_domain);
    }

    const Interval Slice::invert(const Interval& y, const Slice& v, const Interval& search_domain) const
    {
      assert(domain() == v.domain());
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
        ConvexPolygon p = polygon(v);
        IntervalVector box(2);
        box[0] = search_domain; box[1] = y;
        box = p & box;
        return box[0];
      }
    }

    const pair<Interval,Interval> Slice::eval(const Interval& t) const
    {
      Interval intersection = t & m_domain;
      pair<Interval,Interval> p_eval = make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      if(!intersection.is_empty())
      {
        if(intersection.contains(m_domain.lb()))
        {
          p_eval.first |= input_gate().lb();
          p_eval.second |= input_gate().ub();
        }

        if(intersection.contains(m_domain.ub()))
        {
          p_eval.first |= output_gate().lb();
          p_eval.second |= output_gate().ub();
        }

        if(!intersection.is_degenerated()
          || (intersection != m_domain.lb() && intersection != m_domain.ub()))
        {
          p_eval.first |= m_codomain.lb();
          p_eval.second |= m_codomain.ub();
        }
      }
      
      return p_eval;
    }

    // Tests
    
    bool Slice::operator==(const Slice& x) const
    {
      return domain() == x.domain() &&
             codomain() == x.codomain() &&
             input_gate() == x.input_gate() &&
             output_gate() == x.output_gate();
    }
    
    bool Slice::operator!=(const Slice& x) const
    {
      return domain() != x.domain() ||
             codomain() != x.codomain() ||
             input_gate() != x.input_gate() ||
             output_gate() != x.output_gate();
    }

    #define macro_sets_comparison(f) \
      \
      assert(domain() == x.domain()); \
      return codomain().f(x.codomain()) \
          && input_gate().f(x.input_gate()) \
          && output_gate().f(x.output_gate()); \

    bool Slice::is_subset(const Slice& x) const
    {
      macro_sets_comparison(is_subset);
    }

    bool Slice::is_strict_subset(const Slice& x) const
    {
      return is_subset(x) && *this != x;
    }

    bool Slice::is_interior_subset(const Slice& x) const
    {
      macro_sets_comparison(is_interior_subset);
    }

    bool Slice::is_strict_interior_subset(const Slice& x) const
    {
      return is_interior_subset(x) && *this != x;
    }

    bool Slice::is_superset(const Slice& x) const
    {
      macro_sets_comparison(is_superset);
    }

    bool Slice::is_strict_superset(const Slice& x) const
    {
      return is_superset(x) && *this != x;
    }
    
    bool Slice::is_empty() const
    {
      return m_codomain.is_empty() || input_gate().is_empty() || output_gate().is_empty();
    }

    bool Slice::contains(const Trajectory& x) const
    {
      assert(domain().is_subset(x.domain()));
      return x(m_domain).is_subset(m_codomain)
          // the gates should contain double values, but we use x(Interval(double)) for reliable evaluation
          && x(Interval(m_domain.lb())).is_subset(input_gate())
          && x(Interval(m_domain.ub())).is_subset(output_gate());
    }

    // Setting values

    void Slice::set(const Interval& y)
    {
      m_codomain = y;

      *m_input_gate = y;
      if(prev_slice() != NULL)
        *m_input_gate &= prev_slice()->codomain();

      *m_output_gate = y;
      if(next_slice() != NULL)
        *m_output_gate &= next_slice()->codomain();
    }
    
    void Slice::set_empty()
    {
      set(Interval::EMPTY_SET);
    }

    void Slice::set_envelope(const Interval& envelope)
    {
      m_codomain = envelope;
      *m_input_gate &= m_codomain;
      *m_output_gate &= m_codomain;
    }

    void Slice::set_input_gate(const Interval& input_gate)
    {
      *m_input_gate = input_gate;
      *m_input_gate &= m_codomain;

      if(prev_slice() != NULL)
        *m_input_gate &= prev_slice()->codomain();
    }

    void Slice::set_output_gate(const Interval& output_gate)
    {
      *m_output_gate = output_gate;
      *m_output_gate &= m_codomain;

      if(next_slice() != NULL)
        *m_output_gate &= next_slice()->codomain();
    }
    
    const Slice& Slice::inflate(double rad)
    {
      assert(rad >= 0.);
      Interval e(-rad,rad);
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
    
    void Slice::set_domain(const Interval& domain)
    {
      assert(valid_domain(domain));
      m_domain = domain;
    }

    // Slices structure

    // Access values

    const IntervalVector Slice::codomain_box() const
    {
      return IntervalVector(m_codomain);
    }

    // Setting values
    
}