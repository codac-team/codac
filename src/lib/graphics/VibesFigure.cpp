/* ============================================================================
 *  IBEX-Robotics - VibesFigure class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
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

void VibesFigure::saveSVG() const
{
  vibes::saveImage(m_name + ".svg", m_name);
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