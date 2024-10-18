/** 
 *  codac2_Polygon.cpp
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
  Polygon::Polygon(initializer_list<Vector> vertices)
    : Polygon(std::vector<Vector>(vertices))
  { }

  Polygon::Polygon(const std::vector<Vector>& vertices)
    : _edges(
      [vertices]
      {
        assert_release(vertices.size() > 1);
        vector<Edge> edges;

        size_t i = 0;
        for(const auto& vi : vertices)
        {
          assert_release(vi.size() == 2);

          if(i == 0)
            edges.push_back({ vi, vi });

          else
            edges.push_back({ edges[i-1][1], vi });

          i++;
        }

        edges[0][0] = edges[edges.size()-1][1]; // closing the polygon
        return edges;
      }())
  { }

  Polygon::Polygon(initializer_list<Edge> edges)
    : _edges(edges)
  { }

  Polygon::Polygon(const vector<Edge>& edges)
    : _edges(edges)
  { }
  
  const vector<Edge>& Polygon::edges() const
  {
    return _edges;
  }

  BoolInterval Polygon::contains(const Vector& p) const
  {
    assert_release(p.size() == 2);

    size_t i = 0;
    Edge transect { Vector({next_float(-oo),p[1]}), p };

    for(const auto& edge_k : _edges)
    {
      switch(transect.intersects({edge_k[0],edge_k[1]}))
      {
        case BoolInterval::TRUE:
          i++;
          break;

        case BoolInterval::FALSE:
          // no intersection
          break;

        case BoolInterval::EMPTY:
          assert(false && "BoolInterval::EMPTY should not happen");
          break;

        case BoolInterval::UNKNOWN: // case of colinear edges
        {
          if((IntervalVector(transect[0]) | transect[1]).intersects(IntervalVector(edge_k[0]) | edge_k[1]))
            return BoolInterval::UNKNOWN;
          
          else
          {
            // no intersection
          }
        }
      }
    }

    return (i%2 == 0) ? BoolInterval::FALSE : BoolInterval::TRUE;
  }
}