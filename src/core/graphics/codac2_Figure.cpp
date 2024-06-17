/** 
 *  codac2_Figure.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure.h"
#include "codac2_FigureVIBes.h"
#include "codac2_FigureIPE.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;

shared_ptr<Figure> DefaultFigure::_default_fig = nullptr;
Figure *DefaultFigure::_selected_fig = DefaultFigure::_default_fig.get();

Figure::Figure(const std::string& name, GraphicOutput o, bool set_as_default_)
  : _name(name)
{
  if(o & GraphicOutput::VIBES)
    _output_figures.push_back(std::make_shared<FigureVIBes>(*this));
  if(o & GraphicOutput::IPE)
    _output_figures.push_back(std::make_shared<FigureIPE>(*this));
  if(set_as_default_)
    set_as_default();
}

const std::string& Figure::name() const
{
  return _name;
}

size_t Figure::size() const
{
  return _axes.size();
}

const std::vector<FigureAxis>& Figure::axes() const
{
  return _axes;
}

void Figure::set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
{
  _axes = { axis1, axis2 };
  for(const auto& output_fig : _output_figures)
    output_fig->update_axes();
}

const Vector& Figure::pos() const
{
  return _pos;
}

const Vector& Figure::window_size() const
{
  return _window_size;
}

void Figure::set_window_properties(const Vector& pos, const Vector& size)
{
  assert(pos.size() == 2 && size.size() == 2);
  assert(size.min_coeff() > 0);

  _pos = pos; _window_size = size;
  for(const auto& output_fig : _output_figures)
    output_fig->update_window_properties();
}

void Figure::center_viewbox(const Vector& c, const Vector& r)
{
  for(const auto& output_fig : _output_figures)
    output_fig->center_viewbox(c,r);
}

double Figure::scaled_unit() const
{
  return std::max(_axes[0].limits.diam(),_axes[1].limits.diam()) / _window_size.max_coeff();
}

bool Figure::is_default() const
{
  return DefaultFigure::_selected_fig == this;
}

void Figure::set_as_default()
{
  DefaultFigure::set(this);
}

void Figure::draw_point(const Vector& c, const StyleProperties& s)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_point(c,s);
}

void Figure::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_box(x,s);
}

void Figure::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  assert(r > 0.);
  for(const auto& output_fig : _output_figures)
    output_fig->draw_circle(c,r,s);
}

void Figure::draw_ring(const Vector& c, const Interval& r, const StyleProperties& s)
{
  assert(!r.is_empty() && r.lb() > 0.);
  for(const auto& output_fig : _output_figures)
    output_fig->draw_ring(c,r,s);
}

void Figure::draw_polyline(const vector<Vector>& x, const StyleProperties& s)
{
  draw_polyline(x, 1e-3*scaled_unit(), s);
}

void Figure::draw_polyline(const vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  assert(tip_length >= 0.); // 0 = disabled tip
  for(const auto& output_fig : _output_figures)
    output_fig->draw_polyline(x,tip_length,s);
}

void Figure::draw_polygone(const vector<Vector>& x, const StyleProperties& s)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_polygone(x,s);
}

void Figure::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert(!r.is_empty() && !theta.is_empty());
  assert(r.lb() >= 0.);

  Interval theta_(theta);
  if(theta.is_unbounded())
    theta_ = Interval(0,2.*codac2::pi);

  Interval r_(r);
  if(r.is_unbounded())
    r_ &= Interval(0,previous_float(oo));

  for(const auto& output_fig : _output_figures)
    output_fig->draw_pie(c,r_,theta_,s);
}

void Figure::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert(size > 0.);
  for(const auto& output_fig : _output_figures)
    output_fig->draw_tank(x,size,s);
}

void Figure::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert(size > 0.);
  for(const auto& output_fig : _output_figures)
    output_fig->draw_AUV(x,size,s);
}