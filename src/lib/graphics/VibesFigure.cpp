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
  vibes::setFigureProperties(
            vibesParams("figure", m_name,
                        "x", x,
                        "y", y,
                        "width", width,
                        "height", height));
}

void VibesFigure::saveSVG() const
{
  vibes::saveImage(m_name + ".svg", m_name);
}