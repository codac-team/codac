#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestGeometry(unittest.TestCase):

  def tests_orientation(self):
    
    # Align points
    self.assertTrue(orientation([0.,0.],[5.,0.],[8.,0.]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0.,0.],[5.,0.],[-2.,0.]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0.,0.],[-2.,0.],[5.,0.]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0.,0.],[0.,0.],[5.,0.]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0.,0.],[0.,0.],[0.,0.]) == OrientationInterval.COLINEAR)

    # Clockwise points
    self.assertTrue(orientation([0.,0.],[0.,-5.],[-2.,-5.]) == OrientationInterval.COUNTERCLOCKWISE)
    self.assertTrue(orientation([-999.,0.1],[0.,0.],[-999.,-0.1]) == OrientationInterval.COUNTERCLOCKWISE)

    # Anticlockwise points
    self.assertTrue(orientation([0.,0.],[0.,-5.],[2.,-5.]) == OrientationInterval.CLOCKWISE)
    self.assertTrue(orientation([999.,0.1],[0.,0.],[999.,-0.1]) == OrientationInterval.CLOCKWISE)

    # Other tests
    self.assertTrue(orientation([0,0],[1,1],[0,2]) == OrientationInterval.CLOCKWISE)
    self.assertTrue(orientation([0,0],[1,1],[2,2]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0,0],[1,1],[2.2,2]) == OrientationInterval.COUNTERCLOCKWISE)
    self.assertTrue(orientation([0,0],[2,2],[2,2]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0,0],[2,2],[4,4]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0,0],[8,8],[4,4]) == OrientationInterval.COLINEAR)
    self.assertTrue(orientation([0,0],[10,1],[4,4]) == OrientationInterval.CLOCKWISE)
    self.assertTrue(orientation([0,0],[2,2],[10,1]) == OrientationInterval.COUNTERCLOCKWISE)

  def tests_alignments(self):
    
    # Alignments
    p1 = IntervalVector([0.,0.])
    p2 = IntervalVector([0.,1.])
    p3 = IntervalVector([0.,10.])
    p4 = IntervalVector([1.,10.])
    p5 = IntervalVector([0.,9.+1e-8*Interval(-1.,1.)])
    p6 = IntervalVector([0.+1e-8*Interval(-1.,1.),9.])
    p7 = IntervalVector([0.+1e-8*Interval(-1.,1.),9.+1e-8*Interval(-1.,1.)])

    self.assertTrue(aligned(p1,p2,p3) == BoolInterval.TRUE)
    self.assertTrue(aligned(p1,p2,p4) == BoolInterval.FALSE)
    self.assertTrue(aligned(p1,p1,p4) == BoolInterval.TRUE)
    self.assertTrue(aligned(p1,p2,p5) == BoolInterval.TRUE)
    self.assertTrue(aligned(p1,p2,p6) == BoolInterval.UNKNOWN)
    self.assertTrue(aligned(p1,p2,p7) == BoolInterval.UNKNOWN)
    self.assertTrue(aligned([0,0],[1,1],[10,10]) == BoolInterval.TRUE)
    self.assertTrue(aligned([[-oo,0],[-oo,0]],[1,1],[10,10]) == BoolInterval.UNKNOWN)
    self.assertTrue(aligned(IntervalVector(2),[1,1],[10,10]) == BoolInterval.UNKNOWN)

  def tests_convex_hull(self):

    v = [
      [0,3],[1,1],[2,2],[4,4],[0,0],[1,2],[3,1],[3,3]
    ]

    hull = convex_hull(v)

    self.assertTrue(hull[0] == IntervalVector([0,0]))
    self.assertTrue(hull[1] == IntervalVector([3,1]))
    self.assertTrue(hull[2] == IntervalVector([4,4]))
    self.assertTrue(hull[3] == IntervalVector([0,3]))
    self.assertTrue(len(hull) == 4)

    v = [
      [1,3],[1,4],[1.5,2],[2,1],[2,2],[3,0],
      [3,3],[3,4.5],[4,2.5],[4,4],[5,1],[5,2],
      [4,0],[5,0],[5,5],[6,0],[7,2]
    ]

    hull = convex_hull(v)

    self.assertTrue(hull[0] == IntervalVector([3,0]))
    self.assertTrue(hull[1] == IntervalVector([6,0]))
    self.assertTrue(hull[2] == IntervalVector([7,2]))
    self.assertTrue(hull[3] == IntervalVector([5,5]))
    self.assertTrue(hull[4] == IntervalVector([3,4.5]))
    self.assertTrue(hull[5] == IntervalVector([1,4]))
    self.assertTrue(hull[6] == IntervalVector([1,3]))
    self.assertTrue(hull[7] == IntervalVector([1.5,2]))
    self.assertTrue(hull[8] == IntervalVector([2,1]))
    self.assertTrue(len(hull) == 9)

    v = [
      [0.307007,0.0340742],
      [0.0340742,0.307],
      [0.1,0.2],[0.1,0.2],[0.2,0.1],[0.1,0.1]
    ]

    hull = convex_hull(v)

    self.assertTrue(hull[0] == IntervalVector([0.307007,0.0340742]))
    self.assertTrue(hull[1] == IntervalVector([0.0340742,0.307]))
    self.assertTrue(hull[2] == IntervalVector([0.1,0.1]))
    self.assertTrue(len(hull) == 3)

    # Other test

    v = [
      [-4041.935273669676917052129283547401428223,-5492.667604696881426207255572080612182617],
      [-2103.177277725693329557543620467185974121,-5492.667604696881426207255572080612182617],
      [5720.923292917194885376375168561935424805,-975.4210340695084369144751690328121185303],
      [9206.843580880462468485347926616668701172,5062.370015818080901226494461297988891602],
      [52.79381299725321952109879930503666400909,5062.370015818080901226494461297988891602],
      [-4041.935273669676917052129283547401428223-1,-5592.667604696874150249641388654708862305],
      [9206.843580880462468485347926616668701172,6551.674997467660432448610663414001464844],
      [52.79381299725321952109879930503666400909,6551.674997467660432448610663414001464844],
      [-4041.935273669676917052129283547401428223,-540.603823869623056452837772667407989502]
    ]
    v_save = v

    hull = convex_hull(v)

    self.assertTrue(hull[0] == v_save[5])
    self.assertTrue(hull[1] == v_save[1])
    self.assertTrue(hull[2] == v_save[2])
    self.assertTrue(hull[3] == v_save[3])
    self.assertTrue(hull[4] == v_save[6])
    self.assertTrue(hull[5] == v_save[7])
    self.assertTrue(hull[6] == v_save[8])
    self.assertTrue(len(hull) == 7)

if __name__ ==  '__main__':
  unittest.main()