/** 
 *  \file codac2_OutputFigure3D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure3D.h"
#include "codac2_OutputFigure3D.h"
#include "codac2_Paving.h"

using namespace std;
using namespace codac2;

void OutputFigure3D::draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style)
{
  draw_polyline({p1, p2}, style);
}

void OutputFigure3D::draw_triangle(const Vector &p1, const Vector &p2, const Vector &p3, const StyleProperties& style)
{
  draw_triangle(Vector::Zero(3), Matrix::eye(3, 3), p1, p2, p3, style);
}

void OutputFigure3D::draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style)
{
  std::vector<Vector> values;

  auto flush_and_draw = [&]() {
    if(values.size() > 1)
      draw_polyline(values, style);
    else if(values.size() == 1)
      draw_point(values[0], style);
    values.clear();
  };

  for(const auto& [ti, xi] : x)
  {
    if(!xi.is_nan())
      values.push_back(xi);
    else
      flush_and_draw();
  }
  flush_and_draw();
}

void OutputFigure3D::draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style)
{
  draw_trajectory(x.sampled(x.tdomain().diam() / 1e4), style);
}

void OutputFigure3D::draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style)
{
  if(x.is_empty())
    return;

  for(auto it = x.tdomain()->begin(); it != x.tdomain()->end(); ++it)
  {
    const auto& box = x.slice(it)->codomain();
    if(!box.is_empty())
      draw_box(box, style);
  }
}

void OutputFigure3D::draw_paving(const PavingOut& p, const StyleProperties& boundary_style)
{
  StyleProperties style = boundary_style;
  if(style.layer.empty() || style.layer == "alpha")
    style.layer = "paving_bound";

  if(p.tree() && p.tree()->left())
  {
    p.tree()->left()->visit([&](std::shared_ptr<const PavingOut_Node> n) {
      const IntervalVector& outer = get<0>(n->boxes());
      if(n->is_leaf() && !outer.is_empty())
      {
        draw_box(outer, style);
      }
      return true;
    });
  }
}

void OutputFigure3D::draw_paving(const PavingInOut& p, const StyleProperties& boundary_style,
                                const StyleProperties& inside_style)
{
  StyleProperties bound = boundary_style;
  if(bound.layer.empty() || bound.layer == "alpha")
    bound.layer = "paving_bound";

  StyleProperties in = inside_style;
  if(in.layer.empty() || in.layer == "alpha")
    in.layer = "paving_in";

  if(p.tree())
  {
    p.tree()->visit([&](std::shared_ptr<const PavingInOut_Node> n) {
      const IntervalVector& outer = get<0>(n->boxes());
      const IntervalVector& inner = get<1>(n->boxes());

      IntervalVector hull = inner | outer;
      for(const auto& bi : hull.diff(inner))
      {
        if(!bi.is_empty())
        {
          draw_box(bi, in);
        }
      }

      if(n->is_leaf())
      {
        auto boundary = inner & outer;
        if(!boundary.is_empty())
        {
          draw_box(boundary, bound);
        }
      }
      return true;
    });
  }
}
