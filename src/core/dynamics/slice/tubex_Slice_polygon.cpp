/** 
 *  Slice class (polygons)
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Slice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Interval yilb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.input_gate().lb() + v.codomain().lb() * (t - x.tdomain().lb());
  }

  Interval yilb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.input_gate().lb()) / v.codomain().lb()) + x.tdomain().lb();
  }

  Interval yiub(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.input_gate().ub() + v.codomain().ub() * (t - x.tdomain().lb());
  }

  Interval yiub_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.input_gate().ub()) / v.codomain().ub()) + x.tdomain().lb();
  }

  Interval yolb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.output_gate().lb() + v.codomain().ub() * (t - x.tdomain().ub());
  }

  Interval yolb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.output_gate().lb()) / v.codomain().ub()) + x.tdomain().ub();
  }

  Interval youb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.output_gate().ub() + v.codomain().lb() * (t - x.tdomain().ub());
  }

  Interval youb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.output_gate().ub()) / v.codomain().lb()) + x.tdomain().ub();
  }

  Interval ylb_inv(const Interval& y, const Slice& x)
  {
    if(x.input_gate().lb() == x.output_gate().lb())
      return Interval::ALL_REALS;
    return x.tdomain().lb() + (y - x.input_gate().lb()) / ((x.output_gate().lb() - x.input_gate().lb()) / (x.tdomain().diam()));
  }

  Interval yub_inv(const Interval& y, const Slice& x)
  {
    if(x.input_gate().ub() == x.output_gate().ub())
      return Interval::ALL_REALS;
    return x.tdomain().lb() + (y - x.input_gate().ub()) / ((x.output_gate().ub() - x.input_gate().ub()) / (x.tdomain().diam()));
  }

  Interval lines_intersection_ub(const Slice& x, const Slice& v)
  {
    return (x.output_gate().ub() - x.input_gate().ub()
            + v.codomain().ub() * x.tdomain().lb()
            - v.codomain().lb() * x.tdomain().ub()) / v.codomain().diam();
  }

  Interval lines_intersection_lb(const Slice& x, const Slice& v)
  {
    return (x.input_gate().lb() - x.output_gate().lb()
            + v.codomain().ub() * x.tdomain().ub()
            - v.codomain().lb() * x.tdomain().lb()) / v.codomain().diam();
  }

  const ConvexPolygon Slice::polygon(const Slice& v) const
  {
    assert(tdomain() == v.tdomain());

    Interval t = tdomain();
    assert(!t.is_degenerated());
    
    if(t.is_empty() || codomain().is_empty())
      return ConvexPolygon(); // empty polygon

    else if(v.codomain() == Interval::ALL_REALS)
    {
      IntervalVector box(2);
      box[0] = tdomain();
      box[1] = codomain();
      return ConvexPolygon(box);
    }

    else
    {
      vector<Vector> v_pts;
      v_pts.push_back(Vector({t.lb(), input_gate().lb()}));

      // Lower bounds

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_lb, y_inter_lb;

          if(v.codomain().lb() == NEG_INFINITY)
            t_inter_lb = t.lb();

          else if(v.codomain().ub() == POS_INFINITY)
            t_inter_lb = t.ub();

          else
            t_inter_lb = lines_intersection_lb(*this, v);

          if(t_inter_lb.lb() >= t.lb() && t_inter_lb.ub() <= t.ub())
          {
            y_inter_lb = yolb(t_inter_lb, *this, v) | yilb(t_inter_lb, *this, v);

            if(y_inter_lb.ub() >= codomain().lb())
            {
              if(t_inter_lb.is_degenerated())
                v_pts.push_back(Vector({t_inter_lb.ub(), y_inter_lb.lb()}));

              else
              {
                // The following transforms the line intersection result into
                // two floating 2d vectors. This creates an additional point,
                // but maintains reliability.
                v_pts.push_back(Vector({t_inter_lb.lb(), y_inter_lb.lb()}));
                v_pts.push_back(Vector({t_inter_lb.ub(), y_inter_lb.lb()}));
              }
            }

            else
            {
              Interval t_a = yilb_inv(codomain().lb(), *this, v);
              v_pts.push_back(Vector({t_a.lb(), codomain().lb()}));
              Interval t_b = yolb_inv(codomain().lb(), *this, v);
              v_pts.push_back(Vector({t_b.ub(), codomain().lb()}));
            }
          }
        }

        v_pts.push_back(Vector({t.ub(), output_gate().lb()}));

      // Upper bounds

        v_pts.push_back(Vector({t.ub(), output_gate().ub()}));

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_ub, y_inter_ub;

          if(v.codomain().lb() == NEG_INFINITY)
            t_inter_ub = t.ub();

          else if(v.codomain().ub() == POS_INFINITY)
            t_inter_ub = t.lb();

          else
            t_inter_ub = lines_intersection_ub(*this, v);

          if(t_inter_ub.lb() >= t.lb() && t_inter_ub.ub() <= t.ub())
          {
            y_inter_ub = youb(t_inter_ub, *this, v) | yiub(t_inter_ub, *this, v);

            if(y_inter_ub.lb() <= codomain().ub())
            {
              if(t_inter_ub.is_degenerated())
                v_pts.push_back(Vector({t_inter_ub.ub(), y_inter_ub.ub()}));

              else
              {
                // The following transforms the line intersection result into
                // two floating 2d vectors. This creates an additional point,
                // but maintains reliability.
                v_pts.push_back(Vector({t_inter_ub.ub(), y_inter_ub.ub()}));
                v_pts.push_back(Vector({t_inter_ub.lb(), y_inter_ub.ub()}));
              }
            }

            else
            {
              Interval t_b = youb_inv(codomain().ub(), *this, v);
              v_pts.push_back(Vector({t_b.ub(), codomain().ub()}));
              Interval t_a = yiub_inv(codomain().ub(), *this, v);
              v_pts.push_back(Vector({t_a.lb(), codomain().ub()}));
            }
          }
        }
      
      v_pts.push_back(Vector({t.lb(), input_gate().ub()}));
      v_pts = Point::remove_identical_pts(v_pts);
      return ConvexPolygon(v_pts, true);
    }
  }
}