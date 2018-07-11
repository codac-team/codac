/* ============================================================================
 *  tubex-lib - TubeSlice class (operators)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeSlice.h"
#include "tubex_DomainException.h"
#include "tubex_SamplingException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
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

  const ConvexPolygon TubeSlice::polygon(int i, const TubeSlice& v) const
  {
    DomainException::check(*this, v);
    DimensionException::check(*this, v);
    DimensionException::check(*this, i);

    Interval t = domain();
    
    if(t.is_empty() || codomain()[i].is_empty())
      return ConvexPolygon();

    else if(v.codomain()[i] == Interval::ALL_REALS)
    {
      IntervalVector box(2);
      box[0] = domain();
      box[1] = codomain()[i];
      return ConvexPolygon(box);
    }

    else
    {
      vector<Point> v_pts;
      v_pts.push_back(Point(t.lb(), inputGate()[i].lb()));

      if(!t.is_degenerated())
      {
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
              t_inter_lb = linesIntersectionLb(i, *this, v);
            }

            if(t_inter_lb.lb() >= t.lb() && t_inter_lb.ub() <= t.ub())
            {
              y_inter_lb = yolb(i, t_inter_lb, *this, v) | yilb(i, t_inter_lb, *this, v);

              if(y_inter_lb.ub() >= codomain()[i].lb())
                v_pts.push_back(Point(t_inter_lb.mid(), y_inter_lb.mid()));

              else
              {
                Interval t_a = yilb_inv(i, codomain()[i].lb(), *this, v);
                v_pts.push_back(Point(t_a.mid(), codomain()[i].lb()));
                Interval t_b = yolb_inv(i, codomain()[i].lb(), *this, v);
                v_pts.push_back(Point(t_b.mid(), codomain()[i].lb()));
              }
            }
          }

          v_pts.push_back(Point(t.ub(), outputGate()[i].lb()));

        // Upper bounds

          v_pts.push_back(Point(t.ub(), outputGate()[i].ub()));

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
              t_inter_ub = linesIntersectionUb(i, *this, v);
            }

            if(t_inter_ub.lb() >= t.lb() && t_inter_ub.ub() <= t.ub())
            {
              y_inter_ub = youb(i, t_inter_ub, *this, v) | yiub(i, t_inter_ub, *this, v);

              if(y_inter_ub.lb() <= codomain()[i].ub())
                v_pts.push_back(Point(t_inter_ub.mid(), y_inter_ub.mid()));

              else
              {
                Interval t_b = youb_inv(i, codomain()[i].ub(), *this, v);
                v_pts.push_back(Point(t_b.mid(), codomain()[i].ub()));
                Interval t_a = yiub_inv(i, codomain()[i].ub(), *this, v);
                v_pts.push_back(Point(t_a.mid(), codomain()[i].ub()));
              }
            }
          }
      }
      
      v_pts.push_back(Point(t.lb(), inputGate()[i].ub()));
      return ConvexPolygon(v_pts);
    }
  }
}