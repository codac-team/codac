#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepPolygon(unittest.TestCase):

  def tests_SepPolygon(self):

    # Polygon, defined as a list of vertices
    p = Polygon([[3,-1],[3,4],[5,6],[-1,1]])
    s = SepPolygon(p)
    c = CtcPolygon(p)

    x = IntervalVector(2)
    inner,outer = s.separate(IntervalVector(2))
    self.assertTrue(inner == IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,5],[-1,6]]))

    y = IntervalVector(2)
    c.contract(y)
    self.assertTrue(y == IntervalVector([[-1,5],[-1,6]]))

    x = IntervalVector([[3.02,3.16],[2.5,3.2]]) # possible bug
    inner,outer = s.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer.is_empty())

  def tests_SepPolygon_fromCodac1(self):

    # Polygone with hole, defined as a list of edges

    s = SepPolygon([
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

    #draw_while_paving(IntervalVector([[-10,10],[-10,10]]), s, 0.1)

    # Check a box inside the hole

    x = IntervalVector([[0],[0]]).inflate(0.5)
    #DefaultFigure.draw_box(x,Color::purple())
    inner,outer = s.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer.is_empty())

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    #DefaultFigure.draw_box(x,Color::purple())
    inner,outer = s.separate(x)
    self.assertTrue(inner.is_empty())
    self.assertTrue(outer == x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    #DefaultFigure.draw_box(x,Color::purple())
    inner,outer = s.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer.is_empty())


    # Other polygon, defined as a list of vertices

    s = SepPolygon([[6,-6],[7,9],[0,5],[-9,8],[-8,-9]])

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    inner,outer = s.separate(x)
    self.assertTrue(inner.is_empty())
    self.assertTrue(outer == x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    inner,outer = s.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer.is_empty())


if __name__ ==  '__main__':
  unittest.main()