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

class TestSerialization(unittest.TestCase):

  def test_serialization(self):

    v = Vector([2,3,6])

    with open("data.cdc", "wb") as f:
      serialize(f, v)

    v_deserialized = Vector(1)
    with open("data.cdc", "rb") as f:
      deserialize(f, v_deserialized)

    self.assertTrue(v == v_deserialized)

if __name__ ==  '__main__':
  unittest.main()