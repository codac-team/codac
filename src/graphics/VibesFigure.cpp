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
#include "VibesFigure.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  VibesFigure::VibesFigure(const string& figure_name) : m_view_box(2, Interval::EMPTY_SET)
  {
    m_name = figure_name;
    vibes::newFigure(m_name);
  }

  string VibesFigure::name() const
  {
    return m_name;
  }

  void VibesFigure::setProperties(int x, int y, int width, int height)
  {
    m_x = x; m_y = y; m_width = width; m_height = height;
    vibes::setFigureProperties(
              vibesParams("figure", m_name,
                          "x", m_x,
                          "y", m_y,
                          "width", m_width,
                          "height", m_height));
  }

  const IntervalVector& VibesFigure::axisLimits(double x_min, double x_max, double y_min, double y_max, bool keep_ratio)
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

    vibes::axisLimits(m_view_box[0].lb(), m_view_box[0].ub(),
                      m_view_box[1].lb(), m_view_box[1].ub(),
                      m_name);
    return m_view_box;
  }

  void VibesFigure::saveImage(const string& suffix, const string& extension) const
  {
    vibes::saveImage(m_name + suffix + extension, m_name);
  }

  double VibesFigure::x() const
  {
    return m_x;
  }

  double VibesFigure::y() const
  {
    return m_y;
  }

  double VibesFigure::width() const
  {
    return m_width;
  }

  double VibesFigure::height() const
  {
    return m_height;
  }

  string VibesFigure::rgb2hex(int r, int g, int b, int alpha, const char* prefix)
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
}