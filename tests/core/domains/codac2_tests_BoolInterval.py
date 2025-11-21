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

  def test_BoolInterval(self):

    # Bitwise AND (&) — Intersection
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

    # Bitwise OR (|) — Union
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

    # Logical AND (&&)
    self.assertTrue(logical_and(BoolInterval.TRUE, BoolInterval.TRUE)  == BoolInterval.TRUE)
    self.assertTrue(logical_and(BoolInterval.TRUE, BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue(logical_and(BoolInterval.TRUE, BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_and(BoolInterval.TRUE, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_and(BoolInterval.FALSE, BoolInterval.TRUE) == BoolInterval.FALSE)
    self.assertTrue(logical_and(BoolInterval.FALSE, BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue(logical_and(BoolInterval.FALSE, BoolInterval.UNKNOWN) == BoolInterval.FALSE)
    self.assertTrue(logical_and(BoolInterval.FALSE, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_and(BoolInterval.UNKNOWN, BoolInterval.TRUE) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_and(BoolInterval.UNKNOWN, BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue(logical_and(BoolInterval.UNKNOWN, BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_and(BoolInterval.UNKNOWN, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_and(BoolInterval.EMPTY, BoolInterval.TRUE) == BoolInterval.EMPTY)
    self.assertTrue(logical_and(BoolInterval.EMPTY, BoolInterval.FALSE) == BoolInterval.EMPTY)
    self.assertTrue(logical_and(BoolInterval.EMPTY, BoolInterval.UNKNOWN) == BoolInterval.EMPTY)
    self.assertTrue(logical_and(BoolInterval.EMPTY, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    # Logical OR (||)
    self.assertTrue(logical_or(BoolInterval.TRUE, BoolInterval.TRUE)  == BoolInterval.TRUE)
    self.assertTrue(logical_or(BoolInterval.TRUE, BoolInterval.FALSE) == BoolInterval.TRUE)
    self.assertTrue(logical_or(BoolInterval.TRUE, BoolInterval.UNKNOWN) == BoolInterval.TRUE)
    self.assertTrue(logical_or(BoolInterval.TRUE, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_or(BoolInterval.FALSE, BoolInterval.TRUE) == BoolInterval.TRUE)
    self.assertTrue(logical_or(BoolInterval.FALSE, BoolInterval.FALSE) == BoolInterval.FALSE)
    self.assertTrue(logical_or(BoolInterval.FALSE, BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_or(BoolInterval.FALSE, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_or(BoolInterval.UNKNOWN, BoolInterval.TRUE) == BoolInterval.TRUE)
    self.assertTrue(logical_or(BoolInterval.UNKNOWN, BoolInterval.FALSE) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_or(BoolInterval.UNKNOWN, BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue(logical_or(BoolInterval.UNKNOWN, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    self.assertTrue(logical_or(BoolInterval.EMPTY, BoolInterval.TRUE) == BoolInterval.EMPTY)
    self.assertTrue(logical_or(BoolInterval.EMPTY, BoolInterval.FALSE) == BoolInterval.EMPTY)
    self.assertTrue(logical_or(BoolInterval.EMPTY, BoolInterval.UNKNOWN) == BoolInterval.EMPTY)
    self.assertTrue(logical_or(BoolInterval.EMPTY, BoolInterval.EMPTY) == BoolInterval.EMPTY)

    # Complement (~)
    self.assertTrue((~BoolInterval.TRUE) == BoolInterval.FALSE)
    self.assertTrue((~BoolInterval.FALSE) == BoolInterval.TRUE)
    self.assertTrue((~BoolInterval.UNKNOWN) == BoolInterval.UNKNOWN)
    self.assertTrue((~BoolInterval.EMPTY) == BoolInterval.EMPTY)

if __name__ ==  '__main__':
  unittest.main()