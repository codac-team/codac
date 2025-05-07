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

  def tests_polygon_base(self):

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

  def tests_empty_polygon(self):

    p1 = Polygon.empty()
    self.assertTrue(p1.contains(IntervalVector([1,1])) == BoolInterval.FALSE)
    self.assertTrue(p1.contains(IntervalVector(2)) == BoolInterval.FALSE)
    self.assertTrue(p1.is_empty())
    self.assertTrue(len(p1.edges()) == 0)
    self.assertTrue(len(p1.unsorted_vertices()) == 0)
    self.assertTrue(len(p1.sorted_vertices()) == 0)

  def tests_Polygon(self):

    p1 = Polygon([[3,-1],[3,4],[5,6],[-1,1]])
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

  def tests_Polygon_degenerated_cases(self):

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

if __name__ ==  '__main__':
  unittest.main()