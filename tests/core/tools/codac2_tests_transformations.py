#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys

class TestTransformations(unittest.TestCase):

  def tests_affine_transformation(self):

    # Building the src trajectory analytically
    
    t = ScalarVar()
    f_src = AnalyticFunction([t], [
      cos(2*t)+0.1*cos(10*t),
      2*sin(t)+0.1*sin(10*t)
    ])

    src = AnalyticTraj(f_src,[-1,3]).sampled(0.01)

    # The dst trajectory is obtained analytically with a
    # transformation described by the parameters:

    a = PI + PI/3 # rotation angle
    b = 2.5 # scaling
    T = Vector([ -3.2, 2.5 ]) # translation
    f_dst = AnalyticFunction([t], [
      b*cos(a)*f_src(t)[0]-b*sin(a)*f_src(t)[1] + T[0] + 0.05*cos(100*t),
      b*sin(a)*f_src(t)[0]+b*cos(a)*f_src(t)[1] + T[1] + 0.05*sin(100*t)
    ])

    dst = AnalyticTraj(f_dst,[-1,3]).sampled(0.01)

    # Computing the transformation

    tr = affine_transformation(src, dst)

    # Reconstructing the dst trajectory using the estimated transformation

    dst_estim = SampledVectorTraj()
    for ti,src_i in src:
      dst_estim.set(ti, tr*src_i)

    scale = tr.linear().col(0).norm()
    rotation_matrix = tr.linear() / scale
    rotation_angle = atan2(rotation_matrix(1, 0), rotation_matrix(0, 0))
    translation = tr.translation()

    self.assertTrue(Approx(scale,1e-3) == b)
    self.assertTrue(Approx(rotation_angle,1e-3) == (a-2*PI))
    self.assertTrue(Approx(Vector(translation),1e-3) == T)

if __name__ ==  '__main__':
  unittest.main()