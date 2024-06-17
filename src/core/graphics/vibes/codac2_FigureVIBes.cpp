/** 
 *  codac2_FigureVIBes.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_FigureVIBes.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;

int FigureVIBes::_has_been_initialized = 0;

FigureVIBes::FigureVIBes(const Figure& fig)
  : OutputFigure(fig), _params(vibesParams("figure", fig.name()))
{
  if(FigureVIBes::_has_been_initialized == 0)
    vibes::beginDrawing();
  FigureVIBes::_has_been_initialized ++;

  vibes::newFigure(fig.name());
}

FigureVIBes::~FigureVIBes()
{
  FigureVIBes::_has_been_initialized --;
  if(FigureVIBes::_has_been_initialized == 0)
    vibes::endDrawing();
}

void FigureVIBes::update_axes()
{
  vibes::axisLabels(_fig.axes()[0].label, _fig.axes()[1].label, _fig.name());
  vibes::axisLimits(
    _fig.axes()[0].limits.lb(),_fig.axes()[0].limits.ub(),
    _fig.axes()[1].limits.lb(),_fig.axes()[1].limits.ub(),
    _fig.name());
}

void FigureVIBes::update_window_properties()
{
  vibes::setFigureProperties(_fig.name(),
    vibesParams("x", _fig.pos()[0], "y", _fig.pos()[1],
                "width", _fig.window_size()[0], "height", _fig.window_size()[1]));
}

void FigureVIBes::center_viewbox(const Vector& c, const Vector& r)
{
  assert(r.min_coeff() > 0.);
  vibes::axisLimits(c[i()]-r[i()], c[i()]+r[i()], c[j()]-r[j()], c[j()]+r[j()], _fig.name());
}


void FigureVIBes::draw_point(const Vector& c, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  vibes::drawBox(c[i()],c[j()], to_vibes_style(s), _params);
}

void FigureVIBes::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert(_fig.size() <= x.size());
  vibes::drawBox(x[i()].lb(),x[i()].ub(),x[j()].lb(),x[j()].ub(), to_vibes_style(s), _params);
}

void FigureVIBes::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  vibes::drawCircle(c[i()],c[j()],r, to_vibes_style(s), _params);
}

void FigureVIBes::draw_ring(const Vector& c, const Interval& r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(!r.is_empty() && r.lb() > 0.);
  vibes::drawRing(c[i()],c[j()],r.lb(),r.ub(), to_vibes_style(s), _params);
}

void FigureVIBes::draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  vector<double> vx(x.size()), vy(x.size());
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    vx[k] = x[k][i()]; vy[k] = x[k][j()];
  }

  if(tip_length != 0.)
    vibes::drawArrow(vx,vy, tip_length, to_vibes_style(s), _params);
  else
    vibes::drawLine(vx,vy, to_vibes_style(s), _params);
}

void FigureVIBes::draw_polygone(const std::vector<Vector>& x, const StyleProperties& s)
{
  vector<double> vx(x.size()), vy(x.size());
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    vx[k] = x[k][i()]; vy[k] = x[k][j()];
  }

  vibes::drawPolygon(vx,vy, to_vibes_style(s), _params);
}

void FigureVIBes::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  vibes::drawPie(c[i()],c[j()], r.lb(),r.ub(), 180.*theta.lb()/codac2::pi,180.*theta.ub()/codac2::pi, to_vibes_style(s), _params);
}

void FigureVIBes::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  vibes::drawTank(x[i()],x[j()],180.*x[j()+1]/codac2::pi, size, to_vibes_style(s), _params);
}

void FigureVIBes::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  vibes::drawAUV(x[i()],x[j()],180.*x[j()+1]/codac2::pi, size, to_vibes_style(s), _params);
}

string FigureVIBes::to_vibes_style(const StyleProperties& s)
{
  return s.stroke_color.to_hex_str() + "[" + s.fill_color.to_hex_str() + "]";
}