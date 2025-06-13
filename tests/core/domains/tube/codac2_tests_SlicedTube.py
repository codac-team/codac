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
import math

def return_a_tube():
  return SlicedTube(
    create_tdomain(Interval(0,2),0.5),
    IntervalVector.constant(3,Interval(-1.5,1)))

class TestSlicedTube(unittest.TestCase):

  def test_tdomain_with_sampling_and_values(self):

    tdomain = create_tdomain()
    self.assertTrue(tdomain.nb_tslices() == 1)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    x = SlicedTube(tdomain, IntervalVector(1))
    x.set(IntervalVector([[1,5]]), [0,1])
    x.set(IntervalVector([[2,8]]), [1,2])
    x.set(IntervalVector([[6,9]]), [2,3])

    # Checking structure
    v = []
    for s in x:
      v.append(s)

    self.assertTrue(v[0].t0_tf() == Interval(-oo,0))
    self.assertTrue(v[0].codomain() == IntervalVector([[-oo,oo]]))
    self.assertTrue(v[1].t0_tf() == Interval(0,1))
    self.assertTrue(v[1].codomain() == IntervalVector([[1,5]]))
    self.assertTrue(v[2].t0_tf() == Interval(1,2))
    self.assertTrue(v[2].codomain() == IntervalVector([[2,8]]))
    self.assertTrue(v[3].t0_tf() == Interval(2,3))
    self.assertTrue(v[3].codomain() == IntervalVector([[6,9]]))
    self.assertTrue(v[4].t0_tf() == Interval(3,oo))
    self.assertTrue(v[4].codomain() == IntervalVector([[-oo,oo]]))

    self.assertTrue(tdomain.tslice(-1.) == Interval(-oo,0))
    self.assertTrue(tdomain.tslice(0.) == Interval(0,1))
    self.assertTrue(tdomain.tslice(0.01) == Interval(0,1))
    self.assertTrue(tdomain.tslice(1) == Interval(1,2))
    self.assertTrue(tdomain.tslice(2) == Interval(2,3))
    self.assertTrue(tdomain.tslice(previous_float(3.)) == Interval(2,3))
    self.assertTrue(tdomain.tslice(3) == Interval(3,oo))
    self.assertTrue(tdomain.tslice(next_float(3.)) == Interval(3,oo))

    self.assertTrue(tdomain.nb_tslices() == 5) # with [-oo,0] and [3,oo]
    self.assertTrue(x(Interval(0,3)) == IntervalVector([[1,9]]))
    self.assertTrue(x(-1) == IntervalVector(1))
    self.assertTrue(x(0.5) == IntervalVector([[1,5]]))
    self.assertTrue(x(1.5) == IntervalVector([[2,8]]))
    self.assertTrue(x(2.5) == IntervalVector([[6,9]]))
    # No gates: testing values between slices
    self.assertTrue(x(1.) == IntervalVector([[2,5]]))
    self.assertTrue(x(2.) == IntervalVector([[6,8]]))
    self.assertTrue(x(3.) == IntervalVector([[6,9]]))
    self.assertTrue(x(999.) == IntervalVector(1))
    
    s0 = x.first_slice()
    self.assertTrue(s0.t0_tf() == Interval(-oo,0))
    self.assertTrue(s0.codomain() == IntervalVector([[-oo,oo]]))
    s1 = s0.next_slice()
    self.assertTrue(s1.t0_tf() == Interval(0,1))
    self.assertTrue(s1.codomain() == IntervalVector([[1,5]]))
    s2 = s1.next_slice()
    self.assertTrue(s2.t0_tf() == Interval(1,2))
    self.assertTrue(s2.codomain() == IntervalVector([[2,8]]))
    s3 = s2.next_slice()
    self.assertTrue(s3.t0_tf() == Interval(2,3))
    self.assertTrue(s3.codomain() == IntervalVector([[6,9]]))
    s4 = s3.next_slice()
    self.assertTrue(s4.t0_tf() == Interval(3,oo))
    self.assertTrue(s4.codomain() == IntervalVector([[-oo,oo]]))

    self.assertTrue(tdomain.nb_tslices() == 5)
    tdomain.sample(1.3)
    self.assertTrue(tdomain.nb_tslices() == 6)
    self.assertTrue(s2.t0_tf() == Interval(1,1.3))
    self.assertTrue(s2.codomain() == IntervalVector([[2,8]]))
    s2bis = s2.next_slice()
    self.assertTrue(s2bis.t0_tf() == Interval(1.3,2))
    self.assertTrue(s2bis.codomain() == IntervalVector([[2,8]]))
    self.assertTrue(s3.t0_tf() == Interval(2,3))
    self.assertTrue(s3.codomain() == IntervalVector([[6,9]]))
  
  def test_sampling_inside_tdomain(self):

    tdomain = create_tdomain()
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 1)
    tdomain.sample(1., False)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(1., False)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(1., True)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 3)
    it = tdomain.sample(10., True)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 5)
    self.assertTrue(it == Interval(10.))
    it = tdomain.sample(15., False)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    self.assertTrue(tdomain.nb_tslices() == 6)
    self.assertTrue(it == Interval(15.,oo))

  def test_sampling_outside_tdomain(self):

    tdomain = create_tdomain(Interval(0,0.5))
    self.assertTrue(tdomain.t0_tf() == Interval(0,0.5))
    self.assertTrue(tdomain.nb_tslices() == 1)
    tdomain.sample(1., False)
    self.assertTrue(tdomain.t0_tf() == Interval(0,1))
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(1., False)
    self.assertTrue(tdomain.t0_tf() == Interval(0,1))
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(1., False)
    self.assertTrue(tdomain.t0_tf() == Interval(0,1))
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(1., True)
    self.assertTrue(tdomain.t0_tf() == Interval(0,1))
    self.assertTrue(tdomain.nb_tslices() == 3)
    it = tdomain.sample(10., True)
    self.assertTrue(tdomain.t0_tf() == Interval(0,10))
    self.assertTrue(tdomain.nb_tslices() == 5)
    self.assertTrue(it == Interval(10.))
    it = tdomain.sample(15., False)
    self.assertTrue(tdomain.t0_tf() == Interval(0,15))
    self.assertTrue(tdomain.nb_tslices() == 6)
    self.assertTrue(it == Interval(10,15))

  def test_basic_SlicedTube(self):

    tdomain = create_tdomain(Interval(0,1), 0.1, False)
    x = SlicedTube(tdomain, IntervalVector(3))

    self.assertTrue(x.size() == 3)
    self.assertTrue(x.tdomain() == tdomain)
    self.assertTrue(x.t0_tf() == Interval(0,1))
    self.assertTrue(x.nb_slices() == tdomain.nb_tslices())
    self.assertTrue(x.nb_slices() == 10)
    self.assertTrue(x.first_slice().t0_tf() == Interval(0,0.1))
    self.assertTrue(Approx(x.last_slice().t0_tf()) == Interval(0.9,1))
    self.assertTrue(x.codomain() == IntervalVector(3))
    x.set(IntervalVector.constant(3, Interval(-10,10)))
    self.assertTrue(x.codomain() == IntervalVector.constant(3, Interval(-10,10)))

    # Eval
    self.assertTrue(tdomain.nb_tubes() == 1)
    self.assertTrue(x(Interval(-oo,oo)) == IntervalVector(3))
    self.assertTrue(x(Interval(-1,1)) == IntervalVector(3))
    self.assertTrue(x(tdomain.t0_tf()) == x.codomain())
    self.assertTrue(x(-42.) == IntervalVector(3))

    # Eval: affectation at scalar t
    self.assertTrue(tdomain.nb_tslices() == 10)
    x.set(IntervalVector.constant(3,Interval(2.,3.)), -42.)
    self.assertTrue(tdomain.nb_tslices() == 12)

    # Checking structure
    v = []
    for s in x:
      v.append(s)
    self.assertTrue(v[0].t0_tf() == Interval(-42.))
    self.assertTrue(v[0].codomain() == IntervalVector.constant(3,Interval(2.,3.)))
    self.assertTrue(v[1].t0_tf() == Interval(-42.,0.))
    self.assertTrue(v[1].codomain() == IntervalVector(3))
    self.assertTrue(v[2].t0_tf() == Interval(0.,0.1))
    self.assertTrue(v[2].codomain() == IntervalVector.constant(3,Interval(-10,10)))
    self.assertTrue(v[3].t0_tf() == Interval(0.1,0.2))
    self.assertTrue(v[3].codomain() == IntervalVector.constant(3,Interval(-10,10)))

    self.assertTrue(x(-42.) == IntervalVector.constant(3,Interval(2.,3.)))
    self.assertTrue(x(previous_float(-42.)) == IntervalVector(3))
    self.assertTrue(x(next_float(-42.)) == IntervalVector(3))

    # Eval: affectation at interval t
    self.assertTrue(x.codomain() == IntervalVector(3))
    self.assertTrue(tdomain.nb_tslices() == 12)
    x.set(IntervalVector.constant(3,Interval(9.,10.)), Interval(44,55))
    self.assertTrue(tdomain.nb_tslices() == 14)

    v.clear()
    for s in x:
      v.append(s)
    self.assertTrue(Approx(v[11].t0_tf()) == Interval(0.9,1))
    self.assertTrue(v[11].codomain() == IntervalVector.constant(3,Interval(-10,10)))
    self.assertTrue(v[12].t0_tf() == Interval(1,44))
    self.assertTrue(v[12].codomain() == IntervalVector(3))
    self.assertTrue(v[13].t0_tf() == Interval(44,55))
    self.assertTrue(v[13].codomain() == IntervalVector.constant(3,Interval(9.,10.)))

    self.assertTrue(x(Interval(44,55)) == IntervalVector.constant(3,Interval(9.,10.)))
    self.assertTrue(x(previous_float(44.)) == IntervalVector(3))
    self.assertTrue(x(next_float(55.)) == IntervalVector(3))

  def test_sliceT(self):

    tdomain = create_tdomain(Interval(0,1), 0.1)
    x = SlicedTube(tdomain, IntervalVector(2))
    self.assertTrue(x.nb_slices() == 10)
    # C++ like: self.assertTrue(tdomain.tslice(-oo) == tdomain.end())
    # C++ like: self.assertTrue(tdomain.tslice(oo) == tdomain.end())

    for s in x:
      s.set(IntervalVector.constant(2,s.t0_tf()))

    v = []
    for s in x:
      v.append(s)

    self.assertTrue(v[0].t0_tf() == Interval(0,0.1))
    self.assertTrue(v[0].input_gate() == IntervalVector.constant(2,Interval(0.,0.1))) # nothing before
    self.assertTrue(v[0].codomain() == IntervalVector.constant(2,Interval(0,0.1)))
    self.assertTrue(v[0].output_gate() == IntervalVector.constant(2,Interval(0.1)))

    self.assertTrue(Approx(v[9].t0_tf()) == Interval(0.9,1.))
    self.assertTrue(v[9].input_gate() == v[8].output_gate())
    self.assertTrue(Approx(v[9].codomain()) == IntervalVector.constant(2,Interval(0.9,1.)))
    self.assertTrue(Approx(v[9].input_gate()) == IntervalVector.constant(2,Interval(0.9)))
    self.assertTrue(Approx(v[9].output_gate()) == IntervalVector.constant(2,Interval(0.9,1.))) # nothing after

  def test_again_1(self):

    tdomain = create_tdomain(Interval(1,10), 0.01, True) # last argument creates "gates" (degenerated slices at scalar timesteps)
    t = ScalarVar()
    x = SlicedTube(tdomain, AnalyticFunction(
      [t],
      [
        sin(sqrt(t)+((t-5)^2)*Interval(-0.01,0.01)),
        cos(t)+sin(t/0.2)*Interval(-0.1,0.1)
      ]))
    u = SlicedTube(tdomain, IntervalVector(2))
    self.assertTrue(x.size() == 2)
    self.assertTrue(Approx(x.codomain(),1e-4) == IntervalVector([[-0.267392,1],[-1.06721,1.06721]]))

  def test_function_returning_a_tube(self):
  
    x = return_a_tube()
    self.assertTrue(x.tdomain().t0_tf() == Interval(0,2))
    self.assertTrue(x.size() == 3)
    self.assertTrue(x.codomain()[1] == Interval(-1.5,1))
  
  def test_setting_values(self):

    tdomain = create_tdomain(Interval(0,10),1.,True) # with gates
    x = SlicedTube(tdomain, Interval(-10,10))
    self.assertTrue(x.codomain() == Interval(-10,10))
    self.assertTrue(x(0.) == Interval(-10,10))
    self.assertTrue(x(Interval(0,1)) == Interval(-10,10))
    self.assertTrue(x(Interval(1,1)) == Interval(-10,10))

  def test_validity_copy_of_tubes(self):

    tdomain = create_tdomain(Interval(0,5), 0.01, True)

    x1 = SlicedTube(tdomain, Interval(-1,1))
    x2 = SlicedTube(tdomain, Interval(1))
    cx1 = SlicedTube(x1) # copy
    cx2 = SlicedTube(x2) # copy

    self.assertTrue(cx1 == x1)
    self.assertTrue(cx2 == x2)
    self.assertTrue(cx1 != x2)
    self.assertTrue(cx2 != x1)

    cx1.set(Interval(42))
    cx2.set(Interval(-3))

    self.assertTrue(cx1 != x1)
    self.assertTrue(cx2 != x2)

  def test_tube_not_empty_if_built_from_a_AnalyticFunction(self):

    tdomain = create_tdomain(Interval(0,5), 0.01, True)
    t = ScalarVar()
    f = AnalyticFunction([t], 5*sin(2*t)+t)
    aa1 = SlicedTube(tdomain, f)
    self.assertTrue(not aa1.is_empty())

  def test_tube_evaluation(self):

    tdomain = create_tdomain(Interval(0,5), 0.1, True)
    t = ScalarVar()
    f = AnalyticFunction([t], 10*cos(t)+t)
    a = SlicedTube(tdomain, f)
    self.assertTrue(Approx(tdomain.tslice(2.)) == Interval(1.900000000000001, 2.000000000000002))
    self.assertTrue(Approx(a(Interval(1,2)),1e-4) == Interval(-2.17496, 7.13757))

if __name__ ==  '__main__':
  unittest.main()