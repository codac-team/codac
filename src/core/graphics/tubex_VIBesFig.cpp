/** 
 *  VIBesFig class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_VIBesFig.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  VIBesFig::VIBesFig(const string& fig_name)
    : Figure(fig_name)
  {
    vibes::newFigure(m_name);
    vibes::newGroup("transparent_box", "#ffffff00", vibesParams("figure", m_name));
  }

  VIBesFig::~VIBesFig()
  {

  }

  void VIBesFig::close()
  {
    vibes::closeFigure(name());
  }

  void VIBesFig::set_properties(int x, int y, int width, int height)
  {
    assert(x >= 0 && y >= 0 && width > 0 && height > 0);
    Figure::set_properties(x, y, width, height);
    vibes::setFigureProperties(
              name(),
              vibesParams("x", m_x,
                          "y", m_y,
                          "width", m_width,
                          "height", m_height));
  }

  void VIBesFig::set_background(const string& bg_color)
  {
    // todo: do better than this (possible with VIBes?)
    vibes::drawBox(IntervalVector(2,Interval(-999999999.,999999999.)),
                   "[" + bg_color + "]",
                   vibesParams("figure", name()));
  }

  const IntervalVector& VIBesFig::axis_limits(double x_min, double x_max, double y_min, double y_max, bool keep_ratio, float margin)
  {
    assert(margin >= 0.);
    assert(x_min < x_max && y_min < y_max);

    IntervalVector viewbox(2);
    viewbox[0] = Interval(x_min, x_max);
    viewbox[1] = Interval(y_min, y_max);
    return axis_limits(viewbox, keep_ratio, margin);
  }

  const IntervalVector& VIBesFig::axis_limits(const IntervalVector& viewbox, bool same_ratio, float margin)
  {
    assert(viewbox.size() == 2);
    assert(margin >= 0.);

    if(same_ratio && !m_view_box.is_empty())
    {
      float r = 1. * width() / height();

      IntervalVector b1(2);
      b1[0] = viewbox[0];
      b1[1] = viewbox[1].mid() + Interval(-1.,1.) * b1[0].rad() / r;

      IntervalVector b2(2);
      b2[1] = viewbox[1];
      b2[0] = viewbox[0].mid() + Interval(-1.,1.) * b2[1].rad() * r;

      m_view_box = b1 | b2;
    }

    else
      m_view_box = viewbox;

    IntervalVector box_with_margin = m_view_box;
    box_with_margin[0] += margin * m_view_box[0].diam() * Interval(-1.,1.);
    box_with_margin[1] += margin * m_view_box[1].diam() * Interval(-1.,1.);

    vibes::clearGroup(name(), "transparent_box");
    vibes::drawBox(box_with_margin, vibesParams("figure", name(), "group", "transparent_box"));
    vibes::axisLimits(box_with_margin[0].lb(), box_with_margin[0].ub(),
                      box_with_margin[1].lb(), box_with_margin[1].ub(),
                      name());
    return m_view_box;
  }

  void VIBesFig::save_image(const string& suffix, const string& extension, const string& path) const
  {
    vibes::saveImage(path + "/" + name() + suffix + "." + extension, name());
  }
  
  void VIBesFig::clear()
  {
    vibes::clearFigure(name());
  }

  void VIBesFig::draw_box(const IntervalVector& box, const vibes::Params& params)
  {
    assert(box.size() == 2);
    draw_box(box, "", params);
  }

  void VIBesFig::draw_box(const IntervalVector& box, const string& color, const vibes::Params& params)
  {
    assert(box.size() == 2);

    if(box.is_unbounded())
      return;

    if(box.max_diam() == 0.)
      draw_point(Point(box), color, params);

    else
    {
      vibes::Params params_this_fig(params);
      m_view_box |= box;
      params_this_fig["figure"] = name();

      if(color != "")
        vibes::drawBox(box, color, params_this_fig);
      
      else
        vibes::drawBox(box, params_this_fig);
    }
  }
  
  void VIBesFig::draw_line(const vector<vector<double> >& v_pts, const vibes::Params& params)
  {
    for(size_t i = 0 ; i < v_pts.size() ; i++)
      assert(v_pts[i].size() == 2);
    draw_line(v_pts, "", params);
  }
  
  void VIBesFig::draw_line(const vector<vector<double> >& v_pts, const string& color, const vibes::Params& params)
  {
    for(size_t i = 0 ; i < v_pts.size() ; i++)
      assert(v_pts[i].size() == 2);
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawLine(v_pts, color, params_this_fig);
  }
  
  void VIBesFig::draw_line(const vector<double>& v_x, const vector<double>& v_y, const vibes::Params& params)
  {
    assert(v_x.size() == v_y.size());
    draw_line(v_x, v_y, "", params);
  }
  
  void VIBesFig::draw_line(const vector<double>& v_x, const vector<double>& v_y, const string& color, const vibes::Params& params)
  {
    assert(v_x.size() == v_y.size());
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawLine(v_x, v_y, color, params_this_fig);
  }
  
  void VIBesFig::draw_circle(double x, double y, double r, const vibes::Params& params)
  {
    draw_circle(x, y, r, "", params);
  }
  
  void VIBesFig::draw_circle(double x, double y, double r, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawCircle(x, y, r, color, params_this_fig);
  }
  
  void VIBesFig::draw_ring(double x, double y, const Interval& r, const vibes::Params& params)
  {
    draw_ring(x, y, r, "", params);
  }
  
  void VIBesFig::draw_ring(double x, double y, const Interval& r, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawCircle(x, y, r.lb(), color, params_this_fig);
    vibes::drawCircle(x, y, r.ub(), color, params_this_fig);
  }
  
  void VIBesFig::draw_pie(double x, double y, const Interval& r, const Interval& theta, const vibes::Params& params)
  {
    draw_pie(x, y, r, theta, "", params);
  }
  
  void VIBesFig::draw_pie(double x, double y, const Interval& r, const Interval& theta, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();

    // Corrected bug in case of r.lb() == 0 (only one edge of the pie is drawn)
    double near_zero = 0.0000001;
    vibes::drawPie(x, y, (fabs(r.lb()) <= near_zero ? near_zero : r.lb()), r.ub(), theta.lb() * 180. / M_PI, theta.ub() * 180. / M_PI, color, params_this_fig);
  }
  
  void VIBesFig::draw_edge(const Edge& e, const vibes::Params& params)
  {
    draw_edge(e, "", params);
  }
  
  void VIBesFig::draw_edge(const Edge& e, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();

    vector<double> v_x, v_y;
    v_x.push_back(e.p1()[0].mid()); v_x.push_back(e.p2()[0].mid());
    v_y.push_back(e.p1()[1].mid()); v_y.push_back(e.p2()[1].mid());
    vibes::drawLine(v_x, v_y, color, params_this_fig);
    m_view_box |= e.p1().box() | e.p2().box();
  }
  
  void VIBesFig::draw_polygon(const Polygon& p, const vibes::Params& params)
  {
    draw_polygon(p, "", params);
  }
  
  void VIBesFig::draw_polygon(const Polygon& p, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vector<double> v_x, v_y;

    for(int i = 0 ; i < p.nb_vertices() ; i++)
    {
      v_x.push_back(trunc_inf(p[i][0]));
      v_y.push_back(trunc_inf(p[i][1]));
    }

    if(v_x.size() > 0)
    {
      vibes::drawPolygon(v_x, v_y, color, params_this_fig);
      m_view_box |= p.box();
    }
  }

  void VIBesFig::draw_point(const Point& p, float size, const vibes::Params& params)
  {
    assert(!p.does_not_exist());
    Point inflated_pt = p;
    inflated_pt.inflate(size);
    draw_point(inflated_pt, "", params);
  }

  void VIBesFig::draw_point(const Point& p, float size, const string& color, const vibes::Params& params)
  {
    assert(!p.does_not_exist());
    Point inflated_pt = p;
    inflated_pt.inflate(size);
    draw_point(inflated_pt, color, params);
  }

  void VIBesFig::draw_point(const Point& p, const vibes::Params& params)
  {
    assert(!p.does_not_exist());
    draw_point(p, "", params);
  }

  void VIBesFig::draw_point(const Point& p, const string& color, const vibes::Params& params)
  {
    assert(!p.does_not_exist());
    vibes::Params params_this_fig(params);
    m_view_box |= p.box();
    params_this_fig["figure"] = name();

    if(p.x().is_degenerated() && p.y().is_degenerated())
      vibes::drawPoint(p.x().lb(), p.y().lb(), 1, color, params_this_fig);

    else
      draw_box(trunc_inf(p.box()), color, params_this_fig);
  }
  
  void VIBesFig::draw_points(const vector<Point>& v_pts, float size, const vibes::Params& params)
  {
    draw_points(v_pts, size, "", params);
  }
  
  void VIBesFig::draw_points(const vector<Point>& v_pts, float size, const string& color, const vibes::Params& params)
  {
    for(size_t i = 0 ; i < v_pts.size() ; i++)
      draw_point(v_pts[i], size, color, params);
  }
}