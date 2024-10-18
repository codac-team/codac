#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestBoolInterval(unittest.TestCase):

  def tests_BoolInterval(self):
    
    self.assertTrue((BoolInterval.TRUE & BoolInterval.TRUE) == BoolInterval.TRUE)
    self.assertTrue((BoolInterval.TRUE & BoolInterval.FALSE) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.TRUE & BoolInterval.UNKNOWN) == BoolInterval.TRUE)
    self.assertTrue((BoolInterval.TRUE & BoolInterval.EMPTY) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.FALSE & BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue((BoolInterval.FALSE & BoolInterval.UNKNOWN) == BoolInterval.FALSE)
    self.assertTrue((BoolInterval.FALSE & BoolInterval.EMPTY) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.EMPTY & BoolInterval.EMPTY) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.EMPTY & BoolInterval.UNKNOWN) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.UNKNOWN & BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)

    self.assertTrue((BoolInterval.TRUE | BoolInterval.TRUE) == BoolInterval.TRUE)
    self.assertTrue((BoolInterval.TRUE | BoolInterval.FALSE) == BoolInterval.UNKNOWN)
    self.assertTrue((BoolInterval.TRUE | BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue((BoolInterval.TRUE | BoolInterval.EMPTY) == BoolInterval.TRUE)
    self.assertTrue((BoolInterval.FALSE | BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue((BoolInterval.FALSE | BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue((BoolInterval.FALSE | BoolInterval.EMPTY) == BoolInterval.FALSE)
    self.assertTrue((BoolInterval.EMPTY | BoolInterval.EMPTY) == BoolInterval.EMPTY)
    self.assertTrue((BoolInterval.EMPTY | BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue((BoolInterval.UNKNOWN | BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)

if __name__ ==  '__main__':
  unittest.main()