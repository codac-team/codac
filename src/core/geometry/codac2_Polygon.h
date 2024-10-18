/** 
 *  \file codac2_Polygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <utility>
#include "codac2_IntervalVector.h"
#include "codac2_BoolInterval.h"
#include "codac2_Edge.h"

namespace codac2
{
  class Polygon
  {
    public:

      Polygon(std::initializer_list<Vector> vertices);
      Polygon(const std::vector<Vector>& vertices);
      Polygon(std::initializer_list<Edge> edges);
      Polygon(const std::vector<Edge>& edges);

      const std::vector<Edge>& edges() const;
      BoolInterval contains(const Vector& p) const;

    protected:

      const std::vector<Edge> _edges;
  };
}