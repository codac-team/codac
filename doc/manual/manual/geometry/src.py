#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestGeometryManual(unittest.TestCase):

  def tests_geometry_manual(test):

    # [geometry-1-beg]
    a = orientation([0,0],[1,1],[10,10])
    # a == OrientationInterval.COLINEAR
    b = orientation([0,0],[1.1,1.1],[10,10])
    # b == OrientationInterval.UNKNOWN (due to floating point uncertainty)

    p1, p2, p3 = IntervalVector([0,0]), IntervalVector([1,1]), IntervalVector([-1,2])
    c = orientation(p1,p2,p3)
    # c == OrientationInterval.CLOCKWISE
    d = orientation(p3,p2,p1)
    # d == OrientationInterval.COUNTERCLOCKWISE
    # [geometry-1-end]
    test.assertTrue(a == OrientationInterval.COLINEAR)
    test.assertTrue(b == OrientationInterval.UNKNOWN)
    test.assertTrue(c == OrientationInterval.CLOCKWISE)
    test.assertTrue(d == OrientationInterval.COUNTERCLOCKWISE)

    # [geometry-2-beg]
    a = aligned([0,0],[1,1],[10,10])
    # a == BoolInterval.TRUE
    b = aligned([0,0],[1.1,1.1],[10,10])
    # b == BoolInterval.UNKNOWN (due to floating point uncertainty)

    p1, p2, p3 = IntervalVector([0,0]), IntervalVector([0,1]), IntervalVector([10,10])
    c = aligned(p1,p2,p3)
    # c == BoolInterval.FALSE
    # [geometry-2-end]
    test.assertTrue(a == BoolInterval.TRUE)
    test.assertTrue(b == BoolInterval.UNKNOWN)
    test.assertTrue(c == BoolInterval.FALSE)

    # [geometry-3-beg]
    v = [
      [0,3],[1,1],[2,2],[3.5,3.5],
      [0.5,1.2],[1,2],[3,1],[3,3],
      [1,3.5],[2.5,4],[0.1,2.7],[3.2,3.6]
    ]

    hull = convex_hull(v);
    # hull == [ [1,1], [3,1], [3.5,3.5], [2.5,4], [1,3.5], [0,3], [0.5,1.2] ]
    # [geometry-3-end]
    test.assertTrue(hull[0] == IntervalVector([1,1]))
    test.assertTrue(hull[1] == IntervalVector([3,1]))
    test.assertTrue(hull[2] == IntervalVector([3.5,3.5]))
    test.assertTrue(hull[3] == IntervalVector([2.5,4]))
    test.assertTrue(hull[4] == IntervalVector([1,3.5]))
    test.assertTrue(hull[5] == IntervalVector([0,3]))
    test.assertTrue(hull[6] == IntervalVector([0.5,1.2]))

    # [segment-1-beg]
    u1,v1 = Segment([[0,0],[2,2]]),Segment([[2,4],[0,6]])
    p1 = u1 & v1 # the two segments do not intersect
    # p1 == IntervalVector.empty(2)

    u2,v2 = Segment([[4,0],[0,4]]),Segment([[2,0],[4,2]])
    p2 = u2 & v2
    # p2 == IntervalVector([3,1])

    u3,v3 = Segment([[1,1],[4,4]]),Segment([[2,2],[5,5]])
    p3 = u3 & v3 # the two segments are colinear
    # p3 == IntervalVector([[2,4],[2,4]])
    # [segment-1-end]
    test.assertTrue(p1.is_empty())
    test.assertTrue(p2 == IntervalVector([3,1]))
    test.assertTrue(p3 == IntervalVector([[2,4],[2,4]]))

    # [segment-2-beg]
    u1,v1 = Segment([[0,0],[2,2]]),Segment([[2,4],[0,6]])
    p1 = proj_intersection(u1,v1)
    # p1 == IntervalVector([3,3])

    u2,v2 = Segment([[4,0],[0,4]]),Segment([[2,0],[4,2]])
    p2 = proj_intersection(u2,v2)
    # p2 == IntervalVector([3,1])

    u3,v3 = Segment([[1,1],[4,4]]),Segment([[2,2],[5,5]])
    p3 = proj_intersection(u3,v3)
    # p3 == IntervalVector([[-oo,oo],[-oo,oo]])
    # [segment-2-end]
    test.assertTrue(p1 == IntervalVector([3,3]))
    test.assertTrue(p2 == IntervalVector([3,1]))
    test.assertTrue(p3 == IntervalVector(2))

    # [segment-3-beg]
    p1,p2,p3,p4 = [0,0],[1,1],[1.1,1.1],[10,10]

    a = colinear(Segment(p1,p2),Segment(p1,p4))
    # a == BoolInterval.TRUE
    
    b = colinear(Segment(p1,p3),Segment(p1,p4))
    # b == BoolInterval.UNKNOWN (due to floating point uncertainty)
    # [segment-3-end]
    test.assertTrue(a == BoolInterval.TRUE)
    test.assertTrue(b == BoolInterval.UNKNOWN)

    # [polygon-1-beg]
    # ConvexPolygons are defined from sets of vertices in any order
    p1 = ConvexPolygon([ # drawn in red
      [4,2],[1,8],[2,10],[2,3],
      [1,6],[8,6],[7,3],[5,11],[7,10]
    ])
    p2 = ConvexPolygon([ # drawn in blue
      [5,1],[7,1],[9,3],[4,3],[4,5],
      [9,6],[5,6],[8,8],[6,7]
    ])

    p3 = p1 & p2
    # [polygon-1-end]

if __name__ ==  '__main__':
  unittest.main()