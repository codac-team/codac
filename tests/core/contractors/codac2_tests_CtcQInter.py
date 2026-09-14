#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcQInter(unittest.TestCase):
  
  def test_CtcQInter(self):
    
    c1 = CtcWrapper([[-10,10],[-2,2]])
    c2 = CtcWrapper([[-12,2],[0,4]])

    c3 = c1 & c2
    
    # Testing constructors
    test_construct_1 = CtcQInter(1, c1,c2)
    self.assertTrue(test_construct_1.nb() == 2)
    test_construct_2 = CtcQInter(1, c1,c2,c3) # different types
    self.assertTrue(test_construct_2.nb() == 3)
    
    # Testing constructors (lists)
    test_construct_1 = CtcQInter(1, [c1,c2])
    self.assertTrue(test_construct_1.nb() == 2)
    test_construct_2 = CtcQInter(1, [c1,c2,c3]) # different types
    self.assertTrue(test_construct_2.nb() == 3)

if __name__ ==  '__main__':
  unittest.main()