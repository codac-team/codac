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

Figure2D::Figure2D(const string& name, GraphicOutput o, bool set_as_default_)
  : _name(name)
{
  if(o & GraphicOutput::VIBES)
    _output_figures.push_back(make_shared<Figure2D_VIBes>(*this));
  if(o & GraphicOutput::IPE)
    _output_figures.push_back(make_shared<Figure2D_IPE>(*this));
  if(set_as_default_)
    set_as_default();
}

vector<shared_ptr<OutputFigure2D>> Figure2D::output_figures()
{
  return _output_figures;
}

const string& Figure2D::name() const
{
  return _name;
}

Index Figure2D::size() const
{
  return _axes.size();
}

const vector<FigureAxis>& Figure2D::axes() const
{
  return _axes;
}

Figure2D& Figure2D::set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
{
  _axes = { axis1, axis2 };
  assert_release(axis1.dim_id != axis2.dim_id);
  for(const auto& output_fig : _output_figures)
    output_fig->update_axes();
  return *this;
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

void Figure2D::clear()
{
  for(const auto& output_fig : _output_figures)
    output_fig->clear();
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

void Figure2D::draw_point(const Vector& c, const StyleProperties& style)
{
  assert_release(this->size() <= c.size());

  for(const auto& output_fig : _output_figures)
    output_fig->draw_point(c,style);
}

void Figure2D::draw_box(const IntervalVector& x, const StyleProperties& style)
{
  assert_release(this->size() <= x.size());
  if(!x.is_empty())
    for(const auto& output_fig : _output_figures)
    {
      if(x.max_diam() == 0.)
        output_fig->draw_point({x[0].lb(),x[1].lb()}, style);
      else
        output_fig->draw_box(x,style);
    }
}

void Figure2D::draw_circle(const Vector& c, double r, const StyleProperties& style)
{
  assert_release(this->size() <= c.size());
  assert_release(r > 0.);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_circle(c,r,style);
}

void Figure2D::draw_ring(const Vector& c, const Interval& r, const StyleProperties& style)
{
  assert_release(this->size() <= c.size());
  assert_release(!r.is_empty() && r.lb() >= 0.);

  if(!r.is_empty())
    for(const auto& output_fig : _output_figures)
      output_fig->draw_ring(c,r,style);
}

void Figure2D::draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style)
{
  assert_release(p1.size() == p2.size());
  assert_release(this->size() <= p1.size());
  draw_polyline({p1,p2}, style);
}

void Figure2D::draw_line(const Segment& e, const StyleProperties& style)
{
  draw_polyline({e[0].mid(),e[1].mid()}, style);
  if(!e[0].is_degenerated())
    draw_point(e[0].mid(),style); // revealing thick points
  if(!e[1].is_degenerated())
    draw_point(e[1].mid(),style); // revealing thick points
}

void Figure2D::draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& style)
{
  assert_release(p1.size() == p2.size());
  assert_release(this->size() <= p1.size());
  draw_polyline({p1,p2}, tip_length, style);
}

void Figure2D::draw_polyline(const vector<Vector>& x, const StyleProperties& style)
{
  draw_polyline(x, 0., style);
}

void Figure2D::draw_polyline(const vector<Vector>& x, float tip_length, const StyleProperties& style)
{
  assert_release(x.size() > 1);
  assert_release(tip_length >= 0.); // 0 = disabled tip
  for([[maybe_unused]] const auto& xi : x)
  {
    assert_release(this->size() <= xi.size());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polyline(x,tip_length,style);
}

void Figure2D::draw_polygon(const Polygon& x, const StyleProperties& style)
{
  vector<Vector> w;
  for(const auto& xi : x.vertices())
  {
    assert_release(this->size() <= xi.size());
    if(!xi.is_degenerated())
      draw_point(xi.mid(),style); // revealing thick points
    w.push_back(xi.mid());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polygon(w,style);
}

void Figure2D::draw_zonotope(const Zonotope& z, const StyleProperties& style)
{
   map<double,Vector> sides;
   for (int i=0; i < z.A.cols(); i++) {
       auto u = z.A.col(i);
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
   vector<Vector> vertices;
   Vector point=z.z;
   // Start from v[1] maximum (and v[0] min for horizontal side)
   for (const auto& a : sides) {
       point+=a.second;
   }
   // Turn anticlockwise : first half
   for (const auto& a : sides) {
       vertices.push_back(point);
       point-=2*a.second;
   }
   // Turn anticlockwise : second half
   for (const auto& a : sides) {
       vertices.push_back(point);
       point+=2*a.second;
   }
   for(const auto& output_fig : _output_figures)
      output_fig->draw_polygon(vertices,style);
}


void Figure2D::draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style)
{
  assert_release(p.A.is_squared() && p.A.rows() == p.z.size());
  assert_release(p.z.size() == 2);

  auto a1 = p.A.col(0), a2 = p.A.col(1);

  draw_polygon(vector<Vector>({
      Vector(p.z+a1+a2), Vector(p.z-a1+a2),
      Vector(p.z-a1-a2), Vector(p.z+a1-a2)
    }), style);
}

void Figure2D::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& style)
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
    r_ &= Interval(0,prev_float(oo));

  for(const auto& output_fig : _output_figures)
    output_fig->draw_pie(c,r_,theta_,style);
}

