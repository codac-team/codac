#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Image.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2015
#============================================================================

import unittest
import pyibex
from pyibex import *
from pyibex.image import *
import sys
try:
  import numpy as np
  has_np = True
except ImportError:
  print("NUMPY NOT FOUND")
  has_np = False


# Interval.__str__=lambda x: "("+str(x[0])+', '+str(x[1])+")"
# Interval.__repr__=lambda x: "("+x[0]+', '+x[1]+")"
# IntervalVector.__str__=lambda x: "[ "+ ';'.join([b for b in x])+ " ]"
# IntervalVector.__repr__=lambda x: "[ " + ';'.join([str(b) for b in x])+ " ]"
# IntervalVector.__repr__=lambda x: 'titi'


class TestGeoMapper(unittest.TestCase):
  def setUp(self):
    self.mapper = GeoMapper(-5, 5, 10,10, 1, -1)
    # print(self.mapper.boundingBox, file=sys.stderr)
  def test_boxes(self):
    # pass
    res = self.mapper.world_to_grid(IntervalVector(2, [-5.0, 5-0.01]))
    self.assertEqual(res, [0,9,0,9])
    res = self.mapper.world_to_grid(IntervalVector( [[-5, -4], [-5, 5]]))
    self.assertEqual(res, [0,1,0,9])

  def test_points_upperLeft(self):
    # res = self.mapper.world_to_grid(IntervalVector([-5, 5]))
    # self.assertEqual(res, [0,0,0,0])
    # res = self.mapper.world_to_grid(IntervalVector([-4, 5]))
    # self.assertEqual(res, [-1, -1, -1, -1])
    # res = self.mapper.world_to_grid(IntervalVector([-5, 6]))
    # self.assertEqual(res, [-1, -1, -1, -1])
    res = self.mapper.world_to_grid(IntervalVector([-4-0.01, 4+0.01]))
    self.assertEqual(res, [0,0,0,0])

    self.assertEqual(self.mapper.grid_to_world([0, 0, 0, 0]),
                     IntervalVector([[-5, -4], [4, 5]])
                     )


@unittest.skipUnless(has_np, "Numpy not found")
class TestImage(unittest.TestCase):

  def test_world_to_grid(self):

    A = np.zeros((5,10), dtype=np.uint)
    im = GeoImage(A,0,0,1,1)

    X = IntervalVector([[1,2], [1,2]])
    # print(X.is_subse  t(im.boundingBox))
    self.assertEqual(im.world_to_grid(X), [0,2,0,2])
    X = IntervalVector([[1,2], [1,3]])
    self.assertEqual(im.world_to_grid(X), [0,2,0,3])
    X = IntervalVector([[1,1.1], [1,2.5]])
    self.assertEqual(im.world_to_grid(X), [0,1,0,2])



  def test_II_values(self):
    A = np.zeros((5,10), dtype=np.uint)
    A[2,5] = 1
    A = A.cumsum(axis=0).cumsum(axis=1)
    im = GeoImage(A,0,5,1,-1)

    # self.assertEqual(im.test(IntervalVector([[5.1,5.1], [2,3]])), UNK)
    # self.assertEqual(im.test(IntervalVector([[2,3], [1,2]])), OUT)
    # self.assertEqual(im.test(IntervalVector([[5,6], [2,4]])), UNK)

  def test_1(self):

    A = np.zeros((5,10), dtype=np.uint)
    A[2,5] = 1
    A = A.cumsum(axis=0).cumsum(axis=1)
    im = GeoImage(A,0,0,1,0.5)
    # for i in range(5):
      # for j in range(10):
        # print(im.pixelAt(i,j), end=' ')
      # print('')

