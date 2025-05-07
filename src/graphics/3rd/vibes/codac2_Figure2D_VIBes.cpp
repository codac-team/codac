/** 
 *  codac2_Figure2D_VIBes.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure2D_VIBes.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;

int Figure2D_VIBes::_has_been_initialized = 0;

Figure2D_VIBes::Figure2D_VIBes(const Figure2D& fig)
  : OutputFigure2D(fig), _params(vibesParams("figure", fig.name(), "LineStyle", "-"))
{
  if(Figure2D_VIBes::_has_been_initialized == 0)
    vibes::beginDrawing();
  Figure2D_VIBes::_has_been_initialized ++;

  vibes::newFigure(fig.name());
  vibes::newGroup("alpha","");
}

Figure2D_VIBes::~Figure2D_VIBes()
{
  Figure2D_VIBes::_has_been_initialized --;
  if(Figure2D_VIBes::_has_been_initialized == 0)
    vibes::endDrawing();
}

void Figure2D_VIBes::update_axes()
{
  vibes::axisLabels(_fig.axes()[0].label, _fig.axes()[1].label, _fig.name());
  vibes::axisLimits(
    _fig.axes()[0].limits.lb(),_fig.axes()[0].limits.ub(),
    _fig.axes()[1].limits.lb(),_fig.axes()[1].limits.ub(),
    _fig.name());
}

void Figure2D_VIBes::update_window_properties()
{
  vibes::setFigureProperties(_fig.name(),
    vibesParams("x", _fig.pos()[0], "y", _fig.pos()[1],
                "width", _fig.window_size()[0], "height", _fig.window_size()[1]));
}

void Figure2D_VIBes::center_viewbox(const Vector& c, const Vector& r)
{
  assert(_fig.size() <= c.size() && _fig.size() <= r.size());
  assert(r.min_coeff() > 0.);
  vibes::axisLimits(c[i()]-r[i()], c[i()]+r[i()], c[j()]-r[j()], c[j()]+r[j()], _fig.name());
}

void Figure2D_VIBes::draw_point(const Vector& c, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawPoint(c[i()],c[j()],2, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert(_fig.size() <= x.size());

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawBox(x[i()].lb(),x[i()].ub(),x[j()].lb(),x[j()].ub(), to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(r > 0.);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawCircle(c[i()],c[j()],r, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_ring(const Vector& c, const Interval& r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(!r.is_empty() && r.lb() >= 0.);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawRing(c[i()],c[j()],r.lb(),r.ub(), to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  assert(x.size() > 1);
  assert(tip_length >= 0.);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vector<double> vx(x.size()), vy(x.size());
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    assert(_fig.size() <= x[k].size());
    vx[k] = x[k][i()]; vy[k] = x[k][j()];
  }

  if(tip_length != 0.)
    vibes::drawArrow(vx,vy, tip_length, to_vibes_style(s), _params);
  else
    vibes::drawLine(vx,vy, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_polygon(const std::vector<Vector>& x, const StyleProperties& s)
{
  assert(x.size() > 1);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;
  
  vector<double> vx(x.size()), vy(x.size());
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    assert(_fig.size() <= x[k].size());
    vx[k] = x[k][i()]; vy[k] = x[k][j()];
  }

  vibes::drawPolygon(vx,vy, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(r.lb() >= 0.);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  // Corrected a bug in VIBEs in case of r=[..,oo] (the pie disappears when zoomed in)
  vibes::drawPie(c[i()],c[j()], r.lb(),(r.ub()>1e5?1e5:r.ub()), 180.*theta.lb()/PI,180.*theta.ub()/PI, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s)
{
  assert(c.size() == 2);
  assert(ab.size() == 2);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawEllipse(c[0],c[1], ab[0],ab[1], 180.*theta/PI, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawTank(x[i()],x[j()],180.*x[j()+1]/PI, size, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawAUV(x[i()],x[j()],180.*x[j()+1]/PI, size, to_vibes_style(s), _params);
}

void Figure2D_VIBes::draw_motor_boat(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end())
    {
      vibes::newGroup(s.layer,"");
      _layers.push_back(s.layer);
    }

  _params["LineStyle"] = s.line_style;
  _params["group"] = s.layer;

  vibes::drawMotorBoat(x[i()],x[j()],180.*x[j()+1]/PI, size, to_vibes_style(s), _params);
}

std::string Figure2D_VIBes::to_vibes_style(const StyleProperties& s)
{
  return s.stroke_color.hex_str() + "[" + s.fill_color.hex_str() + "]";
}