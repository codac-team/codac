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

using namespace std;
using namespace codac2;

Figure *DefaultFigure::_default_figure = nullptr;

Figure::Figure(const std::string& name, GraphicOutputMode o)
  : _name(name)
{
  DefaultFigure::set(this);
  if(o & GraphicOutputMode::VIBES)
    _output_figures.push_back(std::make_shared<FigureVIBes>(*this));
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

void Figure::set_as_default()
{
  DefaultFigure::set(this);
}

void Figure::set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
{
  _axes.clear();
  _axes.push_back(axis1);
  _axes.push_back(axis2);
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

void Figure::draw_box(const IntervalVector& x, const std::string& color)
{
  for(const auto& output_fig : _output_figures)
    output_fig->draw_box(x,color);
}