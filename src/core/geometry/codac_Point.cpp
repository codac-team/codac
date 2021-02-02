/** 
 *  Point class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iomanip>
#include "codac_Point.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Definition

  Point::Point()
  {

  }

  Point::Point(const Vector& p)
    : Point(p[0], p[1])
  {
    assert(p.size() == 2);
  }

  Point::Point(const IntervalVector& p)
    : Point(p[0], p[1])
  {
    assert(p.size() == 2);
  }

  Point::Point(const Interval& x, const Interval& y)
  {
    m_pt[0] = x;
    m_pt[1] = y;

    if(x.is_empty()) m_pt[1].set_empty();
    if(y.is_empty()) m_pt[0].set_empty();
  }

  const Point& Point::operator=(const Point& p)
  {
    m_pt = p.box();
    return *this;
  }


  // Accessing values

  const Interval& Point::x() const
  {
    return m_pt[0];
  }

  const Interval& Point::y() const
  {
    return m_pt[1];
  }

  const Interval& Point::operator[](size_t id) const
  {
    assert(id >= 0 && (int)id < m_pt.size());
    return m_pt[id];
  }

  const IntervalVector& Point::box() const
  {
    return m_pt;
  }

  const Vector Point::mid() const
  {
    return m_pt.mid();
  }

  double Point::max_diam() const
  {
    return m_pt.max_diam();
  }
  
  const vector<Vector> Point::bounds_pts() const
  {
    vector<Vector> v_pts;

    if(!m_pt[0].is_empty() && !m_pt[1].is_empty())
    {
      v_pts.push_back(Vector({m_pt[0].lb(),m_pt[1].lb()}));
      v_pts.push_back(Vector({m_pt[0].lb(),m_pt[1].ub()}));
      v_pts.push_back(Vector({m_pt[0].ub(),m_pt[1].lb()}));
      v_pts.push_back(Vector({m_pt[0].ub(),m_pt[1].ub()}));
    }

    return Point::remove_identical_pts(v_pts);
  }


  // Tests

  bool Point::is_unbounded() const
  {
    return m_pt.is_unbounded();
  }

  bool Point::does_not_exist() const
  {
    return m_pt.is_empty();
  }

  bool Point::operator==(const Point& p) const
  {
    return m_pt == p.m_pt;
  }

  bool Point::operator!=(const Point& p) const
  {
    return m_pt != p.m_pt;
  }


  // Setting values

  const Point& Point::inflate(double rad)
  {
    assert(rad >= 0.);
    m_pt.inflate(rad);
    return *this;
  }


  // String

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(";
    
    if(p.m_pt[0].is_degenerated()) str << p.m_pt[0].lb();
    else str << p.m_pt[0];

    str << ",";

    if(p.m_pt[1].is_degenerated()) str << p.m_pt[1].lb();
    else str << p.m_pt[1];

    str << ")";
    return str;
  }


  // Static methods

  const BoolInterval Point::aligned(const Point& a, const Point& b, const Point& c)
  {
    const Interval cross_product = (b[0]-a[0]) * (c[1]-a[1]) - (b[1]-a[1]) * (c[0]-a[0]);
    return (cross_product == Interval(0.)) ? YES : (cross_product.contains(0.) ? MAYBE : NO);
  }

  const Point Point::center(const vector<Point> v_pts)
  {
    assert(!v_pts.empty());

    IntervalVector center(2, 0.);
    for(const auto& pt : v_pts)
    {
      assert(!pt.does_not_exist());
      center += pt.box();
    }

    return Point((1./v_pts.size())*center.mid());
  }

  void Point::push(const IntervalVector& box, vector<Point>& v_pts)
  {
    assert(box.size() == 2);
    assert(!box.is_empty());

    const Interval xlb = box[0].lb() != NEG_INFINITY ? box[0].lb() : Interval(NEG_INFINITY, box[0].ub());
    const Interval xub = box[0].ub() != POS_INFINITY ? box[0].ub() : Interval(box[0].lb(), POS_INFINITY);

    const Interval ylb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
    const Interval yub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

    v_pts.push_back(Point(xlb, ylb));
    v_pts.push_back(Point(xub, ylb));
    v_pts.push_back(Point(xub, yub));
    v_pts.push_back(Point(xlb, yub));
  }

  void Point::push(const IntervalVector& box, vector<Vector>& v_pts)
  {
    assert(box.size() == 2);
    assert(!box.is_empty());
    assert(!box.is_unbounded());

    const double xlb = box[0].lb();
    const double xub = box[0].ub();

    const double ylb = box[1].lb();
    const double yub = box[1].ub();

    v_pts.push_back(Vector({xlb, ylb}));
    v_pts.push_back(Vector({xub, ylb}));
    v_pts.push_back(Vector({xub, yub}));
    v_pts.push_back(Vector({xlb, yub}));
  }

  vector<Point> Point::to_Points(const vector<Vector>& v_pts)
  {
    vector<Point> v_pts_(v_pts.size());
    for(size_t i = 0 ; i < v_pts.size() ; i++)
      v_pts_[i] = Point(v_pts[i]);
    return v_pts_;
  }
  
  vector<Point> Point::remove_identical_pts(const vector<Point>& v_pts_)
  {
    // Removing possible redundant points
    vector<Point> v_pts;
    for(const auto& pt : v_pts_)
      if(find(v_pts.begin(),v_pts.end(),pt) == v_pts.end())
        v_pts.push_back(pt);
    return v_pts;
  }
  
  vector<Vector> Point::remove_identical_pts(const vector<Vector>& v_pts_)
  {
    // Removing possible redundant points
    vector<Vector> v_pts;
    for(const auto& pt : v_pts_)
      if(find(v_pts.begin(),v_pts.end(),pt) == v_pts.end())
        v_pts.push_back(pt);
    return v_pts;
  }
}