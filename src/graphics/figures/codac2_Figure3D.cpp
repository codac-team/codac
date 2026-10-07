/** 
 *  codac2_Figure3D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard, Damien Massé, Quentin Brateau
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Index.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Figure3D.h"
#include "codac2_OutputFigure3D.h"
#include "codac2_Figure3D_OBJ.h"
#include "codac2_Figure3D_Rerun.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;

Figure3D::Figure3D(const std::string& name, GraphicOutput o)
  : _name(name)
{
  if(o & GraphicOutput::OBJ)
    _output_figures.push_back(make_shared<Figure3D_OBJ>(*this));
  if(o & GraphicOutput::RERUN)
    _output_figures.push_back(make_shared<Figure3D_Rerun>(*this));
}

Figure3D::~Figure3D()
{
}

const std::string& Figure3D::name() const
{
  return _name;
}

std::vector<std::shared_ptr<OutputFigure3D>> Figure3D::output_figures()
{
  return _output_figures;
}

void Figure3D::clear()
{
  for(const auto& output_fig : _output_figures)
    output_fig->clear();
}

void Figure3D::save(const std::string& filename)
{
  for(const auto& output_fig : _output_figures)
    output_fig->save(filename);
}

void Figure3D::draw_point(const Vector& c, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_point(c, style);
}

void Figure3D::draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_line(p1, p2, style);
}

void Figure3D::draw_polyline(const std::vector<Vector>& x, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_polyline(x, style);
}

void Figure3D::draw_triangle(const Vector &c, const Matrix &A,
  const Vector &p1, const Vector &p2, const Vector &p3, 
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_triangle(c, A, p1, p2, p3, style);
}

void Figure3D::draw_triangle(const Vector &p1, const Vector &p2,
  const Vector &p3, 
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_triangle(p1, p2, p3, style);
}

void Figure3D::draw_polygon(const Vector &c, const Matrix &A,
  const std::vector<Vector> &l, 
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_polygon(c, A, l, style);
}

void Figure3D::draw_parallelogram(const Vector &c, const Matrix &A,
  const Vector &p, const Vector &v1, const Vector &v2, 
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_parallelogram(c, A, p, v1, v2, style);
}

void Figure3D::draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_parallelepiped(p, style);
}

void Figure3D::draw_zonotope(const Zonotope& z, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_zonotope(z, style);
}

void Figure3D::draw_box(const IntervalVector& x, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_box(x, style);
}

void Figure3D::draw_arrow(const Vector &c, const Matrix& A, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_arrow(c, A, style);
}

void Figure3D::draw_axes(double size, const Vector& origin)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_axes(size, origin);
}

void Figure3D::draw_surface(const Vector &c, const Matrix &A,
  const Interval &Ip1, double dp1,
  const Interval &Ip2, double dp2,
  std::function<Vector(double,double)> f,
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_surface(c, A, Ip1, dp1, Ip2, dp2, f, style);
}

void Figure3D::draw_sphere(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_sphere(c, A, style);
}

void Figure3D::draw_ellipsoid(const Ellipsoid &e, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_ellipsoid(e, style);
}

void Figure3D::draw_car(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_car(c, A, style);
}

void Figure3D::draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up,
  const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_plane(c, A, yaw_is_up, style);
}

void Figure3D::draw_AUV(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_AUV(c, A, style);
}

void Figure3D::draw_paving(const PavingOut& p, const StyleProperties& bound_style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_paving(p, bound_style);
}

void Figure3D::draw_paving(const PavingOut& p, const PavingStyle& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_paving(p, style.boundary);
}

void Figure3D::draw_paving(const PavingInOut& p, const StyleProperties& bound_style,
  const StyleProperties& in_style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_paving(p, bound_style, in_style);
}

void Figure3D::draw_paving(const PavingInOut& p, const PavingStyle& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_paving(p, style.boundary, style.inside);
}

void Figure3D::draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_trajectory(x, style);
}

void Figure3D::draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_trajectory(x, style);
}

void Figure3D::draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_tube(x, style);
}
