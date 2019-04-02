/** 
 *  Point class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iomanip>
#include "tubex_Point.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Point::Point()
    : m_x(Interval::EMPTY_SET), m_y(Interval::EMPTY_SET)
  {

  }

  Point::Point(const Interval& x, const Interval& y)
    : m_x(x), m_y(y)
  {
    if(m_x.is_empty()) m_y.set_empty();
    if(m_y.is_empty()) m_x.set_empty();
  }

  const Interval& Point::x() const
  {
    return m_x;
  }

  const Interval& Point::y() const
  {
    return m_y;
  }
  
  const IntervalVector Point::box() const
  {
    IntervalVector box(2);
    box[0] = m_x; box[1] = m_y;
    return box;
  }

  const Point& Point::operator=(const Point& p)
  {
    m_x = p.x();
    m_y = p.y();
  }

  bool Point::operator==(const Point& p) const
  {
    return m_x == p.m_x && m_y == p.m_y;
  }

  bool Point::operator!=(const Point& p) const
  {
    return m_x != p.m_x || m_y != p.m_y;
  }

  bool Point::does_not_exist() const
  {
    return m_x.is_empty() || m_y.is_empty();
  }

  bool Point::is_unbounded() const
  {
    return m_x.is_unbounded() || m_y.is_unbounded();
  }

  const Point& Point::inflate(double rad)
  {
    assert(rad >= 0.);
    m_x.inflate(rad);
    m_y.inflate(rad);
    return *this;
  }
  
  const Point Point::operator|=(const Point& p)
  {
    return Point(m_x | p.m_x, m_y | p.m_y);
  }

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(";
    
    if(p.m_x.is_degenerated()) str << p.m_x.lb();
    else str << p.m_x;

    str << ",";

    if(p.m_y.is_degenerated()) str << p.m_y.lb();
    else str << p.m_y;

    str << ")";
    return str;
  }
  
  const Point Point::center(const vector<Point> v_pts)
  {
    Interval x = 0., y = 0.;
    for(int i = 0 ; i < v_pts.size() ; i++)
    {
      assert(!v_pts[i].does_not_exist());
      x += v_pts[i].x();
      y += v_pts[i].y();
    }

    return Point(x / v_pts.size(), y / v_pts.size());
  }

  const BoolInterval Point::aligned(const Point& a, const Point& b, const Point& c)
  {
    Interval cross_product = (b.x()-a.x()) * (c.y()-a.y()) - (b.y()-a.y()) * (c.x()-a.x());
    return (cross_product == Interval(0.)) ? YES : (cross_product.contains(0.) ? MAYBE : NO);
  }

  const vector<Point> Point::merge_close_points(const vector<Point>& v_pts)
  {
    vector<Point> v_vertices, v_pts_copy = v_pts;

    while(!v_pts_copy.empty())
    {
      v_vertices.push_back(*v_pts_copy.begin());
      v_pts_copy.erase(v_pts_copy.begin());
      bool merge;

      do
      {
        merge = false;

        for(vector<Point>::iterator it = v_pts_copy.begin() ; it != v_pts_copy.end() ; )
        {
          bool similar_pts = v_vertices.back().x().intersects(it->x())
                          && v_vertices.back().y().intersects(it->y());

          if(similar_pts)
          {
            v_vertices.back() = v_vertices.back() | *it;
            it = v_pts_copy.erase(it);
            merge = true;
            break;
          }

          else
            ++it;
        }
      } while(merge);
    }

    return v_vertices;
  }

  const vector<Point> Point::delete_redundant_points(const vector<Point>& v_pts)
  {
    vector<Point> v_vertices;

    for(int i = 0 ; i < v_pts.size() ; i++)
    {
      if(v_pts[i].does_not_exist())
        continue; // no empty points

      bool diff_from_all_prev_points = true;
      for(int j = 0 ; j < v_vertices.size() && diff_from_all_prev_points ; j++)
        diff_from_all_prev_points &= !(v_pts[i] == v_vertices[j]
                                     & v_pts[i].x().is_degenerated()
                                     & v_pts[i].y().is_degenerated());
      
      if(diff_from_all_prev_points)
        v_vertices.push_back(v_pts[i]);
    }

    return v_vertices;
  }

  const Point operator&(const Point& p1, const Point& p2)
  {
    return Point(p1.x() & p2.x(), p1.y() & p2.y());
  }

  const Point operator|(const Point& p1, const Point& p2)
  {
    return Point(p1.x() | p2.x(), p1.y() | p2.y());
  }
  
  const Point operator-(const Point& p1, const Point& p2)
  {
    return Point(p1.m_x - p2.m_x, p1.m_y - p2.m_y);
  }

  void push_points(const IntervalVector& box, vector<Point>& v_points)
  {
    assert(box.size() == 2);
    if(!box.is_empty())
    {
      Interval xlb = box[0].lb() != NEG_INFINITY ? box[0].lb() : Interval(NEG_INFINITY, box[0].ub());
      Interval xub = box[0].ub() != POS_INFINITY ? box[0].ub() : Interval(box[0].lb(), POS_INFINITY);

      Interval ylb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
      Interval yub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

      v_points.push_back(Point(xlb, ylb));
      v_points.push_back(Point(xub, ylb));
      v_points.push_back(Point(xub, yub));
      v_points.push_back(Point(xlb, yub));
    }
  }
}