void Figure2D::draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& style)
{
  assert_release(c.size() == 2);
  assert_release(ab.size() == 2);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_ellipse(c,ab,theta,style);
}

void Figure2D::draw_ellipsoid(const Ellipsoid& e, const StyleProperties& style)
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
    output_fig->draw_ellipse(proj_e.mu, ab, theta, style);
}

void Figure2D::draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style)
{
  assert_release(this->size() <= x.size());

  vector<Vector> values;

  auto display_and_clear = [&]() {

    if(values.size() > 1)
      draw_polyline(values,style);

    else if(values.size() == 1)
      draw_point(values[0],style);

    values.clear();
  };

  for(const auto& [ti,xi] : x)
    if(_tdomain.contains(ti))
    {
      if(!xi.is_nan())
        values.push_back(xi);

      else
        display_and_clear();
    }

  display_and_clear();
}

void Figure2D::draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style)
{
  draw_trajectory(x.sampled(x.tdomain().diam()/1e4), style);
}

void Figure2D::draw_trajectory(const SampledTraj<Vector>& x, const StyleGradientProperties& style)
{
  assert_release(this->size() <= x.size());

  double range = x.tdomain().diam();

  for(auto it = x.begin(); next(it) != x.end(); ++it)
    if(_tdomain.contains(it->first))
      draw_polyline(
        { it->second, next(it)->second },
        {style.cmap.color((it->first - x.begin()->first) / range), style.layer, style.line_style, to_string(style.line_width)});
}

void Figure2D::draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleGradientProperties& style)
{
  draw_trajectory(x.sampled(x.tdomain().diam()/1e4), style);
}

void Figure2D::plot_trajectory(const SampledTraj<double>& x, const StyleProperties& style)
{
  _axes[0].limits = x.tdomain();
  if(_axes[1].limits == Interval(0,1))
    _axes[1].limits.set_empty();
  _axes[1].limits |= x.codomain();
  for(const auto& output_fig : _output_figures)
    output_fig->update_axes();

  vector<Vector> values;

  auto display_and_clear = [&]() {

    if(values.size() > 1)
      draw_polyline(values,style);

    else if(values.size() == 1)
      draw_point(values[0],style);

    values.clear();
  };

  for(const auto& [ti,xi] : x)
    if(_tdomain.contains(ti))
    {
      if(!isnan(xi))
        values.push_back({ti,xi});

      else
        display_and_clear();
    }

  display_and_clear();
}

void Figure2D::plot_trajectories(const SampledTraj<Vector>& x)
{
  for(const auto& xi : as_scalar_trajs(x))
    plot_trajectory(xi,Color::random());
}

void Figure2D::plot_trajectories(const SampledTraj<Vector>& x, const StyleProperties& style)
{
  for(const auto& xi : as_scalar_trajs(x))
    plot_trajectory(xi,style);
}

template<typename Func>
void draw_tube_common(Figure2D& fig, const SlicedTube<IntervalVector>& x, int max_nb_slices_to_display, const Func& slice_color)
{
  const int n = x.nb_slices();
  auto tube_t0tf = x.tdomain()->t0_tf();

  if(n < max_nb_slices_to_display)
    for(auto it = x.tdomain()->rbegin(); it != x.tdomain()->rend(); ++it)
      fig.draw_box(x.slice(it)->codomain(), slice_color(tube_t0tf,it));

  else
  {
    int group_size = std::max(1, (int)(1.*n/max_nb_slices_to_display));

    for(auto it = x.tdomain()->rbegin() ; it != x.tdomain()->rend(); )
    {
      auto c = slice_color(tube_t0tf,it);
      ConvexPolygon p(x.slice(it)->codomain());
      it++;

      for(int j = 0; j < group_size-1 && it != x.tdomain()->rend(); j++,it++)
        p |= ConvexPolygon(x.slice(it)->codomain());

      fig.draw_polygon(p, c);
      if(it != x.tdomain()->rend())
      {
        it--; it--;
      }
    }
  }
}

void Figure2D::draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style, int max_nb_slices_to_display)
{
  draw_tube_common(*this, x, max_nb_slices_to_display,
    [&style]([[maybe_unused]] const Interval& tube_t0tf, [[maybe_unused]] std::list<TSlice>::reverse_iterator it) {
      return style;
    });
}

void Figure2D::draw_tube(const SlicedTube<IntervalVector>& x, const StyleGradientProperties& style, int max_nb_slices_to_display)
{
  draw_tube_common(*this, x, max_nb_slices_to_display,
    [&style](const Interval& tube_t0tf, std::list<TSlice>::reverse_iterator it) {
      auto c = style.cmap.color((it->mid()-tube_t0tf.lb())/tube_t0tf.diam());
      return StyleProperties({c,c}, style.layer, style.line_style, to_string(style.line_width));
    });
}

