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
    s = SepPolygon([[3,-1],[3,4],[5,6],[-1,1]])

    x = IntervalVector(2)
    xs = s.separate(IntervalVector(2))
    self.assertTrue(xs.inner == IntervalVector(2))
    self.assertTrue(xs.outer == IntervalVector([[-1,5],[-1,6]]))

    x = IntervalVector([[3.02,3.16],[2.5,3.2]]) # possible bug
    xs = s.separate(x)
    self.assertTrue(xs.inner == x)
    self.assertTrue(xs.outer.is_empty())

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

    #pave(IntervalVector([[-10,10],[-10,10]]), s, 0.1)

    # Check a box inside the hole

    x = IntervalVector([[0],[0]]).inflate(0.5)
    #DefaultView.draw_box(x,Color::purple())
    xs = s.separate(x)
    self.assertTrue(xs.inner == x)
    self.assertTrue(xs.outer.is_empty())

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    #DefaultView.draw_box(x,Color::purple())
    xs = s.separate(x)
    self.assertTrue(xs.inner.is_empty())
    self.assertTrue(xs.outer == x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    #DefaultView.draw_box(x,Color::purple())
    xs = s.separate(x)
    self.assertTrue(xs.inner == x)
    self.assertTrue(xs.outer.is_empty())


    # Other polygon, defined as a list of vertices

    s = SepPolygon([[6,-6],[7,9],[0,5],[-9,8],[-8,-9]])

    # Check a box inside the polygon

    x = IntervalVector([[5],[-5]]).inflate(0.5)
    xs = s.separate(x)
    self.assertTrue(xs.inner.is_empty())
    self.assertTrue(xs.outer == x)

    # Check a box outside the polygon

    x = IntervalVector([[-1],[8]]).inflate(0.5)
    xs = s.separate(x)
    self.assertTrue(xs.inner == x)
    self.assertTrue(xs.outer.is_empty())


if __name__ ==  '__main__':
  unittest.main()