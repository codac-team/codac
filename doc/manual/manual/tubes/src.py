#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestTubeManual(unittest.TestCase):

  def tests_TDomain_manual(test):

    # [tdomain-class-1-beg]
    td0 = create_tdomain()                  # one unbounded slice [-oo,oo]
    td1 = create_tdomain([0,1])             # one slice [0,1]
    td2 = create_tdomain([0,1], 0.5, False) # [0,0.5],[0.5,1]
    td3 = create_tdomain([0,1], 0.5, True)  # [0],[0,0.5],[0.5],[0.5,1],[1]
    # [tdomain-class-1-end]

    # [tdomain-class-2-beg]
    td = create_tdomain([0,1], 0.5, True)

    dom = td.t0_tf()    # [0,1]
    n = td.nb_tslices() # 5
    m = td.nb_tubes()   # 0 initially
    
    td.tslice(0.0)  # [0]
    td.tslice(0.1)  # [0,0.5]
    td.tslice(0.5)  # [0.5]
    td.tslice(0.6)  # [0.5,1]
    td.tslice(1.0)  # [1]
    # [tdomain-class-2-end]

    test.assertTrue(n == 5)
    test.assertTrue(m == 0)
    test.assertTrue(dom == Interval(0,1))

    # [tdomain-class-3-beg]
    td = create_tdomain()
    td.sample(1.0, False) # [-oo,1],[1,oo]
    td.sample(10.0, True) # [-oo,1],[1,10],[10],[10,oo]
    # [tdomain-class-3-end]

    # [tdomain-class-4-beg]
    td = create_tdomain([0,2], 1.0, False) # False: without gates

    x = SlicedTube(td, Interval(0,1))
    v = SlicedTube(td, Interval(-1,1))

    print(x)            # outputs [0,2]->[0,1], 2 slices
    print(v)            # outputs [0,2]->[0,1], 2 slices

    n = td.nb_tslices() # 2: [0,1],[1,2]
    x.set([0.5,1], 1.3) # local update, will refine the partition at t=1.3
    m = td.nb_tslices() # now 4: [0,1],[1,1.3],[1.3],[1.3,2]

    print(x)            # outputs [0,2]->[-1,1], 4 slices
    print(v)            # outputs [0,2]->[-1,1], 4 slices (v is also impacted by x.set(..))
    # [tdomain-class-4-end]

    test.assertTrue(n == 2)
    test.assertTrue(m == 4)

    # [tdomain-class-5-beg]
    td = create_tdomain([0,3], 1.0, False)
    x = SlicedTube(td, Interval(0,1))

    td.nb_tubes()      # 1
    td.truncate([0.5,2.5])

    print(td.t0_tf())  # [0.5,2.5]
    print(x)           # x now uses the truncated shared partition
    # [tdomain-class-5-end]


  def tests_SlicedTube_manual(test):

    fig = Figure2D("Tube", GraphicOutput.VIBES)
    fig.set_window_properties([50,50],[800,400])

    # [slicedtube-class-1-beg]
    td = create_tdomain([0,10], 0.1, True)

    # Sliced tube from interval-type codomains
    x = SlicedTube(td, Interval(-1,1))
    y = SlicedTube(td, IntervalVector(2))
    z = SlicedTube(td, IntervalMatrix(2,2))

    # From an analytic function
    t = ScalarVar()
    f = AnalyticFunction([t], sin(t))
    xf = SlicedTube(td, f)

    # From an analytic trajectory or sampled trajectory
    at = AnalyticTraj([0,10], AnalyticFunction([t],cos(t)))
    st = AnalyticTraj([0,10], AnalyticFunction([t],cos(t)+t/10)).sampled(1e-2)
    xu = SlicedTube(td,at) | SlicedTube(td,st) # union (hull) of tubes

    fig.plot_tube(xf, [Color.dark_blue(),Color.light_gray()])
    fig.plot_tube(xu, [Color.dark_blue(),Color.light_gray()])
    # [slicedtube-class-1-end]

    # [slicedtube-class-2-beg]
    td = create_tdomain([0,3])
    x = SlicedTube(td, IntervalVector(2))
    x.set([[1,5],[-oo,2]], [0,1])
    x.set([[2,8],[-oo,3]], [1,2])
    x.set([[6,9],[-oo,4]], [2,3])

    s0 = x.first_slice()
    s1 = s0.next_slice()

    for s in x:
      print(s.t0_tf(), s.codomain())
    # [slicedtube-class-2-end]

    # [slicedtube-class-4-beg]
    td = create_tdomain([0,3], 1.0, False)
    x = SlicedTube(td, Interval())

    x.set([1,5], [0,1])
    x.set([2,8], [1,2])
    x.set([6,9], [2,3])

    x(0.5)      # [1,5]
    x(1.5)      # [2,8]
    x([0,3])    # [1,9]
    x(-1.0)     # [-oo,oo]

    # No explicit gates: boundary values come from adjacent-slice intersections
    x(1.0)      # [2,5]
    x(2.0)      # [6,8]
    x(3.0)      # [6,9]
    # [slicedtube-class-4-end]

    T = Interval(0,10)
    td = create_tdomain(T, 1e-1)

    t = ScalarVar()

    z1 = AnalyticTraj(T, AnalyticFunction([t],cos(t)))
    z2 = AnalyticTraj(T, AnalyticFunction([t],cos(t)+t/10))
    z3 = AnalyticTraj(T, AnalyticFunction([t],sin(t)+t/10))
    z4 = AnalyticTraj(T, AnalyticFunction([t],sin(t))).sampled(1e-2)

    x1 = SlicedTube(td, z1)
    x2 = SlicedTube(td, z2)
    x3 = SlicedTube(td, z3)
    x4 = SlicedTube(td, z4)

    x = x1 | x2 | x3 | x4

    # [slicedtube-class-5-beg]
    v_t = []
    y = Interval(0,0.2)
    x.invert(y,v_t)

    for t in v_t:
      z = cart_prod(t,y)
      DefaultFigure.draw_box(z, Color.red())
    # [slicedtube-class-5-end]

    # [slicedtube-class-6-beg]
    td = create_tdomain([0,2], 1.0, False)

    x = SlicedTube(td, Interval(1,2))
    y = SlicedTube(td, Interval(-1,3))

    z_add = x + y  # addition of two tubes
    z_mul = 2 * x  # multiplication by a scalar/interval
    z_hul = x | y  # hull (union) of two tubes
    z_int = x & y  # intersection of two tubes

    u = sin(x) + exp(y)
    # [slicedtube-class-6-end]

    # [slicedtube-class-7-beg]
    A = SlicedTube(td, IntervalMatrix(2,2))
    b = SlicedTube(td, IntervalVector(2))

    y = A * b
    # [slicedtube-class-7-end]

    td = create_tdomain([0,2])
    x = SlicedTube(td, Interval())

    # [slicedtube-class-8-beg]
    x.inflate(0.2) # constant inflation

    rad = SampledTraj({0.0:0.1, 1.0:0.3, 2.0:0.2})
    x.inflate(rad) # time-varying inflation radius
    # [slicedtube-class-8-end]

    td = create_tdomain()
    x = SlicedTube(td, IntervalVector(3))

    # [slicedtube-class-9-beg]
    # Component and subvector extraction
    x0 = x[0]
    x12 = x.subvector(1,2)
    # [slicedtube-class-9-end]

    pass

if __name__ ==  '__main__':
  unittest.main()