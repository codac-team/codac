/** 
 *  \file codac2_Edge.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <utility>
#include "codac2_Vector.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  class Edge : public std::array<Vector,2>
  {
    public:

      Edge(std::initializer_list<Vector> vertices);
      Edge(const std::vector<Vector>& vertices);
      BoolInterval intersects(const Edge& e) const;
  };
}