#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_IntervalVector.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================


import unittest
import pyibex
from pyibex import IntervalMatrix, IntervalVector, Interval

class TestIntervalVector(unittest.TestCase):


    def test_constructor(self):
        a = IntervalMatrix(3,3, Interval(3,4))
        b = IntervalVector([[1,1], [1,1], [1,1]])
        c = a*b
        self.assertEqual(c,IntervalVector(3, [9, 12]))


    def test_contructor(self):
        lst = [Interval(i) for i in range(0,9)]
        M = IntervalMatrix(3,3,lst)
        # print(M)

    def test_get_set(self):
        a = IntervalMatrix(3,3, Interval(3,4))
        self.assertEqual(a[1], IntervalVector(3, Interval(3,4)))
        self.assertEqual(a[0][2], Interval(3,4))
        a[0][2] = Interval(0,0)
        self.assertEqual(a[0][2], Interval(0,0))

    def test_shape(self):
        a = IntervalMatrix(3,3, Interval(3,4))
        self.assertEqual(a.shape(), (3,3))

        # print(a[1])
        # print(a[1][1])
        # a[0][2] = Interval(0,0)
        # print(a)
    # def test_constructor2(self):
    #     c =IntervalVector(3, [1,2])
    #     self.assertEqual(c[0], Interval(1,2))

    # def test_constructor3(self):
    #     c = IntervalVector( (Interval(1, 3), Interval(float('inf'), 4)))
    #     self.assertEqual(c[0], Interval(1, 3))
    #     self.assertEqual(c[1], Interval(float('inf'), 4))

    # def test_gettiem1(self):
    #     """
    #     check if gettiem returns a non constant reference on an Interval
    #     """
    #     c = IntervalVector((1,2,3))
    #     b = c[1]
    #     self.assertEqual(b, Interval(2))
    #     b.assign(Interval(-1, 2))
    #     self.assertEqual(b, Interval(-1, 2))
    #     self.assertEqual(c[1], Interval(-1, 2))

    # def test_gettiem2(self):
    #     """
    #     check if gettiem returns a non constant reference on an Interval
    #     """
    #     c = IntervalVector((1,2,3))
    #     b = c[1]
    #     self.assertEqual(b, Interval(2))
    #     c[1] = Interval(-1, 2)
    #     self.assertEqual(c[1], Interval(-1, 2))
    #     self.assertEqual(b, Interval(-1, 2))


    # def test_assignement(self):
    #     a = IntervalVector(2)
    #     a[1] = Interval(2,1)
    #     self.assertEqual(a[1], Interval(2,1))

    # def test_empty(self):
    #     a  = IntervalVector.empty(3)
    #     self.assertEqual(a.size(), 3)
    #     self.assertEqual(len(a), 3)
    #     self.assertTrue(a.is_empty())

    # def test_set_empty(self):
    #     a = IntervalVector(3)
    #     a.set_empty()
    #     self.assertTrue(a.is_empty())

    # def test_clear(self):
    #     a = IntervalVector(3, Interval(-2,4))
    #     a.clear()
    #     self.assertEqual(a[0], Interval(0))

    # def test_init(self):
    #     a = IntervalVector(3, Interval(-2,4))
    #     a.init(Interval(2,4))
    #     self.assertEqual(a[0], Interval(2,4))

    # def test_inflate(self):
    #     a = IntervalVector(3, Interval(-2,4))
    #     a.inflate(2)
    #     b = IntervalVector(3, Interval(-4,6))


    # def test_resize(self):
    #     a = IntervalVector(3, Interval(-2,4))
    #     self.assertEqual(a.size(),3)
    #     a.resize(1000)
    #     self.assertEqual(a.size(),1000)

    # def test_subvector(self):
    #     a = IntervalVector(list(range(0,10)))
    #     self.assertEqual(len(a), 10)
    #     b = a.subvector(3,6)
    #     self.assertEqual(len(b), 4)
    #     b[0] = Interval(-2,3)
    #     self.assertEqual(b[0], Interval(-2,3))

    # def test_put(self):
    #     a = IntervalVector(list(range(0,10)))
    #     self.assertEqual(len(a), 10)
    #     a.put(3, IntervalVector([1,2,3]))
    #     self.assertEqual(a[3], Interval(1))
    #     self.assertEqual(a[4], Interval(2))
    #     self.assertEqual(a[5], Interval(3))
    #     self.assertEqual(a[6], Interval(6))


    # def test_size(self):
    #     a = IntervalVector(list(range(0,10)))
    #     self.assertTrue(a.size() == 10)

    # def test_lb(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     test = [-1,0,1,2,3,4,5]
    #     self.assertEqual(test, a.lb())
    #     # self.assertTrue(False)

    # def test_ub(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     test = [1,2,3,4,5,6,7]
    #     self.assertEqual(test, a.ub())

    # def test_mid(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     test = list(range(0,7))
    #     self.assertEqual(test, a.mid())

    # def test_mig(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     self.assertEqual([0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0], a.mig())

    # def test_mag(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     self.assertEqual([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0], a.mag())

    # # def test_is_empty(self):
    # #     self.assertTrue(False)

    # def test_is_flat(self):
    #     a = IntervalVector(list(range(1,5)))
    #     self.assertTrue(a.is_flat())

    # def test_is_unbounded(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     self.assertFalse(a.is_unbounded())
    #     a[1] = Interval.POS_REALS
    #     self.assertTrue(a.is_unbounded())

    # def test_is_subset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-2.0, -1, 2])
    #     self.assertTrue(b.is_subset(a))

    # def test_is_strict_subset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-1.9, -1, 2]).inflate(0.1)
    #     self.assertTrue(b.is_strict_subset(a))


    # def test_is_interior_subset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-1.8, -1, 2]).inflate(0.1)
    #     self.assertTrue(b.is_interior_subset(a))


    # def test_is_strict_interior_subset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-1.8, -1, 2]).inflate(0.1)
    #     self.assertTrue(b.is_strict_interior_subset(a))


    # def test_is_superset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-2.0, -1, 2])
    #     self.assertTrue(a.is_superset(b))


    # def test_is_strict_superset(self):
    #     a = IntervalVector(3, Interval(-2,3))
    #     b = IntervalVector([-2.0, -1, 2])
    #     self.assertTrue(a.is_strict_superset(b))


    # def test_contains(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     self.assertTrue(a.contains(list(range(0,7))))

    # def test_interior_contains(self):
    #     a = IntervalVector(list(range(0,7))).inflate(1)
    #     self.assertTrue(a.contains(list(range(0,7))))

    # def test_intersects(self):
    #     a = IntervalVector(list(range(0,7))).inflate(2)
    #     b = IntervalVector(list(range(0,7))).inflate(1)
    #     self.assertTrue(a.intersects(b))

    # def test_overlaps(self):
    #     a = IntervalVector(2, Interval(-2,4))
    #     b = IntervalVector(2, Interval(1,3))
    #     self.assertTrue(a.overlaps(b))

    # def test_is_disjoint(self):
    #     a = IntervalVector(2, Interval(-2,4))
    #     b = IntervalVector(2, Interval(1,3))
    #     c = IntervalVector(2, Interval(-4,-2.5))
    #     self.assertFalse(a.is_disjoint(b))
    #     self.assertTrue(a.is_disjoint(c))


    # def test_is_zero(self):
    #     a = IntervalVector(list(range(0,3)))
    #     self.assertFalse(a.is_zero())
    #     a = IntervalVector([0,0,0])
    #     self.assertTrue(a.is_zero())


    # def test_is_bisectable(self):
    #     a = IntervalVector(2, Interval(-2,4))
    #     self.assertTrue(a.is_bisectable())
    #     b = IntervalVector(2, Interval(1,3))
    #     self.assertTrue(b.is_bisectable())
    #     b[0]  = Interval(0)
    #     self.assertTrue(b.is_bisectable())
    #     b[1]  = Interval(0)
    #     self.assertFalse(b.is_bisectable())


    # def test_rad(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     self.assertEqual(a.rad(),[1,1,1])

    # def test_diam(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     self.assertEqual(a.rad(),[1,1,1])


    # # def test_sort_indices(self):
    # #     a = IntervalVector(list(range(0,3))).inflate(1)
    # #     a.sort_indices()

    # def test_min_max_diam(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     a[1] = a[1].inflate(0.2)
    #     a[2] = a[2].inflate(0.5)
    #     self.assertEqual(a.max_diam(),3)
    #     self.assertEqual(a.min_diam(),2)



    # def test_volume(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     self.assertEqual(round(a.volume()), 8)

    # def test_perimeter(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     self.assertEqual(a.perimeter(), 6.0)


    # def test_rel_distance(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     b = IntervalVector(list(range(0,3)))
    #     self.assertEqual(a.rel_distance(b), 0.5)

    # def test_diff(self):
    #     a = IntervalVector([[-1,1], [0,2], [1, 5]])
    #     b = IntervalVector(list(range(0,3))).inflate(1)
    #     l = a.diff(b)
    #     self.assertEqual(len(l), 1)
    #     self.assertEqual(l[0], IntervalVector([[-1,1], [0,2], [3, 5]]))

    # def test_diff_2(self):
    #     a = IntervalVector([[-1,1], [0,2], [1, 5]])
    #     b = IntervalVector(a)
    #     l = a.diff(b)
    #     self.assertEqual(len(l), 0)
    #     #self.assertEqual(l[0], IntervalVector.empty(3))

    #     # self.assertEqual(l[0], IntervalVector([[-1,1], [0,2], [3, 5]]))


    # def test_complementary(self):
    #     a = IntervalVector(list(range(0,1))).inflate(1)
    #     l = a.complementary()
    #     self.assertEqual(l[0], IntervalVector(1, Interval(float("-inf"), -1)))
    #     self.assertEqual(l[1], IntervalVector(1, Interval(1, float("inf"))))


    # def test_bisect(self):
    #     a = IntervalVector(list(range(0,3))).inflate(1)
    #     (c,d) = a.bisect(2)


    # def test_ops_plus(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     b = IntervalVector(2, Interval(-3, 1))
    #     self.assertEqual(a + b, IntervalVector(2, Interval(-4, 2)))
    #     self.assertEqual(a - b, IntervalVector(2, Interval(-2, 4)))
    #     self.assertEqual(a & b, IntervalVector(2, Interval(-1,1)))
    #     self.assertEqual(a | b, IntervalVector(2, Interval(-3,1)))
    #     self.assertTrue(a != b)

    # def test_ops_2(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     b = IntervalVector(2, Interval(-3, 1))
    #     a += b
    #     self.assertEqual(a, IntervalVector(2, Interval(-4, 2)))
    #     a = IntervalVector(2, Interval(-1,1))
    #     a -= b
    #     self.assertEqual(a, IntervalVector(2, Interval(-2, 4)))
    #     a = IntervalVector(2, Interval(-1,1))
    #     a &= b
    #     self.assertEqual(a, IntervalVector(2, Interval(-1,1)))
    #     a = IntervalVector(2, Interval(-1,1))
    #     a |= b
    #     self.assertEqual(a, IntervalVector(2, Interval(-3,1)))


    # def test_ops_3(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     a += [2,2]
    #     self.assertEqual(a, IntervalVector(2, Interval(1, 3)))

    #     a = IntervalVector(2, Interval(-1,1))
    #     a -= [2,2]
    #     self.assertEqual(a, IntervalVector(2, Interval(-3, -1)))

    #     a = IntervalVector(2, Interval(-1,1))
    #     a *= Interval(2,3)
    #     self.assertEqual(a, IntervalVector(2, Interval(-3, 3)))

    #     a = IntervalVector(2, Interval(-1,1))
    #     a *= 3.0
    #     self.assertEqual(a, IntervalVector(2, Interval(-3, 3)))


    # def test_ops_4(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     b = IntervalVector(2, Interval(-3, 1))
    #     self.assertEqual(a*b, Interval(-6,6))
    #     self.assertEqual(3*a, IntervalVector(2, Interval(-3,3)))
    #     self.assertEqual(a*[2,2], Interval(-4,4))
    #     self.assertEqual(Interval(2,3)*a, IntervalVector(2, Interval(-3,3)))

    # def test_ops_5(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     self.assertEqual(a+[1,1], IntervalVector(2,Interval(0,2)))
    #     self.assertEqual([1,1] + a, IntervalVector(2, Interval(0,2)))

    # def test_ops_6(self):
    #     a = IntervalVector(2, Interval(-1,1))
    #     self.assertEqual(a-[1,1], IntervalVector(2,Interval(-2,0)))
    #     self.assertEqual([1,1] - a, IntervalVector(2, Interval(0,2)))

    # def test_abs(self):
    #     a =  IntervalVector(2, Interval(-2,4))
    #     self.assertEqual(pyIbex.abs(a), IntervalVector(2, Interval(0,4)))

    # def test_bwd_add(self):
    #     a =  IntervalVector(2, Interval(2,4))
    #     b = IntervalVector(2, Interval(-3, 4))
    #     c = IntervalVector(2)
    #     pyIbex.bwd_add(c, a,b)

    # def test_bwd_sub(self):
    #     a =  IntervalVector(2, Interval(2,4))
    #     b = IntervalVector(2, Interval(-3, 4))
    #     c = IntervalVector(2)
    #     pyIbex.bwd_sub(c,a,b)

    # def test_bwd_mul(self):
    #     a =  IntervalVector(2, Interval(2,4))
    #     b = IntervalVector(2, Interval(-3, 4))
    #     c = IntervalVector(2)
    #     pyIbex.bwd_mul(c, a, b)

    # def test_bwd_mul(self):
    #     a =  IntervalVector(2, Interval(2,4))
    #     b = Interval(1,2)
    #     c = IntervalVector(2)
    #     pyIbex.bwd_mul(a,b,c)
    #     pyIbex.bwd_mul(b,a,c)


if __name__ == '__main__':
    unittest.main()
