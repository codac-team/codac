/** 
 *  codac2_Figure2D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard, Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Index.h"
#include "codac2_Figure2D.h"
#include "codac2_Figure2D_VIBes.h"
#include "codac2_Figure2D_IPE.h"
#include "codac2_math.h"
#include "codac2_pave.h"
#include "codac2_matrices.h"
#include "codac2_Matrix.h"

using namespace std;
using namespace codac2;

shared_ptr<Figure2D> DefaultFigure::_default_fig = nullptr;
shared_ptr<Figure2D> DefaultFigure::_selected_fig = DefaultFigure::_default_fig;

Figure2D::Figure2D(const std::string& name, GraphicOutput o, bool set_as_default_)
  : _name(name)
{
  if(o & GraphicOutput::VIBES)
    _output_figures.push_back(std::make_shared<Figure2D_VIBes>(*this));
  if(o & GraphicOutput::IPE)
    _output_figures.push_back(std::make_shared<Figure2D_IPE>(*this));
  if(set_as_default_)
    set_as_default();
}

const std::string& Figure2D::name() const
{
  return _name;
}

Index Figure2D::size() const
{
  return _axes.size();
}

const std::vector<FigureAxis>& Figure2D::axes() const
{
  return _axes;
}

void Figure2D::set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
{
  _axes = { axis1, axis2 };
  for(const auto& output_fig : _output_figures)
    output_fig->update_axes();
}

const Index& Figure2D::i() const
{
  return axes()[0].dim_id;
}

const Index& Figure2D::j() const
{
  return axes()[1].dim_id;
}

const Vector& Figure2D::pos() const
{
  return _pos;
}

const Vector& Figure2D::window_size() const
{
  return _window_size;
}

void Figure2D::set_window_properties(const Vector& pos, const Vector& size)
{
  assert_release(pos.size() == 2 && size.size() == 2);
  assert_release(size.min_coeff() > 0);

  _pos = pos; _window_size = size;
  for(const auto& output_fig : _output_figures)
    output_fig->update_window_properties();
}

void Figure2D::center_viewbox(const Vector& c, const Vector& r)
{
  assert_release(this->size() <= c.size() && this->size() <= r.size());
  assert_release(r.min_coeff() > 0.);

  for(const auto& output_fig : _output_figures)
    output_fig->center_viewbox(c,r);
}

double Figure2D::scaled_unit() const
{
  return std::max(_axes[0].limits.diam(),_axes[1].limits.diam()) / _window_size.max_coeff();
}

void Figure2D::auto_scale()
{
  Vector w = this->window_size();
  if(_axes[0].limits.diam() > _axes[1].limits.diam())
    w[1] *= _axes[1].limits.diam()/_axes[0].limits.diam();
  else
    w[0] *= _axes[0].limits.diam()/_axes[1].limits.diam();

  this->set_window_properties(this->pos(), w);
}

bool Figure2D::is_default() const
{
  return DefaultFigure::_selected_fig == this->weak_from_this().lock();
}

void Figure2D::set_as_default()
{
  DefaultFigure::set(this->shared_from_this());
}

void Figure2D::set_tdomain(const Interval& tdomain)
{
  _tdomain = tdomain;
}

void Figure2D::draw_point(const Vector& c, const StyleProperties& s)
{
  assert_release(this->size() <= c.size());

  for(const auto& output_fig : _output_figures)
    output_fig->draw_point(c,s);
}

void Figure2D::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert_release(this->size() <= x.size());
  if(!x.is_empty())
    for(const auto& output_fig : _output_figures)
    {
      if(x.max_diam() == 0.)
        output_fig->draw_point({x[0].lb(),x[1].lb()}, s);
      else
        output_fig->draw_box(x,s);
    }
}

void Figure2D::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  assert_release(this->size() <= c.size());
  assert_release(r > 0.);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_circle(c,r,s);
}

void Figure2D::draw_ring(const Vector& c, const Interval& r, const StyleProperties& s)
{
  assert_release(this->size() <= c.size());
  assert_release(!r.is_empty() && r.lb() >= 0.);

  if(!r.is_empty())
    for(const auto& output_fig : _output_figures)
      output_fig->draw_ring(c,r,s);
}

void Figure2D::draw_line(const Vector& p1, const Vector& p2, const StyleProperties& s)
{
  assert_release(p1.size() == p2.size());
  assert_release(this->size() <= p1.size());
  draw_polyline({p1,p2}, s);
}

void Figure2D::draw_line(const Segment& e, const StyleProperties& s)
{
  draw_polyline({e[0].mid(),e[1].mid()}, s);
  if(!e[0].is_degenerated())
    draw_point(e[0].mid(),s); // revealing thick points
  if(!e[1].is_degenerated())
    draw_point(e[1].mid(),s); // revealing thick points
}

void Figure2D::draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& s)
{
  assert_release(p1.size() == p2.size());
  assert_release(this->size() <= p1.size());
  draw_polyline({p1,p2}, tip_length, s);
}

void Figure2D::draw_polyline(const vector<Vector>& x, const StyleProperties& s)
{
  draw_polyline(x, 0., s);
}

void Figure2D::draw_polyline(const vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  assert_release(x.size() > 1);
  assert_release(tip_length >= 0.); // 0 = disabled tip
  for([[maybe_unused]] const auto& xi : x)
  {
    assert_release(this->size() <= xi.size());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polyline(x,tip_length,s);
}

void Figure2D::draw_polygon(const Polygon& x, const StyleProperties& s)
{
  assert_release(x.size() > 1);

  vector<Vector> w;
  for(const auto& xi : x.sorted_vertices())
  {
    assert_release(this->size() <= xi.size());
    if(!xi.is_degenerated())
      draw_point(xi.mid(),s); // revealing thick points
    w.push_back(xi.mid());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polygon(w,s);
}

void Figure2D::draw_zonotope(const Vector& z, const std::vector<Vector>& A, const StyleProperties& s)
{
   std::map<double,Vector> sides;
   for (auto &u : A) {
       assert_release(u.size()==2);
       if (u==Vector::zero(2)) continue;
       double theta = std::atan2(u[1],u[0]);
       Vector v(u);
       if (theta<=0.0) { theta=theta+PI; v=-v; } 
    // Theta in ]0,PI] , v[1]>=0 and if v[1]=0, v[0]<0
       auto try_insert=sides.insert({theta,v});
       if (try_insert.second==false) {
           (try_insert.first)->second += v;
       }
   }
   std::vector<Vector> vertices;
   Vector point=z;
   // Start from v[1] maximum (and v[0] min for horizontal side)
   for (const auto &a : sides) {
       point+=a.second;
   }
   // Turn anticlockwise : first half
   for (const auto &a : sides) {
       vertices.push_back(point);
       point-=2*a.second;
   }
   // Turn anticlockwise : second half
   for (const auto &a : sides) {
       vertices.push_back(point);
       point+=2*a.second;
   }
   for(const auto& output_fig : _output_figures)
      output_fig->draw_polygon(vertices,s);
}


void Figure2D::draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s)
{
  assert_release(A.is_squared() && A.rows() == z.size());
  assert_release(z.size() == 2);

  auto a1 = A.col(0), a2 = A.col(1);

  draw_polygon(vector<Vector>({
      Vector(z+a1+a2), Vector(z-a1+a2),
      Vector(z-a1-a2), Vector(z+a1-a2)
    }), s);
}

void Figure2D::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert_release(this->size() <= c.size());
  assert_release(r.lb() >= 0.);

  if(r.is_empty() || theta.is_empty())
    return;

  Interval theta_(theta);
  if(theta.is_unbounded())
    theta_ = Interval(0,2.*PI);

  Interval r_(r);
  if(r.is_unbounded())
    r_ &= Interval(0,previous_float(oo));

  for(const auto& output_fig : _output_figures)
    output_fig->draw_pie(c,r_,theta_,s);
}

void Figure2D::draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s)
{
  assert_release(c.size() == 2);
  assert_release(ab.size() == 2);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_ellipse(c,ab,theta,s);
}

void Figure2D::draw_ellipsoid(const Ellipsoid &e, const StyleProperties &s)
{
  // Author: Morgan Louédec
  assert_release(this->size() <= e.size());

  Index n = e.size();
  Ellipsoid proj_e(2);

  // 2d projection of the ellipsoid
  if(n > 2)
  {
    Vector v = Vector::zero(n);
    v[i()] = 1;
    Vector u = Vector::zero(n);
    u[j()] = 1;
    proj_e = e.proj_2d(Vector::zero(n), v, u);
  }

  else
    proj_e = e;

  // draw the 2d ellipsoid
  Eigen::JacobiSVD<Matrix> jsvd(proj_e.G, Eigen::ComputeThinU);
  Matrix U(jsvd.matrixU());
  Vector ab(jsvd.singularValues());

  double theta = std::atan2(U(1, 0), U(0, 0));

  for(const auto& output_fig : _output_figures)
    output_fig->draw_ellipse(proj_e.mu, ab, theta, s);
}

void Figure2D::draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& s)
{
  assert_release(this->size() <= x.size());

  std::vector<Vector> values;
  for(const auto& [ti,xi] : x)
    if(_tdomain.contains(ti))
      values.push_back(xi);

  if(values.size() > 1)
    draw_polyline(values,s);
}

void Figure2D::draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& s)
{
  draw_trajectory(x.sampled(x.tdomain().diam()/1e4), s);
}

void Figure2D::draw_trajectory(const SampledTraj<Vector>& x, const ColorMap& cmap)
{
  assert_release(this->size() <= x.size());

  double range = x.tdomain().diam();

  for(auto it = x.begin(); std::next(it) != x.end(); ++it)
    if(_tdomain.contains(it->first))
      draw_polyline(
        { it->second, std::next(it)->second },
        cmap.color((it->first - x.begin()->first) / range));
}

void Figure2D::draw_trajectory(const AnalyticTraj<VectorType>& x, const ColorMap& cmap)
{
  draw_trajectory(x.sampled(x.tdomain().diam()/1e4), cmap);
}

void Figure2D::plot_trajectory(const SampledTraj<double>& x, const StyleProperties& s)
{
  std::vector<Vector> values;
  for(const auto& [ti,xi] : x)
    if(_tdomain.contains(ti))
      values.push_back({ti,xi});

  if(values.size() > 1)
  {
    _axes[0].limits = x.tdomain();
    _axes[1].limits = x.codomain();

    for(const auto& output_fig : _output_figures)
      output_fig->update_axes();

    draw_polyline(values,s);
  }
}

void Figure2D::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_tank(x,size,s);
  }
}

void Figure2D::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_AUV(x,size,s);
  }
}

void Figure2D::draw_motor_boat(const Vector& x, float size, const StyleProperties& s)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_motor_boat(x,size,s);
  }
}

void Figure2D::draw_paving(const PavingOut& p,
  const StyleProperties& boundary_style,const StyleProperties& outside_style)
{
    p.tree()->left()->visit([&]
      (std::shared_ptr<const PavingOut_Node> n)
      {
        const IntervalVector& outer = get<0>(n->boxes());

        if(n->top() == p.tree())
          draw_box(get<0>(n->top()->boxes()), outside_style);

        else
        {
          auto b = get<0>(n->top()->boxes()).bisect_largest();
          IntervalVector hull = n->top()->left() == n ? b.first : b.second;

          for(const auto& bi : hull.diff(outer))
            draw_box(bi, outside_style);
        }

        if(n->is_leaf())
          draw_box(outer, boundary_style);

        return true;
      });
}

void Figure2D::draw_paving(const PavingInOut& p, const StyleProperties& boundary_style,
  const StyleProperties& outside_style, const StyleProperties& inside_style)
{
  p.tree()->visit([&]
    (std::shared_ptr<const PavingInOut_Node> n)
    {
      const IntervalVector& outer = get<0>(n->boxes());
      const IntervalVector& inner = get<1>(n->boxes());

      IntervalVector hull = inner | outer;

      for(const auto& bi : hull.diff(inner))
        draw_box(bi, inside_style);

      for(const auto& bi : hull.diff(outer))
        draw_box(bi, outside_style);

      if(n->is_leaf())
          draw_box(inner & outer, boundary_style);

      return true;
    });
}
