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

#ifndef Edge_HEADER
#define Edge_HEADER

#include "tubex_Point.h"

namespace tubex
{
  class Edge
  {
    public:

      Edge(Point p1, Point p2);
  };
}

#endif