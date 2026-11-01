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
    
    td.tslice(0.)  # [0]
    td.tslice(0.1) # [0,0.5]
    td.tslice(0.5) # [0.5]
    td.tslice(0.6) # [0.5,1]
    td.tslice(1.)  # [1]
    # [tdomain-class-2-end]

    test.assertTrue(n == 5)
    test.assertTrue(m == 0)
    test.assertTrue(dom == Interval(0,1))

    # [tdomain-class-3-beg]
    td = create_tdomain()
    td.sample(1., False) # [-oo,1],[1,oo]
    td.sample(10., True) # [-oo,1],[1,10],[10],[10,oo]
    # [tdomain-class-3-end]

    # [tdomain-class-4-beg]
    td = create_tdomain([0,2], 1., False) # False: without gates

    x = SlicedTube(td, Interval(0,1))
    v = SlicedTube(td, Interval(-1,1))

    print(x)            # codomain [0,1], 2 temporal elements
    print(v)            # codomain [-1,1], 2 temporal elements

    n = td.nb_tslices() # 2: [0,1],[1,2]
    x.set([0.5,1], 1.3) # local update, will refine the partition at t=1.3
    m = td.nb_tslices() # now 4: [0,1],[1,1.3],[1.3],[1.3,2]

    print(x)            # codomain [0,1], 4 temporal elements
    print(v)            # codomain [-1,1], 4 temporal elements (shared partition refined)
    # [tdomain-class-4-end]

    test.assertTrue(n == 2)
    test.assertTrue(m == 4)

    # [tdomain-class-5-beg]
    td = create_tdomain([0,3], 1., False)
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
    x = SlicedTube(td, Interval(-1,1))     # scalar tube
    y = SlicedTube(td, IntervalVector(2))  # vector tube
    z = SlicedTube(td, IntervalMatrix(2,2)) # matrix tube

    # From an analytic function
    t = ScalarVar()
    f = AnalyticFunction([t], sin(t))
    xf = SlicedTube(td, f) # scalar tube

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
    td = create_tdomain([0,3], 1., False)
    x = SlicedTube(td, Interval())

    x.set([1,5], [0,1])
    x.set([2,8], [1,2])
    x.set([6,9], [2,3])

    x(0.5)      # [1,5]
    x(1.5)      # [2,8]
    x([0,3])    # [1,9]
    x(-1.)     # [-oo,oo]

    # No explicit gates: boundary values come from adjacent-slice intersections
    x(1.)      # [2,5]
    x(2.)      # [6,8]
    x(3.)      # [6,9]
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
    td = create_tdomain([0,2], 1., False)

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

    y = A * b # vector tube
    # [slicedtube-class-7-end]

    # [slicedtube-class-function-eval-beg]
    td = create_tdomain([0,5], 0.1, True)

    t = ScalarVar()
    heading = SlicedTube(td, AnalyticFunction([t], 0.2*sin(t)))
    speed = SlicedTube(td, Interval(1,2))

    h, v = ScalarVar(), ScalarVar()
    f = AnalyticFunction([h,v], [v*cos(h), v*sin(h)])
    velocity = f.tube_eval(heading,speed) # vector tube

    p = VectorVar(2)
    g = AnalyticFunction([p], sqr(p[0])+sqr(p[1]))
    velocity_norm_squared = g.tube_eval(velocity) # scalar tube: ||velocity||^2
    # [slicedtube-class-function-eval-end]

    test.assertTrue(velocity.size() == 2)
    test.assertTrue(velocity_norm_squared.size() == 1)

    td = create_tdomain([0,2])
    x = SlicedTube(td, Interval())

    # [slicedtube-class-8-beg]
    x.inflate(0.2) # constant inflation

    rad = SampledTraj({0.:0.1, 1.:0.3, 2.:0.2})
    x.inflate(rad) # time-varying inflation radius
    # [slicedtube-class-8-end]

    td = create_tdomain()
    x = SlicedTube(td, IntervalVector(3))

    # [slicedtube-class-9-beg]
    # Component and subvector extraction
    x0 = x[0]              # scalar tube
    x12 = x.subvector(1,2) # vector tube
    # [slicedtube-class-9-end]

    pass


  def test_Trajectories_manual(test):
    # [traj-sampled-create-beg]
    depth = SampledTraj_Scalar()
    depth.set(2., 0.)
    depth.set(3., 1.)
    depth.set(4., 2.)

    position = SampledTraj_Vector()
    position.set(Vector([0,1]), 0.)
    position.set([1,2], 1.)  # equivalent shorter notation
    position.set([2,3], 2.)
    # [traj-sampled-create-end]
    test.assertEqual(depth.nb_samples(), 3)

    # [traj-sampled-eval-beg]
    x = SampledTraj({0.: 2., 1.: 4., 2.: 3.})
    value = x(0.5)                # 3.
    range_x = x([0.5,1.5])        # [3,4]
    sample_count = x.nb_samples() # 3
    output_size = x.size()        # 1
    # [traj-sampled-eval-end]
    test.assertEqual(value, 3)
    test.assertEqual(range_x, Interval(3,4))
    test.assertEqual(sample_count, 3)
    test.assertEqual(output_size, 1)

    # [traj-sampled-iterate-beg]
    for ti, xi in x:
      print(ti, xi)
    # [traj-sampled-iterate-end]
    test.assertAlmostEqual(value, 3.)

    # [traj-sampled-components-beg]
    p = SampledTraj({0.: Vector([0.,1.,2.]),
                     1.: Vector([1.,2.,3.])})
    x = p[0]
    yz = p.subvector(1,2)
    # [traj-sampled-components-end]

    # [traj-sampled-time-beg]
    shifted = SampledTraj({0.:0., 1.:1., 2.:0.})
    shifted.truncate_tdomain([0.5,1.5]) # retains interpolated endpoints
    shifted.shift_tdomain(10.)          # time domain now [10.5,11.5]
    shifted.stretch_tdomain([0,1])      # rescale time, not values
    # [traj-sampled-time-end]

    # [traj-ops-common-tdomain-beg]
    x1 = SampledTraj({0.:0, 4.:4})
    x2 = SampledTraj({2.:Vector([1,2]), 5.:Vector([5,6])})
    truncate_to_common_tdomain(x1,x2) # common time domain: [2,4]
    # [traj-ops-common-tdomain-end]
    test.assertEqual(x1.tdomain(), Interval(2,4))
    test.assertEqual(x2.tdomain(), Interval(2,4))

    # [traj-sampled-npz-beg]
    import numpy as np
    import io
    memory = io.BytesIO()
    np.savez(memory,
      t=np.array([0., 0.5, 1.]),
      x=np.array([[0,1], [0.5,1.5], [1,2]]))
    memory.seek(0)  # rewind before reading
    with np.load(memory) as data:
      imported = SampledTraj(data['t'], data['x'])
    # imported is a SampledTraj_Vector
    # [traj-sampled-npz-end]

    # [traj-analytic-create-beg]
    t = ScalarVar()
    f = AnalyticFunction([t], 2*sin(t))
    traj_f = AnalyticTraj([0,2], f) # scalar trajectory

    g = AnalyticFunction([t], [cos(t), sin(t)])
    traj_g = AnalyticTraj([0,2], g) # vector trajectory
    # [traj-analytic-create-end]

    # [traj-analytic-eval-beg]
    y_point = traj_f(0.5)      # scalar value
    y_interval = traj_f([0,1]) # interval enclosure
    p_point = traj_g(0.5)      # vector value
    # [traj-analytic-eval-end]

    # [traj-analytic-sampled-beg]
    f_samples = traj_f.sampled(0.1)
    g_samples = traj_g.sampled(0.1)
    # [traj-analytic-sampled-end]

    # [traj-ops-calculus-beg]
    speed = SampledTraj({0.:0, 1.:1, 2.:2})
    acceleration = speed.derivative() # numerical derivative
    distance = speed.primitive()      # trapezoidal integral, p(0)=0
    doubled_speed = 2. * speed        # pointwise arithmetic
    # [traj-ops-calculus-end]

    # [traj-ops-cartprod-beg]
    x1 = SampledTraj({0.:0, 1.:1})
    x2 = SampledTraj({0.:2, 1.:3})
    x = traj_cart_prod(x1, x2)
    # [traj-ops-cartprod-end]

    # [traj-ops-continuous-beg]
    heading = SampledTraj({0.:6.1, 1.:0.1, 2.:0.2})
    continuous_heading = continuous_traj(heading)
    # [traj-ops-continuous-end]

    # [traj-ops-eval-beg]
    theta = SampledTraj({0.:0, 1.:0.1, 2.:0.2})
    v = SampledTraj({0.:1., 1.:2., 2.:3.})
    theta_var, v_var = ScalarVar(), ScalarVar()
    f = AnalyticFunction([theta_var, v_var], [v_var*cos(theta_var), v_var*sin(theta_var)])
    u = f.traj_eval(theta, v)
    # [traj-ops-eval-end]

    # [traj-ops-asfunction-beg]
    measurements = SampledTraj({0.:1, 1.:2, 2.:1.5})
    measurement_fn = measurements.as_function()
    t = ScalarVar()
    energy = AnalyticFunction([t], sqr(measurement_fn(t)))
    estimated_energy = energy.eval(0.5)
    # [traj-ops-asfunction-end]

    # [traj-ops-tube-beg]
    t = ScalarVar()
    traj = AnalyticTraj([0,2], AnalyticFunction([t],sin(t)))
    td = create_tdomain([0,2], 0.1)
    tube = SlicedTube(td, traj)
    tube.inflate(0.05) # encloses the true signal if its error is bounded by 0.05
    # [traj-ops-tube-end]

    # [traj-ops-serialization-beg]
    input_traj = SampledTraj({0.:1, 1.:1.5, 2.:2})
    with open('trajectory_python.cdc', 'wb') as f:
      serialize(f, input_traj)
    restored = SampledTraj_Scalar()
    with open('trajectory_python.cdc', 'rb') as f:
      deserialize(f, restored)
    # [traj-ops-serialization-end]
    test.assertEqual(restored.nb_samples(), input_traj.nb_samples())


  def test_Trajectory_reference_manual(test):
    # [traj-sampled-grid-beg]
    x = SampledTraj_Scalar({0.:0, 1.:2, 2.:0})
    merged = x.sampled(0.6)         # original samples also retained
    regular = x.sampled(0.6, False) # regular grid and final time only
    clock = SampledTraj_Scalar({0.:0, 0.5:0, 1.5:0, 2.:0})
    aligned = x.sampled_as(clock, False) # x evaluated at clock's timestamps
    # [traj-sampled-grid-end]
    test.assertGreater(merged.nb_samples(), regular.nb_samples())
    test.assertEqual(aligned.nb_samples(),clock.nb_samples())

    # [traj-sampled-construct-alt-beg]
    x = SampledTraj_Scalar([0.,1.,2.], [3,4,5])
    x.set(6.,1.) # replaces value at t=1
    y = SampledTraj_Scalar({0.:3, 1.:6, 2.:5})
    # [traj-sampled-construct-alt-end]
    test.assertEqual(x.nb_samples(), 3)
    test.assertAlmostEqual(x(1.),6.)

    # [traj-analytic-advanced-beg]
    t = ScalarVar()
    a = AnalyticTraj([0,2], AnalyticFunction([t], sqr(t)))
    f = a.as_function()
    g = AnalyticFunction([t], f(t) + 1)
    # [traj-analytic-advanced-end]

    # [traj-ops-pointwise-beg]
    x = SampledTraj_Scalar({0.:0, 1.:1, 2.:2})
    y = SampledTraj_Scalar({0.:2, 1.:3, 2.:4})
    sum_xy = x + y   # computed at matching timestamps
    sq = sqr(x)      # square samples, then interpolate
    sin_x = sin(x)   # transform samples, then interpolate
    # [traj-ops-pointwise-end]
    test.assertAlmostEqual(sum_xy(1),4.)

    # [traj-ops-fun-vector-beg]
    p = SampledTraj({0.: Vector([3,4]), 1.: Vector([0,5])})
    p_var = VectorVar(2)
    f = AnalyticFunction([p_var], sqr(p_var[0]) + sqr(p_var[1]))
    result = f.traj_eval(p)
    # [traj-ops-fun-vector-end]
    test.assertAlmostEqual(result(0),25.)

    # [traj-tubes-mid-beg]
    td = create_tdomain([0,2],0.5,True)
    t = ScalarVar()
    a = AnalyticTraj([0,2],AnalyticFunction([t],sin(t)))
    tube = SlicedTube(td,a)
    midpoint = tube.mid() # approximate midpoint trajectory
    # [traj-tubes-mid-end]
    test.assertGreater(midpoint.nb_samples(),1)

    # [traj-io-csv-beg]
    import csv
    from pathlib import Path
    measurements = SampledTraj_Scalar()
    with (Path(__file__).parent / 'data.csv').open(newline='') as file:
      # The header is parsed automatically.
      for row in csv.DictReader(file, delimiter=';'):
        measurements.set(float(row['value']), float(row['t']))
    # [traj-io-csv-end]
    test.assertEqual(measurements.nb_samples(),3)

  def test_SlicedTube_cross_reference_manual(test):
    # [tube-example-1-beg]
    td = create_tdomain([0,3], 1., False)
    x = SlicedTube(td, Interval()) # [-∞,∞] by default for any time
    x.set([1,5], [0,1])
    x.set([2,8], [1,2])
    x.set([6,9], [2,3])
    y0 = x([0,3])
    y1 = x(1.5)
    y2 = x(1.)
    # [tube-example-1-end]
    test.assertFalse(y0.is_empty())

    # [tube-example-2-beg]
    td = create_tdomain([0,1], 0.1, False)
    x = SlicedTube(td, IntervalVector(3))
    before = td.nb_tslices()
    x.set(IntervalVector.constant(3,[2,3]), -42.)
    after = td.nb_tslices() # the partition was extended and refined
    # [tube-example-2-end]
    test.assertGreater(after, before)

    # [tube-example-3-beg]
    td = create_tdomain([0,5], 0.1, True)
    t = ScalarVar()
    f = AnalyticFunction([t], 10*cos(t) + t)
    x = SlicedTube(td, f) # encloses f(t) over each time slice
    # [tube-example-3-end]
    test.assertFalse(x.is_empty())

    # [tube-example-4-beg]
    td = create_tdomain([6.4,6.5])
    x = SlicedTube(td, Interval(0.759,1.205))
    v = SlicedTube(td, Interval(-0.216,-0.115))
    x.set(Interval(0.781,1.205), 6.4)
    x.set(Interval(0.759,1.194), 6.5)
    CtcDeriv().contract(x,v)
    sx = x.first_slice()
    sv = v.first_slice()
    polygon = sx.polygon_slice(sv)
    # [tube-example-4-end]

    # [tube-example-5-beg]
    td = create_tdomain([0,2], 0.5, True)
    x = SlicedTube(td, Interval(1,2))
    integral_at_one = x.integral(Interval(1))
    integral_over_range = x.integral(Interval(0.5,1.5))
    p = x.primitive(Interval(0))
    # [tube-example-5-end]
    test.assertFalse(integral_at_one.is_empty())

    # [tube-example-6-beg]
    td = create_tdomain([0,1], 0.5, False)
    x1 = SlicedTube(td, Interval(0,1))
    x2 = SlicedTube(td, IntervalVector([[1,2],[-1,1]]))
    x = tube_cart_prod(x1, x2) # vector tube of dimension 3
    # [tube-example-6-end]
    test.assertEqual(x.size(), 3)

    # [tube-parity-algebra-beg]
    td = create_tdomain([0,2], 0.5, True)
    x = SlicedTube(td, Interval(0,2))
    y = SlicedTube(td, Interval(1,3))
    both = x & y             # intersection of enclosures
    either = x | y           # interval hull, not disconnected union
    sum_xy = x + y           # interval arithmetic on every slice
    xy = tube_cart_prod(x,y) # vector tube: SlicedTube_IntervalVector
    # [tube-parity-algebra-end]
    test.assertEqual(xy.size(), 2)

    # [tube-parity-conversion-beg]
    td = create_tdomain([0,2], 0.5, True)
    tube = SlicedTube(td, Interval(-1,1))
    midpoint = tube.mid()                   # an approximation
    middle_curve = SlicedTube(td, midpoint) # encloses the interpolant
    # [tube-parity-conversion-end]
    test.assertGreater(midpoint.nb_samples(), 1)

    # [tube-parity-input-beg]
    measured = SampledTraj_Scalar({0.:1., 1.:1.5, 2.:2.})
    td = create_tdomain([0,2], 0.5, True)
    x = SlicedTube(td, measured) # encloses the piecewise-linear interpolant
    x.inflate(0.2)               # only guaranteed if the bound is justified
    # [tube-parity-input-end]
    test.assertFalse(x.is_empty())

    # [tube-parity-export-beg]
    import io
    td = create_tdomain([0,2], 1., True)
    x = SlicedTube(td, Interval(-1,1))
    csv = io.StringIO()
    csv.write('t0;t1;lb;ub;is_gate\n')
    for s in x:
      t = s.t0_tf()
      bounds = s.codomain()
      csv.write(f'{t.lb()};{t.ub()};{bounds.lb()};{bounds.ub()};{int(s.is_gate())}\n')
    # [tube-parity-export-end]
    test.assertIn('is_gate', csv.getvalue())


  def test_Slice_reference_manual(test):
    # [slice-endpoint-example-beg]
    td = create_tdomain([0,2], 1., False)
    x = SlicedTube(td, Interval())
    x.set([1,5], [0,1])
    x.set([2,8], [1,2])
    s = x.first_slice()
    boundary = s.output_gate() # [2,5]
    # [slice-endpoint-example-end]

    # [slice-invert-example-beg]
    td = create_tdomain([0,1])
    x = SlicedTube(td, Interval(0,10))
    x.set([2,3], 0)
    x.set([5,6], 1)
    s = x.first_slice()
    possible_times = s.invert([2.5,6], [0.2,0.5])
    # [slice-invert-example-end]

if __name__ ==  '__main__':
  unittest.main()