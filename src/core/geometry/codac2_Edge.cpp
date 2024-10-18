/** 
 *  codac2_Edge.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Polygon.h"
#include "codac2_geometry.h"

using namespace std;
using namespace codac2;


namespace codac2
{  
  Edge::Edge(std::initializer_list<Vector> vertices)
    : Edge(std::vector<Vector>(vertices))
  { }

  Edge::Edge(const std::vector<Vector>& vertices)
    : std::array<Vector,2>(
      [vertices]
      {
        assert_release(vertices.size() == 2);
        auto it = vertices.begin();
        assert_release(it->size() == 2 && std::next(it)->size() == 2 &&
          "Only 2d vectors are supported for the vertices of an edge");
        return std::array<Vector,2>({ *it, *(++it) });
      }())
  { }

  BoolInterval Edge::intersects(const Edge& e) const
  {
    BoolInterval o1 = orientation((*this)[0], (*this)[1], e[0]);
    BoolInterval o2 = orientation((*this)[0], (*this)[1], e[1]);
    BoolInterval o3 = orientation(e[0], e[1], (*this)[0]);
    BoolInterval o4 = orientation(e[0], e[1], (*this)[1]);

    if(o1 == BoolInterval::EMPTY || o2 == BoolInterval::EMPTY || 
        o3 == BoolInterval::EMPTY || o4 == BoolInterval::EMPTY)
      return BoolInterval::EMPTY;

    else if(o1 == BoolInterval::UNKNOWN || o2 == BoolInterval::UNKNOWN || 
        o3 == BoolInterval::UNKNOWN || o4 == BoolInterval::UNKNOWN)
      return BoolInterval::UNKNOWN;

    else if(o1 != o2 && o3 != o4)
      return BoolInterval::TRUE;

    else
      return BoolInterval::FALSE;
  }
}