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

class TestTDomain(unittest.TestCase):

  def test_tdomain(self):

    tdomain = create_tdomain()
    tdomain.sample(Interval(0,1), 0.5)
    self.assertTrue(tdomain.nb_tslices() == 4)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))
    
    vector_tslices = tdomain.tslices_vector()

    self.assertTrue(len(vector_tslices) == 4)
    self.assertTrue(vector_tslices[0] == Interval(-oo,0))
    self.assertTrue(vector_tslices[1] == Interval(0,0.5))
    self.assertTrue(vector_tslices[2] == Interval(0.5,1))
    self.assertTrue(vector_tslices[3] == Interval(1,oo))

    self.assertTrue(tdomain.tslice(-10.) == Interval(-oo,0))
    self.assertTrue(tdomain.tslice(-120.) == Interval(-oo,0))
    self.assertTrue(tdomain.tslice(0.2) == Interval(0,0.5))
    self.assertTrue(tdomain.tslice(5540.2) == Interval(1,oo))

    self.assertTrue(tdomain.nb_tubes() == 0)
    x = SlicedTube(tdomain, IntervalVector(2))
    self.assertTrue(tdomain.nb_tubes() == 1)

    def outside_scope(): # new scope
      v = SlicedTube(tdomain, IntervalVector(3))
      self.assertTrue(tdomain.nb_tubes() == 2)
      # end of scope, removing the tube

    outside_scope()
    self.assertTrue(tdomain.nb_tubes() == 1)

  def test_degenerated_tdomain(self):

    tdomain = create_tdomain(Interval(1))
    self.assertTrue(tdomain.nb_tslices() == 1)
    self.assertTrue(tdomain.t0_tf() == Interval(1))
    self.assertTrue(tdomain.nb_tubes() == 0)
    
    vector_tslices = tdomain.tslices_vector()
      
    self.assertTrue(len(vector_tslices) == 1)
    self.assertTrue(vector_tslices[0] == Interval(1))

  def test_tdomain_with_gates(self):

    tdomain = create_tdomain(Interval(0,1), 0.5, True)
    self.assertTrue(tdomain.nb_tslices() == 5)
    self.assertTrue(tdomain.t0_tf() == Interval(0,1))
    self.assertTrue(tdomain.nb_tubes() == 0)

    vector_tslices = tdomain.tslices_vector()

    self.assertTrue(len(vector_tslices) == 5)
    self.assertTrue(vector_tslices[0] == Interval(0))
    self.assertTrue(vector_tslices[1] == Interval(0,0.5))
    self.assertTrue(vector_tslices[2] == Interval(0.5))
    self.assertTrue(vector_tslices[3] == Interval(0.5,1))
    self.assertTrue(vector_tslices[4] == Interval(1,1))

    self.assertTrue(tdomain.tslice(0.) == Interval(0))
    self.assertTrue(tdomain.tslice(0.1) == Interval(0,0.5))
    self.assertTrue(tdomain.tslice(0.5) == Interval(0.5))
    self.assertTrue(tdomain.tslice(0.6) == Interval(0.5,1))
    self.assertTrue(tdomain.tslice(1.) == Interval(1))
    
  def test_tdomain_with_sampling(self):

    tdomain = create_tdomain()
    tdomain.sample(1.)
    self.assertTrue(tdomain.nb_tslices() == 2)
    tdomain.sample(10.,False) # no gate
    self.assertTrue(tdomain.nb_tslices() == 3)
    tdomain.sample(10.,True) # second sampling with gate
    self.assertTrue(tdomain.nb_tslices() == 4)
    tdomain.sample(10.,True) # no more action
    self.assertTrue(tdomain.nb_tslices() == 4)

    vector_tslices = tdomain.tslices_vector()

    self.assertTrue(len(vector_tslices) == 4)
    self.assertTrue(vector_tslices[0] == Interval(-oo,1))
    self.assertTrue(vector_tslices[1] == Interval(1,10))
    self.assertTrue(vector_tslices[2] == Interval(10))
    self.assertTrue(vector_tslices[3] == Interval(10,oo))

  def test_unbounded_tdomain(self):

    tdomain = create_tdomain()
    self.assertTrue(tdomain.nb_tslices() == 1)
    self.assertTrue(tdomain.t0_tf() == Interval(-oo,oo))

    vector_tslices = tdomain.tslices_vector()

    self.assertTrue(len(vector_tslices) == 1)
    self.assertTrue(vector_tslices[0] == Interval(-oo,oo))

if __name__ ==  '__main__':
  unittest.main()