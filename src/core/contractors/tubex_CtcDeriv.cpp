/* ============================================================================
 *  tubex-lib - CtcDeriv class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcDeriv.h"
#include "tubex_SlicingException.h"
#include "tubex_EmptyException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_ConvexPolygon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv(bool preserve_slicing)
    : Ctc(preserve_slicing)
  {

  }

  bool CtcDeriv::contract(Tube& x, const Tube& v, TPropagation t_propa) const
  {
    DomainException::check(x, v);
    SlicingException::check(x, v);

    bool ctc = false;
    
    if(t_propa & FORWARD)
    {
      Slice *x_slice = x.get_first_slice();
      const Slice *v_slice = v.get_first_slice();

      while(x_slice != NULL)
      {
        ctc |= contract(*x_slice, *v_slice);
        x_slice = x_slice->next_slice();
        v_slice = v_slice->next_slice();
      }
    }
    
    if(t_propa & BACKWARD)
    {
      Slice *x_slice = x.get_last_slice();
      const Slice *v_slice = v.get_last_slice();

      while(x_slice != NULL)
      {
        ctc |= contract(*x_slice, *v_slice);
        x_slice = x_slice->prev_slice();
        v_slice = v_slice->prev_slice();
      }
    }

    return ctc;
  }

  bool CtcDeriv::contract(Slice& x, const Slice& v) const
  {
    DomainException::check(x, v);

    const Slice old_x = x;
    Interval envelope = x.codomain(), ingate = x.input_gate(), outgate = x.output_gate();

    // todo: remove this:
      envelope &= Interval(-99999.,99999.);
      x.set_envelope(envelope);

    if(outgate == Interval::ALL_REALS)
    {
      // Faster evaluation without polygons
      envelope &= ingate + Interval(0.,x.domain().diam()) * v.codomain();
      outgate &= ingate + x.domain().diam() * v.codomain();
    }

    else if(ingate == Interval::ALL_REALS)
    {
      // Faster evaluation without polygons
      envelope &= outgate - Interval(0.,x.domain().diam()) * v.codomain();
      ingate &= outgate - x.domain().diam() * v.codomain();
    }

    else
    {
      // Gates contraction
      outgate &= ingate + x.domain().diam() * v.codomain();
      ingate &= outgate - x.domain().diam() * v.codomain();
      x.set_input_gate(ingate);
      x.set_output_gate(outgate);

      // Optimal envelope
      envelope &= x.polygon(v).box()[1];
    }

    x.set_envelope(envelope);
    x.set_input_gate(ingate);
    x.set_output_gate(outgate);
    return old_x != x;
  }

  bool CtcDeriv::contract_gates(Slice& x, const Slice& v) const
  {
    DomainException::check(x, v);
    
    bool ctc = false;
    Interval in_gate = x.input_gate(), out_gate = x.output_gate();

    Interval in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    ctc |= out_gate != x.output_gate();
    x.set_output_gate(out_gate);

    Interval out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    ctc |= in_gate != x.input_gate();
    x.set_input_gate(in_gate);

    return ctc;
  }

  bool CtcDeriv::contract(TubeVector& x, const TubeVector& v, TPropagation t_propa) const
  {
    DimensionException::check(x, v);
    bool result = false;
    for(int i = 0 ; i < x.size() ; i++)
      result |= contract(x[i], v[i], t_propa);
    return result;
  }
}