/** 
 *  Figure class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Figure.h"

// A real value to display unbounded slices:
#include <limits>
#define BOUNDED_INFINITY 99999. //numeric_limits<float>::max()
// Note: numeric_limits<float>::max() seems unusable with vibes::drawPolygon

using namespace std;
using namespace ibex;

namespace codac
{
  Figure::Figure(const string& fig_name) : m_name(fig_name)
  {
    
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

  const string Figure::name() const
  {
    return m_name;
  }

  const IntervalVector& Figure::view_box() const
  {
    return m_view_box;
  }

  void Figure::reset_view_box()
  {
    m_view_box = IntervalVector(2, Interval::EMPTY_SET);
  }

  void Figure::set_properties(int x, int y, int width, int height)
  {
    assert(x >= 0 && y >= 0 && width > 0 && height > 0);
    m_x = x; m_y = y;
    m_width = width; m_height = height;
  }

  double Figure::trunc_inf(double x)
  {
    return (x == POS_INFINITY ? BOUNDED_INFINITY : (x == NEG_INFINITY ? -BOUNDED_INFINITY : x));
  }

  const Interval Figure::trunc_inf(const Interval& x)
  {
    return Interval(trunc_inf(x.lb()), trunc_inf(x.ub()));
  }

  const IntervalVector Figure::trunc_inf(const IntervalVector& x)
  {
    IntervalVector trunc_x = x;
    for(int i = 0 ; i < trunc_x.size() ; i++)
      trunc_x[i] = trunc_inf(trunc_x[i]);
    return trunc_x;
  }
}