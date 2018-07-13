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
#include "tubex_SamplingException.h"
#include "tubex_EmptyException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_ConvexPolygon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv()
  {

  }

  bool CtcDeriv::contract(TubeVector& x, const TubeVector& v) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    SamplingException::check(x, v);

    bool ctc = false;
    ctc |= contract_fwd(x, v);
    ctc |= contract_bwd(x, v);
    // todo: optimized propagation, according to gates' updates
    return ctc;
  }

  bool CtcDeriv::contract_fwd(TubeVector& x, const TubeVector& v) const // temporal forward
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    SamplingException::check(x, v);

    bool ctc = false;
    TubeSlice *x_slice = x.get_first_slice();
    const TubeSlice *v_slice = v.get_first_slice();

    while(x_slice != NULL)
    {
      ctc |= contract(*x_slice, *v_slice);
      x_slice = x_slice->next_slice();
      v_slice = v_slice->next_slice();
    }

    return ctc;
  }

  bool CtcDeriv::contract_bwd(TubeVector& x, const TubeVector& v) const // temporal backward
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    SamplingException::check(x, v);
    
    bool ctc = false;
    TubeSlice *x_slice = x.get_last_slice();
    const TubeSlice *v_slice = v.get_last_slice();
    
    while(x_slice != NULL)
    {
      ctc |= contract(*x_slice, *v_slice);
      x_slice = x_slice->prev_slice();
      v_slice = v_slice->prev_slice();
    }

    return ctc;
  }

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);

    bool ctc = false;

    ctc |= contract_gates(x, v);

    for(int i = 0 ; i < x.dim() ; i++)
      ctc |= contract_codomain(i, x, v);

    return ctc;
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

  bool CtcDeriv::contract_codomain(int i, TubeSlice& x, const TubeSlice& v) const
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    
    // todo: remove this:
    x.set_envelope(x.codomain() & IntervalVector(x.dim(), Interval(-99999.,99999.)));
    
    IntervalVector box = x.codomain(), prev_box = box;
    box[i] = x.polygon(i, v).box()[1];
    x.set_envelope(box);
    return prev_box != box;
  }
}