/** 
 *  codac2_Polygon.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include "codac2_Polygon.h"
#include "codac2_geometry.h"
#include "codac2_template_tools.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  Polygon::Polygon()
  { }

  Polygon::Polygon(initializer_list<Vector> vertices)
    : Polygon(vector<Vector>(vertices))
  { }

  Polygon::Polygon(const vector<Vector>& vertices)
    : Polygon(vectorVector_to_vectorIntervalVector(vertices))
  { }

  Interval dist_(const IntervalVector& a, const IntervalVector& b)
  {
    return sqr(a[0]-b[0])+sqr(a[1]-b[1]);
  }

  Polygon::Polygon(const std::vector<IntervalVector>& v)
    : std::vector<Segment>(
      [v]
      {
        vector<Segment> edges;

        switch(v.size())
        {
          case 0:
          {
            // Empty polygon has no edge
            break;
          }

          case 1:
          {
            assert_release(v[0].size() == 2);
            edges = { Segment(v[0],v[0]) };
            break;
          }

          case 2:
          {
            assert_release(v[0].size() == 2 && v[1].size() == 2);
            edges = { Segment(v[0],v[1]) };
            break;
          }

          default:
          {
            vector<IntervalVector> v_noaligned { v[0] };

            size_t farthest = 1, n = v.size();
            for(size_t i = 1 ; i < n+1 ; i++)
            {
              if(v[i-1] == v[i%n])
                continue;

              if(i < n && aligned(v[i-1],v[i%n],v[(i+1)%n]) == BoolInterval::TRUE)
              {
                if(dist_(v[i-1],v[farthest%n]).mid() < dist_(v[i-1],v[(i+1)%n]).mid())
                  farthest = std::max(farthest,(i+1)%n);
              }

              else
              {
                if(farthest%n != 0)
                  v_noaligned.push_back(v[farthest%n]);
                farthest = i+1;
              }
            }

            for(size_t i = 0 ; i < v_noaligned.size() ; i++)
              edges.push_back({ v_noaligned[i], v_noaligned[(i+1)%v_noaligned.size()] });
          }
        }
        
        return edges;
      }())
  { }

  Polygon::Polygon(initializer_list<Segment> edges)
    : vector<Segment>(edges)
  { }

  Polygon::Polygon(const vector<Segment>& edges)
    : vector<Segment>(edges)
  { }
  
  Polygon::Polygon(const IntervalVector& x)
    : Polygon([&x]() -> vector<IntervalVector>
      {
        assert_release(x.size() == 2);

        if(x.is_empty())
          return { };
        
        else if(x.is_flat())
          return {
            { x[0].lb(),x[1].lb() },
            { x[0].ub(),x[1].ub() }
          };

        else
          return {
            // Built in counterclockwise order
            { x[0].lb(), x[1].lb() },
            { x[0].ub(), x[1].lb() },
            { x[0].ub(), x[1].ub() },
            { x[0].lb(), x[1].ub() }
          };
      }())
  { }
  
  const vector<Segment>& Polygon::edges() const
  {
    return *this;
  }

  vector<IntervalVector> Polygon::vertices() const
  {
    vector<IntervalVector> v;

    if(!is_empty())
    {
      size_t n = this->size();

      for(size_t i = 0 ; i < n ; i++)
        v.push_back((*this)[i][0]);

      if((*this)[0][0] != (*this)[n-1][1])
        v.push_back((*this)[n-1][1]);
    }

    return v;
  }

  IntervalVector Polygon::box() const
  {
    IntervalVector b = IntervalVector::empty(2);
    for(const auto& e : *this)
      b |= e.box();
    return b;
  }

  bool Polygon::is_empty() const
  {
    return this->vector<Segment>::empty();
  }

  BoolInterval Polygon::contains(const IntervalVector& p) const
  {
    assert_release(p.size() == 2);

    IntervalVector box_hull = box();
    if(!p.intersects(box_hull))
      return BoolInterval::FALSE;

    for(const auto& e : *this)
    {
      auto b = e.contains(p);
      if(b == BoolInterval::TRUE || b == BoolInterval::UNKNOWN)
        return b;
    }

    if(this->size() <= 2) // if the polygon has no volume
      // Then it contains a point only if the point is on one of,
      // its edges, which has been tested before
      return BoolInterval::FALSE;

    bool retry;
    Segment transect(IntervalVector(2),IntervalVector(2));
    // ^ selected transect (e.g. horizontal ray) for crossing the polygon.
    // Odd number of crossing => point is inside
    // Even number of crossing => point is outside

    Index i = 0;
    // Some limit cases must be considered, for instance when the
    // transect crosses exactly one vertex of the polygon, and/or 
    // when the point to be tested is this very vertex. Below, we
    // generate a convenient transect avoiding such configuration.

    do
    {
      retry = false;

      // Creating a ray candidate:
      Segment try_transect = [i,&p]() -> Segment {
        switch(i)
        {
          case 0:
            return { {next_float(-oo),p[1]}, p };
          case 1:
            return { {prev_float(oo),p[1]}, p };
          case 2:
            return { {p[0],next_float(-oo)}, p };
          case 3:
            return { {p[0],prev_float(oo)}, p };
          default:
          {
            assert_release(false &&
              "failed to test if the point is contained in polygon");
            return Segment(IntervalVector(2),IntervalVector(2));
          }
        }
      }();

      // The ray may pass through the vertices, we must double counting
      for(const auto& pi : vertices())
        if(try_transect.contains(pi) != BoolInterval::FALSE)
        {
          retry = true;
          break;
        }

      if(!retry)
        transect = try_transect;

      i++;
      assert(i < 10);
    } while(retry);

    // Now the number i of crossing can be computed.
    i = 0;

    for(const auto& e : *this)
    {
      switch(transect.intersects(e))
      {
        case BoolInterval::TRUE:
          i++;
          break;

        case BoolInterval::FALSE:
          // no intersection
          break;

        case BoolInterval::UNKNOWN:
          return BoolInterval::UNKNOWN;

        case BoolInterval::EMPTY:
        default:
          assert(false && "BoolInterval::EMPTY should not happen");
      }
    }
    
    return (i%2 == 0) ? BoolInterval::FALSE : BoolInterval::TRUE;
  }
  
  Polygon Polygon::empty()
  {
    return Polygon();
  }

  bool Polygon::operator==(const Polygon& p) const
  {
    if(is_empty() || p.is_empty())
      return is_empty() && p.is_empty();

    size_t n = this->size();
    if(p.size() != n)
      return false;

    size_t i; // looking for same reference of first value
    for(i = 0 ; i < n ; i++)
      if((*this)[0] == p[i])
        break;

    int way = 1;
    if(n > 1)
      way = ((*this)[1] == p[(i+1)%n]) ? 1 : -1;

    for(size_t j = 0 ; j < n ; j++)
      if((*this)[j] != p[(i+way*j+2*n)%n])
        return false;

    return true;
  }

  ostream& operator<<(ostream& str, const Polygon& p)
  {
    str << "{ ";

    auto v = p.vertices();
    for(size_t i = 0 ; i < v.size() ; i++)
    {
      if(i > 0) str << " -- ";
      str << v[i];
    }

    str << " }";
    return str;
  }
}