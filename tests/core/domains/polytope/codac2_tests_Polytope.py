#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math


class TestPolytope(unittest.TestCase):
  facets = [[Row([-1,20,82]),80],
           [Row([-20.5,10,1]),0],
           [Row([3.9,1,-3.8]),4],
           [Row([1,-20,-2]),0],
           [Row([-1,10,-38]),0],
           [Row([1,-10,-2]),0]]

##   polytope with vertices
##       (0,0,0) , (2,0,1) , (1,0.1,0) , (0,-0.1,1), (1,2,0.5)
##     inequalities
##       -x + 20y + 82z <= 80        x - 20y - 2z <= 0
##       -20.5x + 10y +z <= 0        -x + 10y - 38z <= 0
##       3.9x + y - 3.8z <= 4        x -10y - 2z <= 0
##     + "redundant" inequality 
##       -2.1 x + 2y + 4.2z <= 4 

  def test_polytope(self):
     p = Polytope(IntervalVector([[-4,4],[-4,4],[-4,4]]),self.facets,True)
     self.assertFalse(p.is_empty())
     self.assertFalse(p.is_flat())
     self.assertTrue(Approx(p.box(),1e-8)==IntervalVector([[0.,2.],[-0.1,2.],[0.,1.]]))
     self.assertTrue(Approx(p.bound_row(Row([-2.1,2.0,4.2])),1e-8)==4.0)
     self.assertTrue(Approx(p.bound_row(Row([1.0,1.0,1.0])),1e-8)==3.5)
     self.assertTrue(p.contains(IntervalVector([1.0,1.0,0.5]))==BoolInterval.TRUE)
#     self.assertTrue(p.contains(IntervalVector([1.0,1.0,61.0/82.0]))==BoolInterval.UNKNOWN)
     self.assertTrue(p.contains(IntervalVector([1.0,1.0,1.0]))==BoolInterval.FALSE)
     
  def test_polytope_subset(self):
    p = Polytope(IntervalVector([[-4,4],[-4,4],[-4,4]]),self.facets,True)
    q = Polytope(IntervalVector([[0.2,0.4],[0.0,0.3],[0.3,0.5]]))
    self.assertTrue(q.is_subset(p))
    r = Polytope(3,True) # empty polyhedron
    self.assertTrue(r.is_empty())
    self.assertTrue(r.is_subset(q))

  def test_polytope_transformation(self):
    p = Polytope(IntervalVector([[-4,4],[-4,4],[-4,4]]),self.facets,True)
    q = Polytope(p)  # or q = Polytope()    q.assign(p)
    q.inflate(0.5)
    self.assertTrue(p.is_subset(q))
    r = Polytope(p)
    r.inflate_ball(0.5)
    self.assertTrue(r.is_subset(q))
    self.assertFalse(q.is_subset(r))
    s = Polytope(p)
    s.unflat(2,0.5)
    self.assertTrue(s.is_subset(r))
    t = Polytope(p)
    t.homothety(IntervalVector([0,0,-0.5]),2)
    self.assertTrue(Approx(t.bound_row(Row([1.0,1.0,1.0])),1e-8)==7.5)
    u = Polytope(t)
    u.meet_with_polytope(p)
    self.assertTrue(u.is_subset(p))
    self.assertTrue(u.is_subset(t))
    v = Polytope.union_of_polytopes([ p,t ])
    self.assertTrue(p.is_subset(v))
    self.assertTrue(t.is_subset(v))
    t.inflate(0.1)
    self.assertFalse(t.is_subset(v))
##  transformation : rotation of pi/3 degrees (reverse), center (1,0,0) 
    M = IntervalMatrix([[cos(PI/3),sin(PI/3),0],[-sin(PI/3),cos(PI/3),0],
                        [0,0,1]])
    P = IntervalVector([1-cos(PI/3),sin(PI/3),0])
    w = p.reverse_affine_transform(M,P,IntervalVector([[-4,4],[-4,4],[-4,4]]))
    self.assertTrue(Approx(w.box(),1e-8)==
                    IntervalVector([[1.0-2*math.sin(PI/3),1.5],
                                    [-math.sin(PI/3)-0.1*math.cos(PI/3),1],
                                    [0,1]]))
  
      
if __name__ ==  '__main__':
  unittest.main()
