/** 
 *  codac2_ConvexPolygon.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include "codac2_ConvexPolygon.h"
#include "codac2_geometry.h"
#include "codac2_template_tools.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  ConvexPolygon::ConvexPolygon()
  { }
  
  ConvexPolygon::ConvexPolygon(std::initializer_list<Vector> vertices)
    : ConvexPolygon(std::vector<Vector>(vertices))
  { }

  ConvexPolygon::ConvexPolygon(const std::vector<Vector>& vertices, bool compute_convex_hull)
    : ConvexPolygon(vectorVector_to_vectorIntervalVector(vertices),compute_convex_hull)
  { }

  ConvexPolygon::ConvexPolygon(const std::vector<IntervalVector>& vertices, bool compute_convex_hull)
    : Polygon(compute_convex_hull ? convex_hull(vertices) : vertices)
  { }

  ConvexPolygon::ConvexPolygon(std::initializer_list<Segment> edges)
    : ConvexPolygon(std::vector<Segment>(edges))
  { }

  ConvexPolygon::ConvexPolygon(const vector<Segment>& edges)
    : Polygon([edges] {
      auto l = Polygon(edges).vertices();
      return convex_hull({ l.begin(), l.end() });
    }())
  { }

  ConvexPolygon::ConvexPolygon(const IntervalVector& x)
    : Polygon(x)
  { }
  
  ConvexPolygon ConvexPolygon::empty()
  {
    return ConvexPolygon();
  }
  
  ConvexPolygon& ConvexPolygon::operator&=(const ConvexPolygon& p)
  {
    *this = *this & p;
    return *this;
  }
  
  ConvexPolygon& ConvexPolygon::operator|=(const ConvexPolygon& p)
  {
    vector<IntervalVector> v, pv = p.vertices(), pthis = this->vertices();
    v.reserve(pv.size() + pthis.size());
    v.insert(v.end(), pv.begin(), pv.end());
    v.insert(v.end(), pthis.begin(), pthis.end());
    *this = ConvexPolygon(v, true);
    return *this;
  }

  ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2)
  {
    vector<IntervalVector> inter;

    if(p1.is_empty() || p2.is_empty())
      return ConvexPolygon::empty();

    auto v1 = p1.vertices();
    for(const auto& vi : v1)
    {
      assert(!vi.is_empty());
      if(p2.contains(vi) == BoolInterval::TRUE) // strictly contained
        inter.push_back(vi);
    }

    auto v2 = p2.vertices();
    for(const auto& vi : p2.vertices())
    {
      assert(!vi.is_empty());
      if(p1.contains(vi) == BoolInterval::TRUE) // strictly contained
        inter.push_back(vi);
    }

    for(const auto& e1 : p1)
      for(const auto& e2 : p2)
      {
        auto x = e1 & e2;
        if(!x.is_empty())
        {
          // In case of colinear edges, the intersection would result in
          // a large box (infinite solutions): end points are kept.
          if((colinear(e1,e2) & BoolInterval::TRUE) == BoolInterval::TRUE)
          {
            if(e1[0].intersects(x)) inter.push_back(e1[0]);
            if(e1[1].intersects(x)) inter.push_back(e1[1]);
            if(e2[0].intersects(x)) inter.push_back(e2[0]);
            if(e2[1].intersects(x)) inter.push_back(e2[1]);
          }

          else
            inter.push_back(x);
        }
      }

    return ConvexPolygon(inter);
  }
}