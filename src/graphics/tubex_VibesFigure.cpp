/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <cstdio>
#include <string>
#include <iostream>
#include "ibex_IntervalVector.h"
#include "tubex_VibesFigure.h"

// A real value to display unbounded slices:
#include <limits>
#define BOUNDED_INFINITY 99999. //numeric_limits<float>::max()
// Note: numeric_limits<float>::max() seems unusable with vibes::drawPolygon

using namespace std;
using namespace ibex;

namespace tubex
{
  VibesFigure::VibesFigure(const string& figure_name, int nb_layers)
    : m_view_box(2, Interval::EMPTY_SET), m_nb_layers(nb_layers)
  {
    m_name = figure_name;

    for(int i = 0 ; i < nb_layers ; i++)
    {
      set_current_layer(i);
      vibes::newFigure(name());
      vibes::newGroup("transparent_box", "#ffffffff", vibesParams("figure", name()));
    }
    
    set_current_layer(0);
  }

  const string VibesFigure::name() const
  {
    if(m_nb_layers == 1)
      return m_name;

    else
    {
      std::ostringstream o;
      o << m_name << " (dim " << (m_current_layer + 1) << "/" << m_nb_layers << ")";
      return o.str();
    }
  }

  void VibesFigure::set_properties(int x, int y, int width, int height)
  {
    int current_layer = m_current_layer;
    m_x = x; m_y = y; m_width = width; m_height = height;

    for(int i = m_nb_layers-1 ; i >= 0 ; i--)
    {
      set_current_layer(i);
      vibes::setFigureProperties(
                vibesParams("x", m_x,
                            "y", m_y,
                            "width", m_width,
                            "height", m_height));
    }

    set_current_layer(current_layer);
  }

  const IntervalVector& VibesFigure::axis_limits(double x_min, double x_max, double y_min, double y_max, bool keep_ratio)
  {
    if(keep_ratio && !m_view_box.is_empty())
    {
      float r = m_view_box[0].diam() / m_view_box[1].diam();

      IntervalVector b1(2);
      b1[0] = Interval(x_min, x_max);
      b1[1] = (y_min + y_max)/2. + (b1[0]-b1[0].mid())/r;

      IntervalVector b2(2);
      b2[1] = Interval(y_min, y_max);
      b2[0] = (x_min + x_max)/2. + (b2[1]-b2[1].mid())*r;

      if(b1.is_subset(b2))
        m_view_box = b2;
      else
        m_view_box = b1;
    }

    else
    {
      m_view_box[0] = Interval(x_min, x_max);
      m_view_box[1] = Interval(y_min, y_max);
    }

    vibes::drawBox(m_view_box[0].lb(), m_view_box[0].ub(),
                   m_view_box[1].lb(), m_view_box[1].ub(),
                   vibesParams("figure", name(), "group", "transparent_box"));
    vibes::axisLimits(m_view_box[0].lb(), m_view_box[0].ub(),
                      m_view_box[1].lb(), m_view_box[1].ub(),
                      name());
    return m_view_box;
  }

  const IntervalVector& VibesFigure::axis_limits(const IntervalVector& viewbox, bool keep_ratio)
  {
    return axis_limits(viewbox[0].lb(), viewbox[0].ub(), viewbox[1].lb(), viewbox[1].ub());
  }

  void VibesFigure::save_image(const string& suffix, const string& extension) const
  {
    vibes::saveImage(name() + suffix + extension, name());
  }

  int VibesFigure::x() const
  {
    return m_x;
  }

  int VibesFigure::y() const
  {
    return m_y;
  }

  int VibesFigure::width() const
  {
    return m_width;
  }

  int VibesFigure::height() const
  {
    return m_height;
  }

  const IntervalVector& VibesFigure::view_box() const
  {
    return m_view_box;
  }

  void VibesFigure::draw_box(const IntervalVector& box, const vibes::Params& params)
  {
    draw_box(box, "", params);
  }

  void VibesFigure::draw_box(const IntervalVector& box, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    if(color != "") vibes::drawBox(box, color, params_this_fig);
    else vibes::drawBox(box, params_this_fig);
  }
  
  void VibesFigure::draw_line(const vector<double>& v_x, const vector<double>& v_y, const vibes::Params& params)
  {
    draw_line(v_x, v_y, "", params);
  }
  
  void VibesFigure::draw_line(const vector<double>& v_x, const vector<double>& v_y, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawLine(v_x, v_y, params_this_fig);
  }
  
  void VibesFigure::draw_circle(double x, double y, double r, const vibes::Params& params)
  {
    draw_circle(x, y, r, "", params);
  }
  
  void VibesFigure::draw_circle(double x, double y, double r, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vibes::drawCircle(x, y, r, params_this_fig);
  }
  
  void VibesFigure::draw_polygon(const Polygon& p, const vibes::Params& params)
  {
    draw_polygon(p, "", params);
  }
  
  void VibesFigure::draw_polygon(const Polygon& p, const string& color, const vibes::Params& params)
  {
    vibes::Params params_this_fig(params);
    params_this_fig["figure"] = name();
    vector<double> v_t, v_x;

    for(int i = 0 ; i < p.nb_vertices() ; i++)
    {
      v_t.push_back(trunc_inf(p[i].t().mid()));
      v_x.push_back(trunc_inf(p[i].x().mid()));
    } 

    if(v_t.size() > 0)
      vibes::drawPolygon(v_t, v_x, params_this_fig);
  }

  const string VibesFigure::rgb2hex(int r, int g, int b, int alpha, const char* prefix)
  {
    string result;
    result.append(prefix);
   
    char char_r[255];
    sprintf(char_r, "%.2X", r);
    result.append(char_r);
    char char_g[255];
    sprintf(char_g, "%.2X", g);
    result.append(char_g);
    char char_b[255];
    sprintf(char_b, "%.2X", b);
    result.append(char_b);

    if(alpha != -1)
    {
      char char_alpha[255];
      sprintf(char_alpha, "%.2X", alpha);
      result.append(char_alpha);
    }

    return result;
  }

  /*void VibesFigure::hex2rgb(const string& hexa, int &r, int &g, int &b, int &alpha)
  {
    string hexa_min = hexa;
    if(hexa.find("#") != string::npos)
      hexa_min = hexa.substr(1,hexa.length()-1);

    if(hexa_min.length() < 6)
      cout << "VibesFigure::hex2rgb: error, invalid hexa value \"" << hexa << "\"" << endl;

    r = std::stoi(hexa_min.substr(0,2).c_str(), NULL, 16);
    g = std::stoi(hexa_min.substr(2,2).c_str(), NULL, 16);
    b = std::stoi(hexa_min.substr(4,2).c_str(), NULL, 16);

    if(hexa_min.length() > 7)
      alpha = std::stoi(hexa_min.substr(6,2).c_str(), NULL, 16);
    else
      alpha = 0;
  }*/

  string VibesFigure::add_suffix(const string& name, int id)
  {
    std::ostringstream o;
    o << name << "_" << id;
    return o.str();
  }

  void VibesFigure::set_current_layer(int layer)
  {
    m_current_layer = layer;
    vibes::selectFigure(name());
  }

  double VibesFigure::trunc_inf(double x)
  {
    return (x == POS_INFINITY ? BOUNDED_INFINITY : (x == NEG_INFINITY ? -BOUNDED_INFINITY : x));
  }

  const Interval VibesFigure::trunc_inf(const ibex::Interval& x)
  {
    return Interval(trunc_inf(x.lb()),trunc_inf(x.ub()));
  }
}