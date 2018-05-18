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

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv()
  {

  }

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v)
  {
    StructureException::check(x, v);

    bool ctc = false;
    const Interval t = x.domain();
    Interval y;

    ctc |= contractGates(x, v);
    ctc |= contractEnvelope(x, v, t, y);
    x.setEnvelope(y);

    return ctc;
  }

  bool CtcDeriv::contract(const TubeSlice& x, const TubeSlice& v, const Interval& t, Interval& y)
  {
    StructureException::check(x, v);

    bool ctc = false;
    TubeSlice x_temp = x;

    ctc |= contractGates(x_temp, v);
    ctc |= contractEnvelope(x_temp, v, t, y);

    return ctc;
  }

  bool CtcDeriv::contract(Tube& x, const Tube& v)
  {
    bool ctc = false;
    ctc |= contractFwd(x, v);
    ctc |= contractBwd(x, v);
    // todo: optimized propagation, according to gates' updates
    return ctc;
  }

  bool CtcDeriv::contractFwd(Tube& x, const Tube& v) // temporal forward
  {
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

  bool CtcDeriv::contractBwd(Tube& x, const Tube& v) // temporal backward
  {
    bool ctc = false;
    TubeSlice *x_slice = x.getLastSlice();
    TubeSlice *v_slice = v.getLastSlice();
    
    while(x_slice != NULL)
    {
      ctc |= contract(*x_slice, *v_slice);
      x_slice = x_slice->prevSlice();
      v_slice = v_slice->prevSlice();
    }

    x.flagFutureDataUpdateFromRoot(); // todo: remove this
    return ctc;
  }

  bool CtcDeriv::contract(const Tube& x, const Tube& v, const Interval& t, Interval& y)
  {
    DomainException::check(x, t);

    TubeSlice *x_slice = (TubeSlice*)x.getSlice(t.lb());
    TubeSlice *v_slice = (TubeSlice*)v.getSlice(t.lb());

    Interval envelope = Interval::EMPTY_SET;
    
    while(x_slice != NULL && x_slice->domain().intersects(t))
    {
      if(x_slice->domain().is_subset(t))
        envelope |= x_slice->codomain();

      else
      {
        Interval y_slice;
        contract(*x_slice, *v_slice, t & x_slice->domain(), y_slice);
        envelope |= y_slice;
      }

      x_slice = x_slice->nextSlice();
      v_slice = v_slice->nextSlice();
    }

    Interval prev_y = y;
    y &= envelope;
    return prev_y != y;
  }

  bool CtcDeriv::contractGates(TubeSlice& x, const TubeSlice& v)
  {
    bool ctc = false;
    Interval in_gate = x.inputGate(), out_gate = x.outputGate();

    // Propagations from gate to gate

    Interval in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    ctc |= out_gate != x.outputGate();
    x.setOutputGate(out_gate);

    Interval out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    ctc |= in_gate != x.inputGate();
    x.setInputGate(in_gate);

    return ctc;
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

  bool CtcDeriv::contractEnvelope(const TubeSlice& x, const TubeSlice& v, const Interval& t, Interval& y)
  {
    DomainException::check(x, t);
    Interval envelope;

    if(x.inputGate() == Interval::ALL_REALS || x.outputGate() == Interval::ALL_REALS)
      envelope = Interval::ALL_REALS;

    else if(v.codomain().is_degenerated())
      envelope = yiub(t, x, v) | yilb(t, x, v);

    else
    {
      envelope.set_empty();

      Interval t_inter_ub = linesIntersectionUb(x, v);

        if(t_inter_ub.intersects(t))
          envelope |= youb(t_inter_ub, x, v);

        if(t_inter_ub.ub() <= t.lb())
          envelope |= youb(t, x, v);

        if(t_inter_ub.lb() >= t.ub())
          envelope |= yiub(t, x, v);

      Interval t_inter_lb = linesIntersectionLb(x, v);

        if(t_inter_lb.intersects(t))
          envelope |= yolb(t_inter_lb, x, v);

        if(t_inter_lb.ub() <= t.lb())
          envelope |= yolb(t, x, v);

        if(t_inter_lb.lb() >= t.ub())
          envelope |= yilb(t, x, v);
    }

    Interval prev_y = y;
    envelope &= x.codomain();
    y &= envelope;
    return prev_y != y;
  }
}