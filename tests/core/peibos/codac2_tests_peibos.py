#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestPeibos(unittest.TestCase):

  def test_peibos(self):

    # 2D checks of the PEIBOS algorithm

    y_2d = VectorVar(2)
    f_2d = AnalyticFunction([y_2d],[sqr(y_2d[0])-sqr(y_2d[1])+y_2d[0],2.*y_2d[0]*y_2d[1] + y_2d[1]])

    X_2d = VectorVar(1)
    psi0_2d = AnalyticFunction([X_2d],[cos(X_2d[0]*PI/4.),sin(X_2d[0]*PI/4.)])

    id_2d = OctaSym([1, 2])
    s = OctaSym([-2, 1])

    v_par_2d = PEIBOS(f_2d,psi0_2d,[id_2d,s,s*s,s.invert()],0.25,[-0.2,0.])

    b0 = Vector([-0.5,0.0])
    b1 = Vector([0.0,1.45])
    b2 = Vector([-1.165,0.0])

    count_b0, count_b1, count_b2 = 0, 0, 0

    for p in v_par_2d:
      if (p.contains(b0))==BoolInterval.TRUE:
        count_b0 += 1
      if (p.contains(b1))==BoolInterval.TRUE:
        count_b1 += 1
      if (p.contains(b2))==BoolInterval.TRUE:
        count_b2 += 1

    self.assertTrue(count_b0 == 0)
    self.assertTrue(count_b1 == 1)
    self.assertTrue(count_b2 == 2)

    # 3D checks of the PEIBOS algorithm

    y_3d = VectorVar(3)
    f_3d = AnalyticFunction([y_3d],[y_3d[0],y_3d[1],y_3d[2]])

    X_3d = VectorVar(2)
    psi0_3d = AnalyticFunction([X_3d],[1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))])

    id_3d = OctaSym([1, 2, 3])
    s1 = OctaSym([-2, 1, 3])
    s2 = OctaSym([3, 2, -1])

    v_par_3d = PEIBOS(f_3d,psi0_3d,[id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()],2.0)

    self.assertTrue(len(v_par_3d) == 6)

    self.assertTrue(Approx(v_par_3d[0].z,1e-6) == Vector([1.,0.,0.]))
    self.assertTrue(Approx(v_par_3d[1].z,1e-6) == Vector([0.,1.,0.]))
    self.assertTrue(Approx(v_par_3d[2].z,1e-6) == Vector([-1,0.,0.]))
    self.assertTrue(Approx(v_par_3d[3].z,1e-6) == Vector([0.,-1.,0.]))
    self.assertTrue(Approx(v_par_3d[4].z,1e-6) == Vector([0.,0.,-1.]))
    self.assertTrue(Approx(v_par_3d[5].z,1e-6) == Vector([0.,0.,1.]))

    a = 4.35066

    self.assertTrue(Approx(v_par_3d[0].A,1e-5) == Matrix([[0.,0.,a],[a+1,0.,0.],[0.,a+1,0.]]))
    self.assertTrue(Approx(v_par_3d[1].A,1e-5) == Matrix([[-(a+1),0.,0.],[0.,0.,a],[0.,a+1,0.]]))
    self.assertTrue(Approx(v_par_3d[2].A,1e-5) == Matrix([[0.,0.,a],[-(a+1),0.,0.],[0.,a+1,0.]]))
    self.assertTrue(Approx(v_par_3d[3].A,1e-5) == Matrix([[a+1,0.,0.],[0.,0.,a],[0.,a+1,0.]]))
    self.assertTrue(Approx(v_par_3d[4].A,1e-5) == Matrix([[0.,a+1,0.],[a+1,0.,0.],[0.,0.,a]]))
    self.assertTrue(Approx(v_par_3d[5].A,1e-5) == Matrix([[0.,-(a+1),0.],[a+1,0.,0.],[0.,0.,a]]))
      
if __name__ ==  '__main__':
  unittest.main()