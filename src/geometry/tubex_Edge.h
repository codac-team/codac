/* ============================================================================
 *  tubex-lib - Edge structure
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_EDGE_H__
#define __TUBEX_EDGE_H__

#include "tubex_Point.h"

namespace tubex
{
  class Edge
  {
    public:

      Edge(const Point& p1, const Point& p2);
      const ibex::IntervalVector box() const;
      const ibex::IntervalVector operator&(const ibex::IntervalVector& box) const;
      bool operator==(const Edge& e) const;
      bool operator!=(const Edge& e) const;
      const Point operator&(const Edge& e) const;
      friend std::ostream& operator<<(std::ostream& str, const Edge& e);

    protected:

      Point m_p1, m_p2;
  };

  void pushEdges(const ibex::IntervalVector& box, std::vector<Edge>& v_edges);
}

#endif