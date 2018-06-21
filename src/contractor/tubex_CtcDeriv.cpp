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

  bool CtcDeriv::contract(TubeSlice& x, const TubeSlice& v)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);

    TubeSlice prev_x = x;
    Interval t = x.domain();
    IntervalVector y = x.codomain();

    contractGates(x, v);

    for(int i = 0 ; i < x.dim() ; i++)
    {
      ConvexPolygon p; // unused
      contractEnvelope(i, x, v, t, y[i], p);
    }

    x.setEnvelope(y);

    return x != prev_x;
  }

  bool CtcDeriv::contract(const TubeSlice& x, const TubeSlice& v, Interval& t, IntervalVector& y)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    DimensionException::check(x, y);

    bool ctc = false;
    TubeSlice x_temp = x;
    contractGates(x_temp, v);

    for(int i = 0 ; i < x.dim() ; i++)
    {
      ConvexPolygon p; // unused
      ctc |= contractEnvelope(i, x_temp, v, t, y[i], p);
    }

    return ctc;
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

  bool CtcDeriv::contract(const TubeVector& x, const TubeVector& v, Interval& t, IntervalVector& y)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    DimensionException::check(x, y);
    
    IntervalVector prev_y = y;
    Interval prev_t = t;

    t &= x.domain();

    TubeSlice *x_slice = (TubeSlice*)x.getSlice(t.lb());
    TubeSlice *x_last_slice = (TubeSlice*)x.getSlice(t.ub());
    TubeSlice *v_slice = (TubeSlice*)v.getSlice(t.lb());

    IntervalVector y_union(x.dim(), Interval::EMPTY_SET);
    Interval t_union = Interval::EMPTY_SET;
    
    while(x_slice != x_last_slice->nextSlice())
    {
      Interval t_slice = t & x_slice->domain();
      IntervalVector y_slice = y;
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
    DomainException::check(x, v);
    DimensionException::check(x, v);
    
    bool ctc = false;
    IntervalVector in_gate = x.inputGate(), out_gate = x.outputGate();

    // Propagations from gate to gate

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

  Interval yilb(int i, const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return x.inputGate()[i].lb() + v.codomain()[i].lb() * (t - x.domain().lb());
  }

  Interval yilb_inv(int i, const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return ((y - x.inputGate()[i].lb()) / v.codomain()[i].lb()) + x.domain().lb();
  }

  Interval yiub(int i, const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return x.inputGate()[i].ub() + v.codomain()[i].ub() * (t - x.domain().lb());
  }

  Interval yiub_inv(int i, const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return ((y - x.inputGate()[i].ub()) / v.codomain()[i].ub()) + x.domain().lb();
  }

  Interval yolb(int i, const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return x.outputGate()[i].lb() + v.codomain()[i].ub() * (t - x.domain().ub());
  }

  Interval yolb_inv(int i, const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return ((y - x.outputGate()[i].lb()) / v.codomain()[i].ub()) + x.domain().ub();
  }

  Interval youb(int i, const Interval& t, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return x.outputGate()[i].ub() + v.codomain()[i].lb() * (t - x.domain().ub());
  }

  Interval youb_inv(int i, const Interval& y, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return ((y - x.outputGate()[i].ub()) / v.codomain()[i].lb()) + x.domain().ub();
  }

  Interval ylb_inv(int i, const Interval& y, const TubeSlice& x)
  {
    DimensionException::check(x, i);
    if(x.inputGate()[i].lb() == x.outputGate()[i].lb())
      return Interval::ALL_REALS;
    return x.domain().lb() + (y - x.inputGate()[i].lb()) / ((x.outputGate()[i].lb() - x.inputGate()[i].lb()) / (x.domain().diam()));
  }

  Interval yub_inv(int i, const Interval& y, const TubeSlice& x)
  {
    DimensionException::check(x, i);

    if(x.inputGate()[i].ub() == x.outputGate()[i].ub())
      return Interval::ALL_REALS;
    return x.domain().lb() + (y - x.inputGate()[i].ub()) / ((x.outputGate()[i].ub() - x.inputGate()[i].ub()) / (x.domain().diam()));
  }

  Interval linesIntersectionUb(int i, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return (x.outputGate()[i].ub() - x.inputGate()[i].ub()
            + v.codomain()[i].ub() * x.domain().lb()
            - v.codomain()[i].lb() * x.domain().ub()) / v.codomain()[i].diam();
  }

  Interval linesIntersectionLb(int i, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);
    return (x.inputGate()[i].lb() - x.outputGate()[i].lb()
            + v.codomain()[i].ub() * x.domain().ub()
            - v.codomain()[i].lb() * x.domain().lb()) / v.codomain()[i].diam();
  }

  ConvexPolygon CtcDeriv::getPolygon(int i, const TubeSlice& x, const TubeSlice& v)
  {
    DimensionException::check(x, v);
    DimensionException::check(x, i);

    ConvexPolygon p;
    Interval t = x.domain();
    Interval y = x.codomain()[i];
    contractEnvelope(i, x, v, t, y, p);
    return p;
  }

  bool CtcDeriv::contractEnvelope(int i, const TubeSlice& x, const TubeSlice& v, Interval& t, Interval& y, ConvexPolygon& p)
  {
    DomainException::check(x, v);
    DimensionException::check(x, v);
    DimensionException::check(x, i);

    Interval prev_y = y;
    Interval prev_t = t;

    if(t.is_empty() || y.is_empty())
    {
      t.set_empty();
      y.set_empty();
    }

    else if(v.codomain()[i] == Interval::ALL_REALS)
    {
      // No contraction expected
      // Polygon computed for display purposes
      IntervalVector box(2);
      box[0] = x.domain();
      box[1] = x.codomain()[i];
      p = ConvexPolygon(box);
    }

    else
    {
      // At this step, gates are supposed to be contracted

      t &= x.domain();
      DomainException::check(x, t);

      // Contracting [y]

      y.set_empty();

      vector<Point> v_pts;
      v_pts.push_back(point(t.lb(), x.inputGate()[i].lb()));

      if(t.is_degenerated())
      {
        if(t.lb() == x.domain().lb())
          y = x.inputGate()[i];

        else if(t.lb() == x.domain().ub())
          y = x.outputGate()[i];

        else
        {
          y = (x.outputGate()[i] - (x.domain().ub() - t.lb()) * v.codomain()[i])
            & (x.inputGate()[i] + (t.lb() - x.domain().lb()) * v.codomain()[i]);
        }
      }

      else
      {
        Interval y_tlb, y_tub;
        Interval t_lb = t.lb(), t_ub = t.ub();
        ConvexPolygon p_;
        contractEnvelope(i, x, v, t_lb, y_tlb, p_);
        contractEnvelope(i, x, v, t_ub, y_tub, p_);
        y |= y_tlb | y_tub;

        // Lower bounds

          if(!v.codomain()[i].is_degenerated())
          {
            Interval t_inter_lb, y_inter_lb;

            if(v.codomain()[i].lb() == NEG_INFINITY)
            {
              t_inter_lb = t.lb();
            }

            else if(v.codomain()[i].ub() == POS_INFINITY)
            {
              t_inter_lb = t.ub();
            }

            else
            {
              t_inter_lb = linesIntersectionLb(i, x, v);
            }

            if(t_inter_lb.lb() >= t.lb() && t_inter_lb.ub() <= t.ub())
            {
              y_inter_lb = yolb(i, t_inter_lb, x, v) | yilb(i, t_inter_lb, x, v);
              y |= y_inter_lb;

              if(y_inter_lb.ub() >= prev_y.lb())
                v_pts.push_back(point(t_inter_lb.mid(), y_inter_lb.mid()));

              else
              {
                Interval t_a = yilb_inv(i, prev_y.lb(), x, v);
                v_pts.push_back(point(t_a.mid(), prev_y.lb()));
                Interval t_b = yolb_inv(i, prev_y.lb(), x, v);
                v_pts.push_back(point(t_b.mid(), prev_y.lb()));
              }
            }

            else
            {
              if(t_inter_lb.ub() < t.lb())
                y |= x.outputGate()[i].lb() - (x.domain().ub() - t.lb()) * v.codomain()[i].ub();

              else if(t_inter_lb.lb() > t.ub())
                y |= x.inputGate()[i].lb() + (t.ub() - x.domain().lb()) * v.codomain()[i].lb();
            }
          }

          v_pts.push_back(point(t.ub(), x.outputGate()[i].lb()));

        // Upper bounds

          v_pts.push_back(point(t.ub(), x.outputGate()[i].ub()));

          if(!v.codomain()[i].is_degenerated())
          {
            Interval t_inter_ub, y_inter_ub;

            if(v.codomain()[i].lb() == NEG_INFINITY)
            {
              t_inter_ub = t.ub();
            }

            else if(v.codomain()[i].ub() == POS_INFINITY)
            {
              t_inter_ub = t.lb();
            }

            else
            {
              t_inter_ub = linesIntersectionUb(i, x, v);
            }

            if(t_inter_ub.lb() >= t.lb() && t_inter_ub.ub() <= t.ub())
            {
              y_inter_ub = youb(i, t_inter_ub, x, v) | yiub(i, t_inter_ub, x, v);
              y |= y_inter_ub;

              if(y_inter_ub.lb() <= prev_y.ub())
                v_pts.push_back(point(t_inter_ub.mid(), y_inter_ub.mid()));

              else
              {
                Interval t_b = youb_inv(i, prev_y.ub(), x, v);
                v_pts.push_back(point(t_b.mid(), prev_y.ub()));
                Interval t_a = yiub_inv(i, prev_y.ub(), x, v);
                v_pts.push_back(point(t_a.mid(), prev_y.ub()));
              }
            }

            else
            {
              if(t_inter_ub.ub() < t.lb())
                y |= x.outputGate()[i].ub() - (x.domain().ub() - t.lb()) * v.codomain()[i].lb();

              else if(t_inter_ub.lb() > t.ub())
                y |= x.inputGate()[i].ub() + (t.ub() - x.domain().lb()) * v.codomain()[i].ub();
            }
          }
      }
      
      v_pts.push_back(point(t.lb(), x.inputGate()[i].ub()));

      y &= prev_y;
      y &= x.codomain()[i];
      p = ConvexPolygon(v_pts);
    }

    if(t.is_empty() || y.is_empty())
    {
      t.set_empty();
      y.set_empty();
    }

    return prev_y != y || prev_t != t;
  }
}