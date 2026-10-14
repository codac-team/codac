#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestPolygon(unittest.TestCase):

  def test_polygon_base(self):

    p1 = Polygon([[3,4]])
    self.assertTrue(p1 == Polygon([[3,4]]))

    p2 = Polygon([[3,4],[1,2]])
    self.assertTrue(p2 == Polygon([[3,4],[1,2]]))
    self.assertTrue(p2 == Polygon([[1,2],[3,4]]))

    p3 = Polygon([[3,4],[1,2],[5,1]])
    self.assertTrue(p3 == Polygon([[3,4],[1,2],[5,1]]))
    self.assertTrue(p3 == Polygon([[1,2],[5,1],[3,4]]))
    self.assertTrue(p3 == Polygon([[5,1],[1,2],[3,4]]))
    self.assertTrue(p3 == Polygon([[1,2],[3,4],[5,1]]))

    # Multiple contours can have independent orientations.
    edges1 = [
      Segment([0,0],[1,0]), Segment([1,0],[0,1]), Segment([0,1],[0,0]),
      Segment([3,0],[4,0]), Segment([4,0],[3,1]), Segment([3,1],[3,0])
    ]

    edges2 = [
      Segment([0,0],[0,1]), Segment([0,1],[1,0]), Segment([1,0],[0,0]),
      Segment([3,0],[4,0]), Segment([4,0],[3,1]), Segment([3,1],[3,0])
    ]

    self.assertTrue(Polygon(edges1) == Polygon(edges2))

    vertices = Polygon(edges1).vertices()
    self.assertTrue(len(vertices) == 6)
    self.assertTrue(vertices[0] == IntervalVector([0,0]))
    self.assertTrue(vertices[1] == IntervalVector([1,0]))
    self.assertTrue(vertices[2] == IntervalVector([0,1]))
    self.assertTrue(vertices[3] == IntervalVector([3,0]))
    self.assertTrue(vertices[4] == IntervalVector([4,0]))
    self.assertTrue(vertices[5] == IntervalVector([3,1]))

    # Same number of edges, but a different polygon.
    edges1 = [
      Segment([0,0],[1,0]), Segment([1,0],[0,1]), Segment([0,1],[0,0])
    ]

    edges2 = [
      Segment([0,0],[1,0]), Segment([1,0],[0,2]), Segment([0,2],[0,0])
    ]

    self.assertFalse(Polygon(edges1) == Polygon(edges2))

    # Fully collinear vertices reduce to one segment.
    p = Polygon([[0,0],[1,0],[2,0],[3,0]])
    expected = Polygon([[0,0],[3,0]])

    self.assertTrue(p == expected)
    self.assertTrue(len(p.edges()) == 1)
    self.assertTrue(len(p.vertices()) == 2)
    self.assertTrue(p.contains([1.5,0]) == BoolInterval.TRUE)

    # Consecutive duplicate vertices.
    p = Polygon([[0,0],[1,0],[1,0],[2,0],[2,1],[0,1]])
    self.assertTrue(p == Polygon([[0,0],[2,0],[2,1],[0,1]]))

    # Repeated duplicates and explicit closing vertex.
    p = Polygon([[0,0],[2,0],[2,0],[2,0],[2,1],[0,1],[0,0]])
    self.assertTrue(p == Polygon([[0,0],[2,0],[2,1],[0,1]]))

  def test_empty_polygon(self):

    p1 = Polygon.empty()
    self.assertTrue(p1.contains(IntervalVector([1,1])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector(2)) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector.empty(2)) == BoolInterval.EMPTY)
    self.assertTrue(p1.is_empty())
    self.assertTrue(len(p1.edges()) == 0)
    self.assertTrue(len(p1.vertices()) == 0)

  def test_Polygon(self):

    p1 = Polygon([[3,-1],[3,4],[5,6],[-1,1]])
    self.assertTrue(p1.contains(IntervalVector.empty(2)) == BoolInterval.EMPTY)
    self.assertTrue(p1.contains([3.1,3]) == BoolInterval.FALSE)
    self.assertTrue(p1.contains([2.9,3]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([3,3]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([0,1]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([4,1]) == BoolInterval.FALSE)
    self.assertTrue(p1.contains([2,4]) == BoolInterval.FALSE)
    self.assertTrue(p1.contains([2.8,4]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([3,4]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([4,4]) == BoolInterval.FALSE)
    self.assertTrue(p1.contains([5,6]) == BoolInterval.TRUE)
    self.assertTrue(p1.contains([6,6]) == BoolInterval.FALSE)

    transect = Segment(Vector([next_float(-oo),3]), Vector([3,3]))
    e1,e2 = Segment(Vector([5,6]),Vector([-1,1])), Segment(Vector([3,-1]),Vector([3,4]))
    self.assertTrue(transect.intersects(e1) == BoolInterval.TRUE)
    self.assertTrue(transect.intersects(e2) == BoolInterval.TRUE)

    p2 = Polygon([[0,0],[0,1],[1,1],[1,0]])
    self.assertTrue(p2.contains([0,0]) == BoolInterval.TRUE)
    self.assertTrue(p2.contains([1,1]) == BoolInterval.TRUE)
    self.assertTrue(p2.contains([0,2]) == BoolInterval.FALSE)
    self.assertTrue(p2.contains([2,0]) == BoolInterval.FALSE)
    self.assertTrue(p2.contains([0.5,1]) == BoolInterval.TRUE)
    self.assertTrue(p2.contains([1,0.5]) == BoolInterval.TRUE)

    transect = Segment(Vector([next_float(-oo),2]), Vector([0,2]))
    e1 = Segment(Vector([0,0]),Vector([0,1]))
    e2 = Segment(Vector([0,1]),Vector([1,1]))
    e3 = Segment(Vector([1,1]),Vector([1,0]))
    e4 = Segment(Vector([1,0]),Vector([0,0]))

    self.assertTrue(transect.intersects(e1) == BoolInterval.FALSE)
    self.assertTrue(transect.intersects(e2) == BoolInterval.FALSE)
    self.assertTrue(transect.intersects(e3) == BoolInterval.FALSE)
    self.assertTrue(transect.intersects(e4) == BoolInterval.FALSE)

    p3 = Polygon([[0,1],[1,0],[0,0]])
    self.assertTrue(p3.contains([1,1]) == BoolInterval.FALSE)

  def test_Polygon_degenerated_cases(self):

    p1 = Polygon([[1,1]])
    self.assertTrue(p1.contains(IntervalVector([1,1])) == BoolInterval.TRUE)
    self.assertTrue(p1.contains(IntervalVector([2,1])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector([2,2])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector([1,2])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector([-10,1])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector([10,1])) == BoolInterval.FALSE)

    p2 = Polygon([[1,1],[3,2]])
    self.assertTrue(p2.contains(IntervalVector([1,1])) == BoolInterval.TRUE)
    self.assertTrue(p2.contains(IntervalVector([3,2])) == BoolInterval.TRUE)
    self.assertTrue(p2.contains(IntervalVector([2,1.5])) == BoolInterval.TRUE)

    p3 = Polygon([[1,1],[3,1]])
    self.assertTrue(p3.contains(IntervalVector([1,1])) == BoolInterval.TRUE)
    self.assertTrue(p3.contains(IntervalVector([3,1])) == BoolInterval.TRUE)
    self.assertTrue(p3.contains(IntervalVector([2,1])) == BoolInterval.TRUE)

    p4 = Polygon([[1,1],[1,3]])
    self.assertTrue(p4.contains(IntervalVector([1,1])) == BoolInterval.TRUE)
    self.assertTrue(p4.contains(IntervalVector([1,3])) == BoolInterval.TRUE)
    self.assertTrue(p4.contains(IntervalVector([1,2])) == BoolInterval.TRUE)

  def test_Polygon_limit_case(self):

    x = Vector([5,3.5])
    p = Polygon([[4,3.5],[5,4],[4,4.5]])
    self.assertTrue(p.contains(x) == BoolInterval.FALSE)

    p1 = ConvexPolygon([[4,3.5],[5,4],[4.5,4.25],[4,4]])
    self.assertTrue(p1.contains([4.5,4.25]) == BoolInterval.TRUE)
    p2 = ConvexPolygon([[4,4.25],[5,4.25]])
    i = p1 & p2
    self.assertTrue(i == ConvexPolygon(IntervalVector([4.5,4.25])))
    self.assertTrue(len(i.vertices()) == 1)

    # All four axis-aligned transects cross a vertex.
    p = Polygon([[0,1],[1,0],[0,-1],[-1,0]])
    self.assertTrue(p.contains([0,0]) == BoolInterval.TRUE)

    p_rev = Polygon([[-1,0],[0,-1],[1,0],[0,1]])
    self.assertTrue(p_rev.contains([0,0]) == BoolInterval.TRUE)

if __name__ ==  '__main__':
  unittest.main()