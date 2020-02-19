#!/usr/bin/env python

import unittest
import pyibex
from pyibex_tube import *
import numpy as np

class TestTube(unittest.TestCase):

  def setUp(self):
    tube = Tube(Interval(0.,46.), 0.1, Interval(10.,11.));
    self.tube = tube

  def test_Init(self):
    self.assertTrue(self.tube.volume() == 46)
    #print(cos(self.tube).codomain())
    #self.assertTrue(cos(self.tube).codomain() == cos(Interval(10.,11.)))

  #def test_Serialize(self):
  #  self.tube.serialize("/tmp/x.tube")


if __name__ == '__main__':

  unittest.main()