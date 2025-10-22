/** 
 *  \file codac2_Slice_polygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  template<typename T>
  ConvexPolygon Slice<T>::polygon_slice(const Slice<T>& v) const
    requires std::is_same_v<T,Interval>
  {
    const Interval& t = t0_tf();

    ConvexPolygon p(IntervalVector({t,codomain()
       & Interval(next_float(-oo),prev_float(oo)) // todo: remove this
     }));

    if(v.codomain().is_unbounded())
      return p;

    ConvexPolygon p_fwd(std::vector<Vector>({
      {t.ub(), (input_gate().ub()+Interval(t.diam())*v.codomain().ub()).ub()},
      {t.ub(), (input_gate().lb()+Interval(t.diam())*v.codomain().lb()).lb()},
      {t.lb(), input_gate().lb()},
      {t.lb(), input_gate().ub()}
    }), false); // false: avoid costly computation of convex hull (vertices are already ordered)

    ConvexPolygon p_bwd(std::vector<Vector>({
      {t.lb(), (output_gate().ub()-Interval(t.diam())*v.codomain().lb()).ub()},
      {t.lb(), (output_gate().lb()-Interval(t.diam())*v.codomain().ub()).lb()},
      {t.ub(), output_gate().lb()},
      {t.ub(), output_gate().ub()}
    }), false); // false: avoid costly computation of convex hull (vertices are already ordered)

    return p & p_fwd & p_bwd;
  }
}