@unittest.skipUnless(has_np, "Numpy not found")
class TestWorldToGrid(unittest.TestCase):

  def setUp(self):
    a = np.zeros((500,500))
    # IMage (x0, y0)=(-5,5)
    self.im = GeoImage(a, -5,5,0.02, -0.02)

  def intervalAlmostEqual(self, itv1, itv2 ):
    self.assertAlmostEqual(itv1[0], itv2[0])
    self.assertAlmostEqual(itv1[1], itv2[1])

  def intervalVectorAlmostEqual(self, itv1, itv2):
    for i in range(itv1.size()):
      self.intervalAlmostEqual(itv1[i], itv2[i])





  def test_world_to_grid(self):
    """
        0   1   2   3   4   5   6   7   8   9
      +---+---+---+---+---+---+---+---+---+---+...
    0 |   |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+---+...
    1 |   |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+---+...
    2 |   |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+---+...
    3 |   |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+---+...
    4 |   |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+---+...

    """
    b = IntervalVector([-5, 5])
    self.assertEqual(self.im.world_to_grid(b), [0,0,0,0])

  def test_boundingBox(self):
    b = self.im.boundingBox
    # print(b.ub()[0].hex(), 5.02.hex())
    self.intervalVectorAlmostEqual(self.im.boundingBox, IntervalVector([[-5,5], [-5,5]]))
    # self.assertEqual(self.im.boundingBox, IntervalVector([[-5,5.02], [-5.02,5]]))

  # def test_Grid_to_world01(self):
  #   a = IntervalVector.empty(2)
  #   L = self.im.world_to_grid(a)
  #   # self.assertEqual(L, [-1, -1, -1, -1])
  #   a = IntervalVector(2, Interval.ALL_REALS)
  #   L = self.im.world_to_grid(a)
  #   self.assertEqual(L, [0, 499, 0, 499])

    # a = IntervalVector([[-6, -5], [-10,-9]])
    # L = self.im.world_to_grid(IntervalVector([[-6, -5], [-10,-9]]))
    # print(L)

  # def test_world_to_grid(self):
  #   X0 = IntervalVector([[-7, -6], [0,1]])
  #   self.assertEqual(self.im.test(X0), MAYBE)
@unittest.skipUnless(has_np, "Numpy not found")
class TestCtcRaster(unittest.TestCase):

  def setUp(self):

    # under approximation of the set X
    ain = np.zeros((200, 200), dtype=np.int64)
    ain[75:125, 50:125] = 1
    ain[25:100, 25:50] = 1

    # over approximation of the set X
    aout = np.zeros((200, 200), dtype=np.int64)
    aout[74:126, 49:125] = 1
    aout[24:101, 24:51] = 1

    img_out = aout.cumsum(0).cumsum(1)
    img_in = (1 - ain).cumsum(0).cumsum(1)

    self.ctcOut = CtcRaster(img_out, -5, 5, 0.1, -0.1)
    self.ctcIn = CtcRaster(img_in, -5, 5, 0.1, -0.1)
    self.sep = SepCtcPair(self.ctcIn, self.ctcOut)


  def test_ctcOut_01(self):
    X = IntervalVector(2, [-oo, oo])
    self.ctcOut.contract(X)
    self.assertTrue(IntervalVector([ [-2.6, 7.6],  [-7.5, 2.6] ]).is_subset(X))

  def test_ctcOut_02(self):
    X = IntervalVector([ [-2, 0],  [-2, 1] ])
    self.ctcOut.contract(X)
    self.assertTrue(IntervalVector([ [-2, 0],  [-0.1, 1] ]).is_subset(X) )

  def test_ctcOut_03(self):
    X = IntervalVector([ [-4, 2],  [1, 2] ])
    self.ctcOut.contract(X)
    self.assertEqual(X, IntervalVector([ [-2.6, 2],  [1, 2] ]) )

  def test_ctcOut_04(self):
    X = IntervalVector([ [-2.85, -2.18],  [2.15, 2.85] ])
    self.ctcOut.contract(X)
    self.assertEqual(X, IntervalVector([ [-2.6, -2.18],  [2.15, 2.6] ]) )

  def test_ctcOut_05(self):
    X = IntervalVector([ [-2, -1],  [3, 4] ])
    self.ctcOut.contract(X)
    self.assertTrue(X.is_empty())

  def test_ctcIn_01(self):
    X = IntervalVector([ [-2, -1],  [1, 2] ])
    self.ctcIn.contract(X)
    self.assertTrue(X.is_empty())

  def test_ctcIn_02(self):
    X = IntervalVector([ [-2, -1],  [1, 3.5] ])
    self.ctcIn.contract(X)
    self.assertTrue( IntervalVector([ [-2, -1],  [2.5, 3.5] ]).is_subset(X) )

  def test_ctcIn_03(self):
    X = IntervalVector([ [-3, -1],  [1, 2] ])
    self.ctcIn.contract(X)
    self.assertTrue( IntervalVector([ [-3, -2.5],  [1, 2] ]).is_subset(X) )

  def test_border_01(self):
    X0 = IntervalVector([ [2.05, 2.15],  [-0.08, -0.02] ])
    xin, xout  = IntervalVector(X0),IntervalVector(X0)
    self.sep.separate(xin, xout)
    self.assertEqual(xin, X0)
    self.assertEqual(xout, X0)

  def test_border_02(self):
    X0 = IntervalVector([ [2.05, 2.15],  [-0.08, -0.0] ])
    X = IntervalVector(X0)
    self.ctcOut.contract(X0)
    self.assertEqual(X, X0)
    self.ctcIn.contract(X0)
    self.assertEqual(X, X0)








if __name__ == '__main__':

  unittest.main()
