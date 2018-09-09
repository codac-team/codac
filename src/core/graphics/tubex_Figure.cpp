/* ============================================================================
 *  tubex-lib - Figure class
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
#include <sstream>
#include "tubex_Figure.h"

// A real value to display unbounded slices:
#include <limits>
#define BOUNDED_INFINITY 99999. //numeric_limits<float>::max()
// Note: numeric_limits<float>::max() seems unusable with vibes::drawPolygon

using namespace std;
using namespace ibex;

namespace tubex
{
  Figure::Figure(const string& figure_name) : m_name(figure_name)
  {
    
  }

  const string Figure::name() const
  {
    return m_name;
  }

  void Figure::set_properties(int x, int y, int width, int height)
  {
    assert(x >= 0 && y >= 0 && width > 0 && height > 0);
    m_x = x; m_y = y;
    m_width = width; m_height = height;
  }

  int Figure::x() const
  {
    return m_x;
  }

  int Figure::y() const
  {
    return m_y;
  }

  int Figure::width() const
  {
    return m_width;
  }

  int Figure::height() const
  {
    return m_height;
  }

  string Figure::add_suffix(const string& name, int id)
  {
    ostringstream o;
    o << name << "_" << id;
    return o.str();
  }

  double Figure::trunc_inf(double x)
  {
    return (x == POS_INFINITY ? BOUNDED_INFINITY : (x == NEG_INFINITY ? -BOUNDED_INFINITY : x));
  }

  const Interval Figure::trunc_inf(const ibex::Interval& x)
  {
    return Interval(trunc_inf(x.lb()),trunc_inf(x.ub()));
  }
}