template<typename Func>
void plot_tube_common(Figure2D& fig, const SlicedTube<Interval>& x, const StyleProperties& style, const Func& apply_draw_slice)
{
  fig.set_axes(axis(0,x.tdomain()->t0_tf()), axis(1,x.codomain()));
  for(const auto& output_fig : fig.output_figures())
    output_fig->update_axes();

  for(auto it = x.tdomain()->begin() ; it != x.tdomain()->end() ; it++)
  {
    if(!it->is_gate())
    {
      auto xi = x.slice(it);
      apply_draw_slice(*xi,it,style);
    }
  }

  for(const auto& xi : x)
    if(xi.is_gate())
    {
      Vector g1({xi.t0_tf().lb(),xi.codomain().lb()});
      Vector g2({xi.t0_tf().lb(),xi.codomain().ub()});
      fig.draw_point(g1, style.stroke_color);
      fig.draw_point(g2, style.stroke_color);
      fig.draw_line(Segment(g1,g2), StyleProperties(style.stroke_color));
    }
}

void Figure2D::plot_tube(const SlicedTube<Interval>& x, const StyleProperties& style)
{
  plot_tube_common(*this, x, style,
    [this](const Slice<Interval>& xi, [[maybe_unused]] TDomain::iterator it, const StyleProperties& style) {
      this->draw_box(cart_prod(xi.t0_tf(),xi.codomain()), style);
    });
}

void Figure2D::plot_tube(const SlicedTube<Interval>& x, const SlicedTube<Interval>& v, const StyleProperties& style)
{
  plot_tube_common(*this, x, style,
    [this,&v](const Slice<Interval>& xi, TDomain::iterator it, const StyleProperties& style) {
      this->draw_box(cart_prod(xi.t0_tf(),xi.codomain()), Color::light_gray());
      this->draw_polygon(xi.polygon_slice(*v.slice(it)), style);
    });
}

void Figure2D::draw_tank(const Vector& x, float size, const StyleProperties& style)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_tank(x,size,style);
  }
}

void Figure2D::draw_AUV(const Vector& x, float size, const StyleProperties& style)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_AUV(x,size,style);
  }
}

void Figure2D::draw_motor_boat(const Vector& x, float size, const StyleProperties& style)
{
  assert_release(this->size() <= x.size()+1);
  assert_release(size >= 0.);

  for(const auto& output_fig : _output_figures)
  {
    assert_release(output_fig->j()+1 < x.size());
    output_fig->draw_motor_boat(x,size,style);
  }
}

void Figure2D::draw_text(const std::string& text, const Vector& p, double scale, const StyleProperties& style)
{
  assert_release(p.size() == 2);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_text(text,p,scale,style);
}

void Figure2D::draw_raster(const std::string& filename, const IntervalVector& bbox, const StyleProperties& style)
{
  assert_release(bbox.size() == 2);

  for(const auto& output_fig : _output_figures)
    output_fig->draw_raster(filename,bbox,style);
}

void Figure2D::draw_paving(const PavingOut& p, const PavingStyle& style)
{
  draw_paving(p, cartesian_drawing(), style);
}

void Figure2D::draw_paving(const PavingOut& p,
  const function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box_,
  const PavingStyle& style)
{
  p.tree()->left()->visit([&]
    (shared_ptr<const PavingOut_Node> n)
    {
      const IntervalVector& outer = get<0>(n->boxes());

      if(n->top() == p.tree())
        draw_box_(*this, get<0>(n->top()->boxes()), style.outside);

      else
      {
        auto b = get<0>(n->top()->boxes()).bisect_largest();
        IntervalVector hull = n->top()->left() == n ? b.first : b.second;

        for(const auto& bi : hull.diff(outer))
          draw_box_(*this, bi, style.outside);
      }

      if(n->is_leaf())
        draw_box_(*this, outer, style.boundary);

      return true;
    });
}

void Figure2D::draw_paving(const PavingInOut& p, const PavingStyle& style)
{
  draw_paving(p, cartesian_drawing(), style);
}

void Figure2D::draw_paving(const PavingInOut& p,
  const function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box_,
  const PavingStyle& style)
{
  p.tree()->visit([&]
    (shared_ptr<const PavingInOut_Node> n)
    {
      const IntervalVector& outer = get<0>(n->boxes());
      const IntervalVector& inner = get<1>(n->boxes());

      IntervalVector hull = inner | outer;

      for(const auto& bi : hull.diff(inner))
        draw_box_(*this, bi, style.inside);

      for(const auto& bi : hull.diff(outer))
        draw_box_(*this, bi, style.outside);

      if(n->is_leaf())
          draw_box_(*this, inner & outer, style.boundary);

      return true;
    });
}
