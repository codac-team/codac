#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestConvexPolygon(unittest.TestCase):

  def test_ConvexPolygon_degenerate_base(self):

    p = ConvexPolygon([[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 1)

    p = ConvexPolygon([[1,3],[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

    p = ConvexPolygon([[1,2],[1,3],[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

    p = ConvexPolygon([[1,2],[1,3],[1,3],[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

    self.assertTrue(len(convex_hull([[1,2],[1,2],[1,2]])) == 1)
    p = ConvexPolygon([[1,2],[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 1)

    p = ConvexPolygon([[1,2],[1,3]])
    self.assertTrue(len(p.vertices()) == 2)

    p = ConvexPolygon([[1,2],[1,3],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

    p = ConvexPolygon([[1,2],[1,3],[1,2],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

    p = ConvexPolygon([[1,2],[1,3],[1,3],[1,2]])
    self.assertTrue(len(p.vertices()) == 2)

  def test_ConvexPolygon_degenerate_cases(self):

    p1 = ConvexPolygon([[0,4]])
    self.assertTrue(len(p1.edges()) == 1)
    self.assertTrue(p1.edges()[0][0] == IntervalVector([0,4]))
    self.assertTrue(p1.edges()[0][1] == IntervalVector([0,4]))

    p1 = ConvexPolygon([[0,4],[2,8]])
    self.assertTrue(len(p1.edges()) == 1)
    self.assertTrue(p1.edges()[0][0] == IntervalVector([0,4]))
    self.assertTrue(p1.edges()[0][1] == IntervalVector([2,8]))
    self.assertTrue(p1 == ConvexPolygon([[2,8],[0,4]]))

  def test_ConvexPolygon_intersection(self):

    p1 = ConvexPolygon([[0,4],[4,8],[7,0]])
    p2 = ConvexPolygon([[-1,2],[-1,3],[5,3],[5,2]])
    self.assertTrue((p1 & p2) == ConvexPolygon([
        [3.5,2],[5,2],[5,3],[1.75,3]
      ]))

    p1 = ConvexPolygon([[3,2],[1,6],[6,5]])
    p2 = ConvexPolygon(IntervalVector([[0,4],[0,4]]))
    self.assertTrue((p1 & p2) == ConvexPolygon([
        [3,2],[2,4],[4,4],[4,3]
      ]))
  
    p1 = ConvexPolygon([[1,2],[3,4],[5,1],[2,1]])
    p2 = ConvexPolygon(IntervalVector([[2,6],[2,6]]))
    self.assertTrue(Approx(p1 & p2) == ConvexPolygon([
        [2,2],[2,3],[3,4],[4.+1./3.,2.]
      ]))

    # Big box
    p1 = ConvexPolygon([[1,2],[3,4],[5,1],[2,1]])
    p2 = ConvexPolygon(IntervalVector([[-10,10],[-10,10]]))
    self.assertTrue((p1 & p2) == p1) # same polygon

    # Inner box
    p1 = ConvexPolygon([[1,2],[3,4],[5,1],[2,1]])
    p2 = ConvexPolygon(IntervalVector([[2.8,3],[2.8,3]]))
    self.assertTrue((p1 & p2) == p2) # same box

    p1 = ConvexPolygon([[2,1],[3,1],[4,2],[4,3],[3,4],[2,4],[1,3],[1,2]])
    p2 = ConvexPolygon(IntervalVector([[1,4],[1,4]]))
    self.assertTrue((p1 & p2) == p1) # same polygon

    # Shifted polygon points declaration
    p1 = ConvexPolygon([[3,4],[2,4],[1,3],[1,2],[2,1],[3,1],[4,2],[4,3]])
    p2 = ConvexPolygon(IntervalVector([[1,4],[1,4]]))
    self.assertTrue((p1 & p2) == p1) # same polygon

    # Degenerate case
    p1 = ConvexPolygon([[4000,200]])
    p2 = ConvexPolygon(IntervalVector([4000,200]))
    self.assertTrue(p1.box() == IntervalVector([4000,200]))
    self.assertTrue(p2.box() == IntervalVector([4000,200]))
    self.assertTrue((p1 & p2) == p1) # same polygon

    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon(IntervalVector([[2,6],[1,5]]))
    q = p1 & p2
    self.assertTrue(Approx(q) == Polygon([[2,1.2],[6,2],[6,4.8],[2,4]]))
    self.assertTrue(len(q.edges()) == 4)
    self.assertTrue(len(q.vertices()) == 4)

    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon(IntervalVector([[3,5],[1,5]]))
    q = p1 & p2
    self.assertTrue(Approx(q) == Polygon([[3,1.4],[5,1.8],[5,4.6],[3,4.2]]))
    self.assertTrue(len(q.edges()) == 4)
    self.assertTrue(len(q.vertices()) == 4)

    # Degenerated box
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon(IntervalVector([[4],[1,5]]))

    self.assertTrue(p1.edges()[0] == Segment([[1,1],[6,2]]))
    self.assertTrue(p1.edges()[1] == Segment([[6,2],[7,5]]))
    self.assertTrue(p1.edges()[2] == Segment([[7,5],[2,4]]))
    self.assertTrue(p1.edges()[3] == Segment([[2,4],[1,1]]))
    self.assertTrue(len(p1.edges()) == 4)

    self.assertTrue(p2.edges()[0] == Segment([[4,1],[4,5]]))
    self.assertTrue(len(p2.edges()) == 1)

    self.assertTrue((p1.edges()[1] & p2.edges()[0]) == IntervalVector.empty(2))
    self.assertTrue(Approx(p1.edges()[0] & p2.edges()[0]) == IntervalVector([4,1.6]))
    self.assertTrue((p1.edges()[3] & p2.edges()[0]) == IntervalVector.empty(2))
    self.assertTrue(Approx(p1.edges()[2] & p2.edges()[0]) == IntervalVector([4,4.4]))

    q = p1 & p2
    self.assertTrue(Approx(q) == Polygon([[4,4.4],[4,1.6]]))
    self.assertTrue(len(q.vertices()) == 2)

    # Degenerated polygon
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon([[4,1],[4,5]])
    q = p1 & p2
    self.assertTrue(Approx(q) == Polygon([[4,4.4],[4,1.6]]))
    self.assertTrue(len(q.vertices()) == 2)

    # Point intersection
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon([[2,4],[-4,4],[0,8]])
    q = p1 & p2
    self.assertTrue(q == Polygon([[2,4]]))
    self.assertTrue(len(q.vertices()) == 1)

    # Point intersection, line polygon
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon([[2,4],[-4,4]])
    q = p1 & p2
    self.assertTrue(q == Polygon([[2,4]]))
    self.assertTrue(len(q.vertices()) == 1)

    # Empty intersection
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon([[5,1.5],[8,2],[8,0],[5,0]])
    q = p1 & p2
    self.assertTrue(q == Polygon.empty())
    self.assertTrue(len(q.vertices()) == 0)
    self.assertTrue(q.is_empty())

    # Empty intersection, degenerate case
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon([[5,1.5],[80,2]])
    q = p1 & p2
    self.assertTrue(q == Polygon.empty())
    self.assertTrue(len(q.vertices()) == 0)
    self.assertTrue(q.is_empty())

    # Intersection of empty polygons
    p1 = ConvexPolygon.empty()
    p2 = ConvexPolygon.empty()
    q = p1 & p2
    self.assertTrue(q == Polygon.empty())
    self.assertTrue(q.is_empty())

    # Intersection of a polygon and one empty polygon
    p1 = ConvexPolygon([[1,1],[2,4],[7,5],[6,2]])
    p2 = ConvexPolygon.empty()
    q = p1 & p2
    self.assertTrue(q == Polygon.empty())
    self.assertTrue(q.is_empty())

    # Intersection of a polygon and one empty polygon (line)
    p1 = ConvexPolygon([[5,1.5],[80,2]])
    p2 = ConvexPolygon.empty()
    q = p1 & p2
    self.assertTrue(q == Polygon.empty())
    self.assertTrue(q.is_empty())

    # Other test with inflated points
    p1 = ConvexPolygon(IntervalVector([[-4,4],[-3,3]]))
    a1,a2 = IntervalVector([-4,-6]), IntervalVector([-4,6])
    a1.inflate(1e-10)
    a2.inflate(1e-10)
    p2 = ConvexPolygon([Segment(a1,a2)])
    q = p1 & p2
    self.assertTrue(len(q.edges()) == 1)
    self.assertTrue(Approx(q.edges()[0][0],1e-5) == IntervalVector([-4,-3]))
    self.assertTrue(Approx(q.edges()[0][1],1e-5) == IntervalVector([-4,3]))

    # Parallel edges
    p1 = ConvexPolygon([
      [-1,-10],
      [3,-10],
      [3,1],
      [-1,6],
    ])
    p2 = ConvexPolygon([
      [-1,10],
      [-1,-1],
      [3,-6],
      [3,10],
    ])
    q = p1 & p2
    self.assertTrue(q == ConvexPolygon([
      [3,-6],
      [3,1],
      [-1,6],
      [-1,-1],
    ]))

    # Parallel edges towards infinity
    p1 = ConvexPolygon([
      [-1,next_float(-oo)],
      [3,next_float(-oo)],
      [3,1],
      [-1,6],
    ])
    p2 = ConvexPolygon([
      [-1,prev_float(oo)],
      [-1,-1],
      [3,-6],
      [3,prev_float(oo)],
    ])
    q = p1 & p2
    self.assertTrue(q == ConvexPolygon([
      [3,-6],
      [3,1],
      [-1,6],
      [-1,-1],
    ]))

    p1 = ConvexPolygon([[4,3.5],[5,4],[4,4.5]])
    p2 = ConvexPolygon([[4,3.5],[5,3.5],[5,4.25],[4.5,4.25],[4,4]])
    self.assertTrue(Approx(p1 & p2) == ConvexPolygon([[4,4],[4,3.5],[5,4],[4.5,4.25]]))

    p1 = ConvexPolygon([[4,4],[4,3.5],[5,4],[4.5,4.25]])
    p2 = ConvexPolygon(IntervalVector([[4,5],[4.1]]))
    self.assertTrue(Approx(p1 & p2, 1e-10) == ConvexPolygon(IntervalVector([[4.2,4.8],[4.1]])))

    p1 = ConvexPolygon([[4,4],[4,3.5],[5,4],[4.5,4.25]])
    p2 = ConvexPolygon(IntervalVector([[4,5],Interval(41)/10]))
    self.assertTrue(Approx(p1 & p2, 1e-10) == ConvexPolygon(IntervalVector([[4.2,4.8],Interval(41)/10])))

    p1 = ConvexPolygon([
      IntervalVector([[0.0999999, 0.100001],[0.989016, 0.989017]]),
      IntervalVector([[0.0999999, 0.100001],[1.0015, 1.00151]]),
      IntervalVector([[0, 0],[0.999999, 1]]),
      IntervalVector([[0, 0],[1, 1.00001]])
    ])

    p2 = ConvexPolygon([
      IntervalVector([[0, 0],[0.987514, 0.987515]]),
      IntervalVector([[0.1, 0.100001],[0.989016, 0.989017]]),
      IntervalVector([[0.1, 0.100001],[1.0015, 1.00151]]),
      IntervalVector([[0, 0],[1.01248, 1.01249]]),
    ])

    self.assertTrue(Approx(p1 & p2, 1e-5) == ConvexPolygon([
      [[0.0997289, 0.100002],[0.989016, 0.989938]],
      [[0.0995829, 0.100002],[1.00059, 1.00151]],
      [[0, 0],[0.999547, 1.00047]]
    ]))

    p1 &= p2

    m = IntervalVector([6.5,1])
    p1 = ConvexPolygon(m)
    p2 = ConvexPolygon([
      [[6.49999, 6.50001],[0.759358, 0.759359]],
      [[6.49999, 6.50001],[1.19382, 1.19383]],
      [[6.4, 6.40001],[1.20537, 1.20538]],
      [[6.4, 6.40001],[0.78097, 0.780971]],
    ])

    s = Segment(IntervalVector([[6.49999, 6.50001],[0.759358, 0.759359]]), IntervalVector([[6.49999, 6.50001],[1.19382, 1.19383]]))
    self.assertTrue(s.contains(m) == BoolInterval.UNKNOWN)
    self.assertTrue((p1 & p2) == ConvexPolygon([m]))


if __name__ ==  '__main__':
  unittest.main()