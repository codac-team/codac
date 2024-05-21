/** 
 *  FigureVIBes classes
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Figure.h"
#include "codac2_FigureVIBes.h"
#include "codac2_FigureIPE.h"

using namespace std;
using namespace codac2;

shared_ptr<Figure> DefaultFigure::_default_fig = nullptr;
Figure *DefaultFigure::_selected_fig = DefaultFigure::_default_fig.get();

Figure::Figure(const std::string& name, GraphicOutputMode o, bool set_as_default_)
  : _name(name)
{
  if(o & GraphicOutputMode::VIBES)
    _output_figures.push_back(std::make_shared<FigureVIBes>(*this));
  if(o & GraphicOutputMode::IPE)
    _output_figures.push_back(std::make_shared<FigureIPE>(*this));
  if(set_as_default_)
    set_as_default();
}

size_t Figure::size() const
{
  return _axes.size();
}

const Vector& Figure::pos() const
{
  return _pos;
}

const Vector& Figure::window_size() const
{
  return _window_size;
}

const std::string& Figure::name() const
{
  return _name;
}

const std::vector<FigureAxis>& Figure::axes() const
{
  return _axes;
}

bool Figure::is_default() const
{
  return DefaultFigure::selected_fig() == this;
}

void Figure::set_as_default()
{
  DefaultFigure::set(this);
}

void Figure::set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
{
  _axes = { axis1, axis2 };
  for(const auto& output_fig : _output_figures)
    output_fig->update_axes();
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

void Figure::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_box(x,s);
}