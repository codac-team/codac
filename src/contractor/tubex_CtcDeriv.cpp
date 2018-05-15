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

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv()
  {

  }

  Interval yilb(const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    return x.inputGate().lb() + v.codomain().lb() * (t - x.domain().lb());
  }

  Interval yilb_inv(const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    return ((y - x.inputGate().lb()) / v.codomain().lb()) + x.domain().lb();
  }

  Interval yiub(const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    return x.inputGate().ub() + v.codomain().ub() * (t - x.domain().lb());
  }

  Interval yiub_inv(const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    return ((y - x.inputGate().ub()) / v.codomain().ub()) + x.domain().lb();
  }

  Interval yolb(const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    return x.outputGate().lb() + v.codomain().ub() * (t - x.domain().ub());
  }

  Interval yolb_inv(const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    return ((y - x.outputGate().lb()) / v.codomain().ub()) + x.domain().ub();
  }

  Interval youb(const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    return x.outputGate().ub() + v.codomain().lb() * (t - x.domain().ub());
  }

  Interval youb_inv(const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    return ((y - x.outputGate().ub()) / v.codomain().lb()) + x.domain().ub();
  }

  typedef Interval (* vFunctionCall)(const Interval& t, const TubeSlice& x, const TubeSlice& v);

  Interval linesIntersectionUb(const TubeSlice& x, const TubeSlice& v)
  {
    return (x.outputGate().ub() - x.inputGate().ub()
            + v.codomain().ub()*x.domain().lb()
            - v.codomain().lb()*x.domain().ub()) / v.codomain().diam();
  }

  Interval linesIntersectionLb(const TubeSlice& x, const TubeSlice& v)
  {
    return (x.inputGate().lb() - x.outputGate().lb()
            + v.codomain().ub()*x.domain().ub()
            - v.codomain().lb()*x.domain().lb()) / v.codomain().diam();
  }

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v)
  {
    EmptyException::check(v);

    bool ctc = false;
    Interval t = x.domain(), y = x.codomain();
    Interval in_gate = x.inputGate(), out_gate = x.outputGate();

    // todo : check empty cases

    // 1. Propagations from gate to gate

    Interval in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    ctc |= out_gate != x.outputGate();
    x.setOutputGate(out_gate);

    Interval out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    ctc |= in_gate != x.inputGate();
    x.setInputGate(in_gate);

    // 2. Envelope

    Interval envelope;
    
    if(v.codomain().is_degenerated())
      envelope = in_gate | out_gate;

    else
    {
      envelope.set_empty();

      Interval t_inter_ub = linesIntersectionUb(x, v);

        if(t_inter_ub.intersects(t))
          envelope |= youb(t_inter_ub,x,v);

        if(t_inter_ub.ub() < t.lb())
          envelope |= youb(t,x,v);

        if(t_inter_ub.lb() > t.ub())
          envelope |= yiub(t,x,v);

      Interval t_inter_lb = linesIntersectionLb(x, v);

        if(t_inter_lb.intersects(t))
          envelope |= yolb(t_inter_lb,x,v);

        if(t_inter_lb.ub() < t.lb())
          envelope |= yolb(t,x,v);

        if(t_inter_lb.lb() > t.ub())
          envelope |= yilb(t,x,v);
    }

    if(y.is_strict_superset(envelope))
    {
      ctc = true;
      x.set(envelope);
    }

    return ctc;
  }

  bool CtcDeriv::contract(const TubeSlice& x, const TubeSlice& v, double t, Interval& y)
  {
    // todo: optimized function for double t
    return contract(x, v, Interval(t), y);
  }

  bool CtcDeriv::contract(const TubeSlice& x, const TubeSlice& v, const Interval& t, Interval& y)
  {

  }

  bool CtcDeriv::contract(Tube& x, const Tube& v)
  {
    bool contraction = false;
    contraction |= contractFwd(x, v);
    contraction |= contractBwd(x, v);
    // todo: optimized propagation, according to gates' updates
    return contraction;
  }

  bool CtcDeriv::contractFwd(Tube& x, const Tube& v) // temporal forward
  {
    StructureException::check(x, v);

    bool contraction = false;
    TubeSlice *x_slice = x.getFirstSlice();
    TubeSlice *v_slice = v.getFirstSlice();
    
    while(x_slice != NULL)
    {
      contraction |= contract(*x_slice, *v_slice);
      x_slice = x_slice->nextSlice();
      v_slice = v_slice->nextSlice();
    }

    return contraction;
  }

  bool CtcDeriv::contractBwd(Tube& x, const Tube& v) // temporal backward
  {
    StructureException::check(x, v);

    bool contraction = false;
    TubeSlice *x_slice = x.getLastSlice();
    TubeSlice *v_slice = v.getLastSlice();
    
    while(x_slice != NULL)
    {
      contraction |= contract(*x_slice, *v_slice);
      x_slice = x_slice->prevSlice();
      v_slice = v_slice->prevSlice();
    }

    return contraction;
  }
}