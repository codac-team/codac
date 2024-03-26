#!/usr/bin/env python

import unittest
from codac import *

class TestActions(unittest.TestCase):

  def tests_action(self):

    x = Interval()
    self.assertTrue(x.is_unbounded())
      
if __name__ ==  '__main__':
  unittest.main()