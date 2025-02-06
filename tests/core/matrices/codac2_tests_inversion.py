#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)


import unittest
from codac import *

class TestInversion(unittest.TestCase):

  def tests_Inversion_1(self):

    x = Matrix([
      [ 1, 2, 0 ],
      [ 3, 4, 1 ],
      [ 0, 1, 0 ],
    ])

    y = inverse_enclosure(x)
    self.assertTrue((x*y).contains(Matrix.eye(3,3)))


  def tests_Inversion_2(self):

    x = IntervalMatrix([
      [ [0.0,0.0], [-0.1,-0.1], [0.2,0.2]  ],
      [ [0.0,0.0], [-0.2,-0.2], [0.1,0.1] ],
      [ [0.1,0.1], [-0.1,-0.1], [0.1,0.1] ]
    ])

    y = infinite_sum_enclosure(x)
    #self.assertTrue(Approx(y.max_rad())==Interval(5.0)/Interval(2.0))

    z = inverse_enclosure(x)
    self.assertTrue(z(1,2)==Interval(0))
    self.assertTrue(Approx(z*x)==IntervalMatrix.eye(3,3))


  def tests_Inversion_3(self):

    u = Matrix([
       [ 1,2,3 ],
       [ 1,3,5 ],
       [ 3,4,5 ]
     ])

    v = inverse_enclosure(u)
    self.assertTrue((u*v).contains(Matrix.eye(3,3)))


if __name__ ==  '__main__':
  unittest.main()