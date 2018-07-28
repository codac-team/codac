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

  bool CtcDeriv::contract(TubeVector& x, const TubeVector& v, TPropagation t_propa) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    SlicingException::check(x, v);

    bool ctc = false;
    
    if(t_propa & FORWARD)
    {
      TubeSlice *x_slice = x.get_first_slice();
      const TubeSlice *v_slice = v.get_first_slice();

      while(x_slice != NULL)
      {
        ctc |= contract(*x_slice, *v_slice);
        x_slice = x_slice->next_slice();
        v_slice = v_slice->next_slice();
      }
    }
    
    if(t_propa & BACKWARD)
    {
      TubeSlice *x_slice = x.get_last_slice();
      const TubeSlice *v_slice = v.get_last_slice();

      while(x_slice != NULL)
      {
        ctc |= contract(*x_slice, *v_slice);
        x_slice = x_slice->prev_slice();
        v_slice = v_slice->prev_slice();
      }
    }

    return ctc;
  }

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);

    const TubeSlice old_x = x;
    IntervalVector box = x.codomain(), ingate = x.input_gate(), outgate = x.output_gate();

    // todo: remove this:
      box &= IntervalVector(x.dim(), Interval(-99999.,99999.));
      x.set_envelope(box);

    for(int i = 0 ; i < x.dim() ; i++)
    {
      if(outgate[i] == Interval::ALL_REALS)
      {
        // Faster evaluation without polygons
        box[i] &= ingate[i] + Interval(0.,x.domain().diam()) * v.codomain()[i];
        outgate[i] &= ingate[i] + x.domain().diam() * v.codomain()[i];
      }

      else if(ingate[i] == Interval::ALL_REALS)
      {
        // Faster evaluation without polygons
        box[i] &= outgate[i] - Interval(0.,x.domain().diam()) * v.codomain()[i];
        ingate[i] &= outgate[i] - x.domain().diam() * v.codomain()[i];
      }

      else
      {
        // Gates contraction
        outgate[i] &= ingate[i] + x.domain().diam() * v.codomain()[i];
        ingate[i] &= outgate[i] - x.domain().diam() * v.codomain()[i];
        x.set_input_gate(ingate);
        x.set_output_gate(outgate);

        // Optimal envelope
        box[i] &= x.polygon(i, v).box()[1];
      }
    }

    x.set_envelope(box);
    x.set_input_gate(ingate);
    x.set_output_gate(outgate);
    return old_x != x;
  }

  bool CtcDeriv::contract_gates(TubeSlice& x, const TubeSlice& v) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    
    bool ctc = false;
    IntervalVector in_gate = x.input_gate(), out_gate = x.output_gate();

    IntervalVector in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    ctc |= out_gate != x.output_gate();
    x.set_output_gate(out_gate);

    IntervalVector out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    ctc |= in_gate != x.input_gate();
    x.set_input_gate(in_gate);

    return ctc;
  }
}