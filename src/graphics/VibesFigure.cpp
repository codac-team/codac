/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU General Public License (GPL). See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "VibesFigure.h"

using namespace std;

VibesFigure::VibesFigure(const string& figure_name)
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