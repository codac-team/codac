/** 
 *  codac2_Figure2D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure2D.h"
#include "codac2_Figure2D_VIBes.h"
#include "codac2_Figure2D_IPE.h"
#include "codac2_math.h"
#include "codac2_pave.h"

using namespace std;
using namespace codac2;

shared_ptr<Figure2D> DefaultView::_default_fig = nullptr;
shared_ptr<Figure2D> DefaultView::_selected_fig = DefaultView::_default_fig;

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

size_t Figure2D::size() const
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

bool Figure2D::is_default() const
{
  return DefaultView::_selected_fig == this->weak_from_this().lock();
}

void Figure2D::set_as_default()
{
  DefaultView::set(this->shared_from_this());
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

void Figure2D::draw_polyline(const vector<Vector>& x, const StyleProperties& s)
{
  draw_polyline(x, 1e-3*scaled_unit(), s);
}

void Figure2D::draw_polyline(const vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  assert_release(x.size() > 1);
  assert_release(tip_length >= 0.); // 0 = disabled tip
  for(const auto& xi : x)
  {
    assert_release(this->size() <= xi.size());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polyline(x,tip_length,s);
}

void Figure2D::draw_polygone(const vector<Vector>& x, const StyleProperties& s)
{
  assert_release(x.size() > 1);
  for(const auto& xi : x)
  {
    assert_release(this->size() <= xi.size());
  }

  for(const auto& output_fig : _output_figures)
    output_fig->draw_polygone(x,s);
}

void Figure2D::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert_release(this->size() <= c.size());
  assert_release(r.lb() >= 0.);

  if(r.is_empty() || theta.is_empty())
    return;

  Interval theta_(theta);
  if(theta.is_unbounded())
    theta_ = Interval(0,2.*codac2::pi);

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

void Figure2D::draw_paving(const PavingOut& p,
  const StyleProperties& boundary_style,const StyleProperties& outside_style)
{
  for(const auto& output_fig : _output_figures)
  {
    p.tree()->left()->visit([&]
      (std::shared_ptr<const PavingOut_Node> n)
      {
        const IntervalVector& outer = get<0>(n->boxes());

        if(n->top() == p.tree())
          output_fig->draw_box(get<0>(n->top()->boxes()), outside_style);

        else
        {
          auto p = get<0>(n->top()->boxes()).bisect_largest();
          IntervalVector hull = n->top()->left() == n ? p.first : p.second;

          for(const auto& bi : hull.diff(outer))
            output_fig->draw_box(bi, outside_style);
        }

        if(n->is_leaf())
          output_fig->draw_box(outer, boundary_style);

        return true;
      });
  }
}

void Figure2D::draw_paving(const PavingInOut& p, const StyleProperties& boundary_style,
  const StyleProperties& outside_style, const StyleProperties& inside_style)
{
  for(const auto& output_fig : _output_figures)
  {
    p.tree()->visit([&]
      (std::shared_ptr<const PavingInOut_Node> n)
      {
        const IntervalVector& outer = get<0>(n->boxes());
        const IntervalVector& inner = get<1>(n->boxes());

        IntervalVector hull = inner | outer;

        for(const auto& bi : hull.diff(inner))
          output_fig->draw_box(bi, inside_style);

        for(const auto& bi : hull.diff(outer))
          output_fig->draw_box(bi, outside_style);
        
        if(n->is_leaf())
          output_fig->draw_box(inner & outer, boundary_style);

        return true;
      });
  }
}