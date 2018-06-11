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
#include "tubex_ConvexPolygon.h"

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
    Interval t = x.domain();
    Interval y = x.codomain();
    ConvexPolygon p; // unused

    ctc |= contractGates(x, v);
    ctc |= contractEnvelope(x, v, t, y, p);
    x.setEnvelope(y);

    return ctc;
  }

  bool CtcDeriv::contract(const TubeSlice& x, const TubeSlice& v, Interval& t, Interval& y)
  {
    StructureException::check(x, v);
    TubeSlice x_temp = x;
    ConvexPolygon p; // unused
    contractGates(x_temp, v);
    return contractEnvelope(x_temp, v, t, y, p);
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

    return ctc;
  }

  bool CtcDeriv::contract(const Tube& x, const Tube& v, Interval& t, Interval& y)
  {
    Interval prev_y = y;
    Interval prev_t = t;

    t &= x.domain();

    TubeSlice *x_slice = (TubeSlice*)x.getSlice(t.lb());
    TubeSlice *x_last_slice = (TubeSlice*)x.getSlice(t.ub());
    TubeSlice *v_slice = (TubeSlice*)v.getSlice(t.lb());

    Interval y_union = Interval::EMPTY_SET;
    Interval t_union = Interval::EMPTY_SET;
    
    while(x_slice != x_last_slice->nextSlice())
    {
      Interval y_slice = y, t_slice = t & x_slice->domain();
      contract(*x_slice, *v_slice, t_slice, y_slice);
      y_union |= y_slice;
      t_union |= t_slice;

      x_slice = x_slice->nextSlice();
      v_slice = v_slice->nextSlice();
    }

    y = y_union;
    t = t_union;
    return prev_y != y || prev_t != t;
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

  ConvexPolygon CtcDeriv::getPolygon(const TubeSlice& x, const TubeSlice& v)
  {
    if(v.codomain().is_unbounded())
    {
      //throw Exception("CtcDeriv::getPolygon", "unbounded derivative");
    }

    else if(x.inputGate() == Interval::ALL_REALS || x.outputGate() == Interval::ALL_REALS)
    {
      //throw Exception("CtcDeriv::getPolygon", "unbounded gates");
    }

    else
    {
      ConvexPolygon p;
      Interval t = x.domain();
      Interval y = x.codomain();
      contractEnvelope(x, v, t, y, p);
      return p;
    }
  }

  bool CtcDeriv::contractEnvelope(const TubeSlice& x, const TubeSlice& v, Interval& t, Interval& y, ConvexPolygon& p)
  {
    t &= x.domain();
    DomainException::check(x, t);
    Interval prev_y = y;
    y.set_empty();
    Interval prev_t = t;

    // At this step, gates are already contracted

    vector<Point> v_pts;
    v_pts.push_back(point(t.lb(), x.inputGate().lb()));

    if(t.is_degenerated())
    {
      if(t.lb() == x.domain().lb())
        y = x.inputGate();

      else if(t.lb() == x.domain().ub())
        y = x.outputGate();

      else
      {
        y = (x.outputGate() - (x.domain().ub() - t.lb()) * v.codomain())
          & (x.inputGate() + (t.lb() - x.domain().lb()) * v.codomain());
      }
    }

    else
    {
      y |= x.inputGate() | x.outputGate();

      // Lower bounds

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_lb, y_inter_lb;

          if(v.codomain().lb() == NEG_INFINITY)
          {
            t_inter_lb = t.lb();
          }

          else if(v.codomain().ub() == POS_INFINITY)
          {
            t_inter_lb = t.ub();
          }

          else
          {
            t_inter_lb = linesIntersectionLb(x, v);
          }

          if(t_inter_lb.lb() >= t.lb() && t_inter_lb.ub() <= t.ub())
          {
            y_inter_lb = yolb(t_inter_lb, x, v) | yilb(t_inter_lb, x, v);
            y |= y_inter_lb;

            if(y_inter_lb.ub() >= y.lb())
              v_pts.push_back(point(t_inter_lb.mid(), y_inter_lb.mid()));

            else
            {
              Interval t_a = yilb_inv(y.lb(), x, v);
              v_pts.push_back(point(t_a.mid(), y.lb()));
              Interval t_b = yolb_inv(y.lb(), x, v);
              v_pts.push_back(point(t_b.mid(), y.lb()));
            }
          }
        }

        v_pts.push_back(point(t.ub(), x.outputGate().lb()));

      // Upper bounds

        v_pts.push_back(point(t.ub(), x.outputGate().ub()));

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_ub, y_inter_ub;

          if(v.codomain().lb() == NEG_INFINITY)
          {
            t_inter_ub = t.ub();
          }

          else if(v.codomain().ub() == POS_INFINITY)
          {
            t_inter_ub = t.lb();
          }

          else
          {
            t_inter_ub = linesIntersectionUb(x, v);
          }

          if(t_inter_ub.lb() >= t.lb() && t_inter_ub.ub() <= t.ub())
          {
            y_inter_ub = youb(t_inter_ub, x, v) | yiub(t_inter_ub, x, v);
            y |= y_inter_ub;

            if(y_inter_ub.lb() <= y.ub())
              v_pts.push_back(point(t_inter_ub.mid(), y_inter_ub.mid()));

            else
            {
              Interval t_a = yiub_inv(y.ub(), x, v);
              v_pts.push_back(point(t_a.mid(), y.ub()));
              Interval t_b = youb_inv(y.ub(), x, v);
              v_pts.push_back(point(t_b.mid(), y.ub()));
            }
          }
        }

        v_pts.push_back(point(t.lb(), x.inputGate().ub()));
    }

    y &= x.codomain();

    p = ConvexPolygon(v_pts);

    return prev_y != y || prev_t != t;
  }
}