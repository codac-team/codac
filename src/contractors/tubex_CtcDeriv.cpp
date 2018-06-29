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
#include "tubex_StructureException.h"
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

  bool CtcDeriv::contract(TubeVector& x, const TubeVector& v)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    StructureException::check(x, v);

    bool ctc = false;
    ctc |= contractFwd(x, v);
    ctc |= contractBwd(x, v);
    // todo: optimized propagation, according to gates' updates
    return ctc;
  }

  bool CtcDeriv::contractFwd(TubeVector& x, const TubeVector& v) // temporal forward
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    StructureException::check(x, v);

    bool ctc = false;
    TubeSlice *x_slice = x.getFirstSlice();
    TubeSlice *v_slice = v.getFirstSlice();

    while(x_slice != NULL)
    {
      ctc |= contract(*x_slice, *v_slice);
      x_slice = x_slice->nextSlice();
      v_slice = v_slice->nextSlice();
    }

    return ctc;
  }

  bool CtcDeriv::contractBwd(TubeVector& x, const TubeVector& v) // temporal backward
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    StructureException::check(x, v);
    
    bool ctc = false;
    TubeSlice *x_slice = x.getLastSlice();
    TubeSlice *v_slice = v.getLastSlice();
    
    while(x_slice != NULL)
    {
      ctc |= contract(*x_slice, *v_slice);
      x_slice = x_slice->prevSlice();
      v_slice = v_slice->prevSlice();
    }

    return ctc;
  }

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);

    bool ctc = false;

    ctc |= contractGates(x, v);

    for(int i = 0 ; i < x.dim() ; i++)
      ctc |= contractCodomain(i, x, v);

    return ctc;
  }

  bool CtcDeriv::contractGates(TubeSlice& x, const TubeSlice& v)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    
    bool ctc = false;
    IntervalVector in_gate = x.inputGate(), out_gate = x.outputGate();

    IntervalVector in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    ctc |= out_gate != x.outputGate();
    x.setOutputGate(out_gate);

    IntervalVector out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    ctc |= in_gate != x.inputGate();
    x.setInputGate(in_gate);

    return ctc;
  }

  bool CtcDeriv::contractCodomain(int i, TubeSlice& x, const TubeSlice& v)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    DimensionException::check(x, i);

    IntervalVector box = x.codomain(), prev_box = box;
    box[i] = x.polygon(i, v).box()[1];
    x.setEnvelope(box);
    return prev_box != box;
  }
}