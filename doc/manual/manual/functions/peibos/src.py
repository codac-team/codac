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

class TestToolsManual(unittest.TestCase):

  def tests_tools_manual_peibos(test):

    # [peibos-1-beg]
    # Definition of f
    y_2d = VectorVar(2)
    a,b = 1.4,0.3
    f_2d = AnalyticFunction([y_2d],[y_2d[1]+1-a*sqr(y_2d[0]),b*y_2d[0]])

    # Definition of the gnomonic atlas
    X_2d = VectorVar(1)
    psi0_2d = AnalyticFunction([X_2d],[sin(X_2d[0]*PI/4.),cos(X_2d[0]*PI/4.)])
    id_2d = OctaSym([1, 2])
    s = OctaSym([-2, 1])

    # Call to PEIBOS
    v_par_2d = PEIBOS(f_2d,psi0_2d,[id_2d,s,s*s,s.invert()],0.2)
    # [peibos-1-end]

    # [peibos-2-beg]
    # Definition of f
    y_3d = VectorVar(3)
    f_3d = AnalyticFunction([y_3d],[sqr(y_3d[0])-sqr(y_3d[1])+y_3d[0],2*y_3d[0]*y_3d[1]+y_3d[1],y_3d[2]])

    # Definition of the gnomonic atlas
    X_3d = VectorVar(2)
    psi0_3d = AnalyticFunction([X_3d],[1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))])
    id_3d = OctaSym([1, 2, 3])
    s1 = OctaSym([-2, 1, 3])
    s2 = OctaSym([3, 2, -1])

    # Call to PEIBOS
    v_par_3d = PEIBOS(f_3d,psi0_3d,[id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()],0.2)
    # [peibos-2-end]

if __name__ ==  '__main__':
  unittest.main()