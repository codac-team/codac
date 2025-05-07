#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcPolygon(unittest.TestCase):

  def tests_CtcPolygon(self):

    # Polygon, defined as a list of vertices
    c = CtcPolygon([[3,-1],[3,4],[5,6],[-1,1]])

    x = IntervalVector(2)
    c.contract(x)
    self.assertTrue(x == IntervalVector([[-1,5],[-1,6]]))

    x = IntervalVector([[3.02,3.16],[2.5,3.2]]) # possible bug
    c.contract(x)
    self.assertTrue(x.is_empty())

  def tests_CtcPolygon_fromCodac1(self):

    # Polygone with hole, defined as a list of edges

    c = CtcPolygon([
      # external border
      [[6,-6], [7,9]],
      [[7,9], [0,5]],
      [[0,5], [-9,8]],
      [[-9,8], [-8,-9]],
      [[-8,-9], [6,-6]],
      # hole
      [[-2,3], [3.5,2]],
      [[3.5,2], [1.5,0.5]],
      [[1.5,0.5], [3,-4]],
      [[3,-4], [-3,-3]],
      [[-3,-3], [-2,3]]
    ])

    #draw_while_paving(IntervalVector([[-10,10],[-10,10]]), c, 0.1)

    # Check a box inside the hole

    x = IntervalVector([[0],[0]]).inflate(0.5)
    #DefaultFigure.draw_box(x,Color::purple())
    c.contract(x)
    self.assertTrue(x.is_empty())

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    _x = IntervalVector(x)
    #DefaultFigure.draw_box(x,Color::purple())
    c.contract(x)
    self.assertTrue(x == _x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    #DefaultFigure.draw_box(x,Color::purple())
    c.contract(x)
    self.assertTrue(x.is_empty())


    # Other polygon, defined as a list of vertices

    c = CtcPolygon([[6,-6],[7,9],[0,5],[-9,8],[-8,-9]])

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    _x = IntervalVector(x)
    c.contract(x)
    self.assertTrue(x == _x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    c.contract(x)
    self.assertTrue(x.is_empty())


if __name__ ==  '__main__':
  unittest.main()