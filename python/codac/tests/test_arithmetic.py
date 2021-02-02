#!/usr/bin/env python

import unittest
import pyibex 
from pyibex import Interval, IntervalVector
from codac import *

class TestArithmeticOnTube(unittest.TestCase):

    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)
    
    def tests_scalar_tube(self):

        tdomain = Interval(0.,10.)
        dt = 0.1
        a, b = Interval(-0.8), Interval(3.)

        x = Tube(tdomain, dt, Interval(a))
        y = Tube(x)
        y.set(Interval(b))
        self.assertEqual(x.codomain(), Interval(a))
        self.assertEqual(y.codomain(), Interval(b))

        # const Tube cos(const Tube& x)
        self.assertApproxIntv(cos(x).codomain(), pyibex.cos(a))

        # const Tube sin(const Tube& x)
        self.assertApproxIntv(sin(x).codomain(), pyibex.sin(a))

        # const Tube abs(const Tube& x)
        # self.assertApproxIntv(abs(x).codomain(), pyibex.fabs(a))

        # const Tube sqr(const Tube& x)
        self.assertApproxIntv(sqr(x).codomain(), a**2)

        # const Tube sqrt(const Tube& x)
        self.assertApproxIntv(sqrt(x).codomain(), Interval.EMPTY_SET)
        a = Interval(0.2)
        x.set(a)
        self.assertApproxIntv(sqrt(x).codomain(), pyibex.sqrt(a))

        # const Tube exp(const Tube& x)
        self.assertApproxIntv(exp(x).codomain(), pyibex.exp(a))

        # const Tube log(const Tube& x)
        self.assertApproxIntv(log(x).codomain(), pyibex.log(a))

        # const Tube tan(const Tube& x)
        self.assertApproxIntv(tan(x).codomain(), pyibex.tan(a))

        # const Tube acos(const Tube& x)
        self.assertApproxIntv(acos(x).codomain(), pyibex.acos(a))

        # const Tube asin(const Tube& x)
        self.assertApproxIntv(asin(x).codomain(), pyibex.asin(a))

        # const Tube atan(const Tube& x)
        self.assertApproxIntv(atan(x).codomain(), pyibex.atan(a))

        # const Tube cosh(const Tube& x)
        self.assertApproxIntv(cosh(x).codomain(), pyibex.cosh(a))

        # const Tube sinh(const Tube& x)
        self.assertApproxIntv(sinh(x).codomain(), pyibex.sinh(a))

        # const Tube tanh(const Tube& x)
        self.assertApproxIntv(tanh(x).codomain(), pyibex.tanh(a))

        # const Tube acosh(const Tube& x)
        self.assertApproxIntv(acosh(x).codomain(), pyibex.acosh(a))

        # const Tube asinh(const Tube& x)
        self.assertApproxIntv(asinh(x).codomain(), pyibex.asinh(a))

        # const Tube atanh(const Tube& x)
        self.assertApproxIntv(atanh(x).codomain(), pyibex.atanh(a))

        # const Tube atan2(const Tube& y, const Tube& x)
        self.assertApproxIntv(atan2(y,x).codomain(), pyibex.atan2(b,a))

        # const Tube atan2(const Tube& y, const ibex.Interval& x)
        self.assertApproxIntv(atan2(y,Interval(a)).codomain(), pyibex.atan2(b,a))

        # const Tube atan2(const ibex.Interval& y, const Tube& x)
        self.assertApproxIntv(atan2(Interval(b),x).codomain(), pyibex.atan2(b,a))

        # const Tube pow(const Tube& x, int p)
        self.assertApproxIntv(pow(x,3).codomain(), a**3)

        # const Tube pow(const Tube& x, double p)
        self.assertApproxIntv(pow(x,7.2).codomain(), a**7.2) #  todo: not working with GAO

        # const Tube pow(const Tube& x, const ibex.Interval& p)
        self.assertApproxIntv(pow(x,Interval(4.,5.2)).codomain(), a**Interval(4., 5.2))

        # const Tube root(const Tube& x, int p)
        self.assertApproxIntv(root(x,6).codomain(), pyibex.root(a,6))

        # const Tube operator+(const Tube& x)
        # self.assertEqual(+x, x) # not necessary
        # self.assertEqual((+x).codomain(),  a)

        # const Tube operator+(const Tube& x, const Tube& y)
        self.assertApproxIntv((x+y).codomain(), a+b)

        # const Tube operator+(const Tube& x, const ibex.Interval& y)
        self.assertApproxIntv((x+Interval(b)).codomain(), a+b)

        #     # const Tube operator+(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)+y).codomain(), a+b)

        # Trajectory trajx, trajy
        # trajx.set(a, tdomain.lb()) trajx.set(a, tdomain.mid()) trajx.set(a, tdomain.ub())
        # trajy.set(b, tdomain.lb()) trajy.set(b, tdomain.mid()) trajy.set(b, tdomain.ub())

        #     //const Tube operator+(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x+trajy).codomain(), a+b)
        # self.assertApproxIntv((y+trajx).codomain(), a+b)

        #     //const Tube operator+(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajy+x).codomain(), a+b)
        # self.assertApproxIntv((trajx+y).codomain(), a+b)

        #     //const Tube operator-(const Tube& x)
        #     CHECK((-x).codomain(, -a)

        #     //const Tube operator-(const Tube& x, const Tube& y)
        self.assertApproxIntv((x-y).codomain(), a-b)

        #     //const Tube operator-(const Tube& x, const ibex.Interval& y)

        self.assertApproxIntv((x-Interval(b)).codomain(), a-b)

        #     //const Tube operator-(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)-y).codomain(), a-b)

        #     //const Tube operator-(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x-trajy).codomain(), a-b)

        #     //const Tube operator-(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajx-y).codomain(), a-b)
        # self.assertApproxIntv((trajy-x).codomain(), b-a)

        #     //const Tube operator*(const Tube& x, const Tube& y)
        self.assertApproxIntv((x*y).codomain(), a*b)

        #     //const Tube operator*(const Tube& x, const ibex.Interval& y)
        self.assertApproxIntv((x*Interval(b)).codomain(), a*b)

        #     //const Tube operator*(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)*y).codomain(), a*b)

        #     //const Tube operator*(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x*trajy).codomain(), a*b)

        #     //const Tube operator*(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajx*y).codomain(), a*b)

        #     //const Tube operator/(const Tube& x, const Tube& y)
        self.assertApproxIntv((x/y).codomain(), a/b)

        #     //const Tube operator/(const Tube& x, const ibex.Interval& y)
        self.assertApproxIntv((x/Interval(b)).codomain(), a/b)

        #     //const Tube operator/(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)/y).codomain(), a/b)

        #     //const Tube operator/(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x/trajy).codomain(), a/b)

        #     //const Tube operator/(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajx/y).codomain(), a/b)

        #     //const Tube operator|(const Tube& x, const Tube& y)
        self.assertApproxIntv((x|y).codomain(), (a|b))

        #     //const Tube operator|(const Tube& x, const ibex.Interval& y)
        self.assertApproxIntv((x|Interval(b)).codomain(), (a|b))

        #     //const Tube operator|(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)|y).codomain(), (a|b))

        #     //const Tube operator|(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x|trajy).codomain(), Interval(a,b))

        #     //const Tube operator|(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajx|y).codomain(), Interval(a,b))

        #     x.set(Interval(-10.,10.))

        #     //const Tube operator&(const Tube& x, const Tube& y)
        self.assertApproxIntv((x&y).codomain(), a&b)

        #     //const Tube operator&(const Tube& x, const ibex.Interval& y)
        self.assertApproxIntv((x&Interval(b)).codomain(), a&b)

        #     //const Tube operator&(const ibex.Interval& x, const Tube& y)
        self.assertApproxIntv((Interval(a)&x).codomain(), a)

        #     //const Tube operator&(const Tube& x, const Trajectory& y)
        # self.assertApproxIntv((x&trajy).codomain(), b)

        #     //const Tube operator&(const Trajectory& x, const Tube& y)
        # self.assertApproxIntv((trajx&x).codomain(), a)

        x.set(a)

        # const Tube& operator+=(const ibex.Interval& x)
        z = Tube(x)
        z += Interval(b)

        self.assertApproxIntv(z.codomain(), a+b)

        # const Tube& operator+=(const Trajectory& x)
        # z = x z += trajy
        # self.assertApproxIntv(z.codomain(), a+b)

        # const Tube& operator+=(const Tube& x)
        z = Tube(x)
        z += y
        self.assertApproxIntv(z.codomain(), a+b)

        # const Tube& operator-=(const ibex.Interval& x)
        z = Tube(x)
        z -= Interval(b)
        self.assertApproxIntv(z.codomain(), a-b)

        # const Tube& operator-=(const Trajectory& x)
        # z = x z -= trajy
        # self.assertApproxIntv(z.codomain(), a-b)

        # const Tube& operator-=(const Tube& x)
        z = Tube(x)
        z -= y
        self.assertApproxIntv(z.codomain(), a-b)

        # const Tube& operator*=(const ibex.Interval& x)
        z = Tube(x)
        z *= Interval(b)
        self.assertApproxIntv(z.codomain(), a*b)

        # const Tube& operator*=(const Trajectory& x)
        # z = Tube(x) z *= trajy
        # self.assertApproxIntv(z.codomain(), a*b)

        # const Tube& operator*=(const Tube& x)
        z = Tube(x) 
        z *= y
        self.assertApproxIntv(z.codomain(), a*b)

        # const Tube& operator/=(const ibex.Interval& x)
        z = Tube(x) 
        z /= Interval(b)
        self.assertApproxIntv(z.codomain(), a/b)

        # const Tube& operator/=(const Trajectory& x)
        # z = Tube(x) z /= trajy
        # self.assertApproxIntv(z.codomain(), a/b)

        # const Tube& operator/=(const Tube& x)
        z = Tube(x) 
        z /= y
        self.assertApproxIntv(z.codomain(), a/b)

        # const Tube& operator|=(const ibex.Interval& x)
        z = Tube(x) 
        z |= Interval(b)
        self.assertApproxIntv(z.codomain(), (a|b))

        # const Tube& operator|=(const Trajectory& x)
        # z = Tube(x) z |= trajy
        # self.assertApproxIntv(z.codomain(), Interval(a,b))

        # const Tube& operator|=(const Tube& x)
        z = Tube(x) 
        z |= y
        self.assertApproxIntv(z.codomain(), (a|b))

        # const Tube& operator&=(const ibex.Interval& x)
        z.set(Interval(-10.,10.))
        z &= Interval(b)
        self.assertApproxIntv(z.codomain(), b)

        # const Tube& operator&=(const Trajectory& x)
        # z.set(Interval(-10.,10.)) z &= trajy
        # self.assertApproxIntv(z.codomain(), b)

        # const Tube& operator&=(const Tube& x)
        z.set(Interval(-10.,10.))
        z &= y
        self.assertApproxIntv(z.codomain(), b)

class TestArithmeticOnVectorTube(unittest.TestCase):
    
    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)

    def setUp(self):
        tdomain = Interval(0.,10.)
        dt = 0.1

        self.x = TubeVector(tdomain, dt, 3)
        self.y = TubeVector(self.x)
        self.z = TubeVector(self.x)

        # TrajectoryVector trajx(3), trajy(3)
        # IntervalVector ix(3), iy(3)
        self.a0, self.a1, self.a2 = Interval(1.), Interval(2.), Interval(3.)
        self.b0, self.b1, self.b2 = Interval(0.), Interval(10.), Interval(20.)
        self.ix = IntervalVector([self.a0, self.a1, self.a2])
        self.iy = IntervalVector([self.b0, self.b1, self.b2])

        # ix[0] = a0 ix[1] = a1 ix[2] = a2
        # iy[0] = b0 iy[1] = b1 iy[2] = b2

        self.x.set(self.ix)
        self.y.set(self.iy)

    # trajx.set(ix.mid(), tdomain.lb()) trajx.set(ix.mid(), tdomain.mid()) trajx.set(ix.mid(), tdomain.ub())
    # trajy.set(iy.mid(), tdomain.lb()) trajy.set(iy.mid(), tdomain.mid()) trajy.set(iy.mid(), tdomain.ub())
    def test_vector_size(self):
        self.assertEqual(self.x.size(), 3)
        self.assertEqual(self.y.size(), 3) 
        self.assertEqual(self.z.size(), 3)


    def test_VectorTube_operator_add(self):

        # const TubeVector operator+(const TubeVector& x)
        # z = +self.x
        # self.assertEqual(self.z[0].codomain(), self.a0)
        # # self.assertEqual(self.z[1].codomain(), self.a1)
        # self.assertEqual(self.z[2].codomain(), self.a2) 

        # const TubeVector operator+(const TubeVector& x, const TubeVector& y)
        z = self.x + self.y
        self.assertEqual(z[0].codomain(), self.a0+self.b0)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b2) 

        # const TubeVector operator+(const TubeVector& x, const ibex.IntervalVector& y)
        z = self.x + self.iy
        self.assertEqual(z[0].codomain(), self.a0+self.b0)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b2) 

        # const TubeVector operator+(const ibex.IntervalVector& x, const TubeVector& y)
        z = self.ix + self.y
    
        self.assertEqual(z[0].codomain(), self.a0+self.b0)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b2) 

        # const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y)
        # z = x + trajy
        # CHECK(z[0].codomain() == a0+b0) CHECK(z[1].codomain() == a1+b1) CHECK(z[2].codomain() == a2+b2) 

        # const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y)
        # z = trajx + y
        # CHECK(z[0].codomain() == a0+b0) CHECK(z[1].codomain() == a1+b1) CHECK(z[2].codomain() == a2+b2) 

    def test_VectorTube_operator_sub(self):

        # const TubeVector operator-(const TubeVector& x)
        z = -self.x
        self.assertEqual(z[0].codomain(), -self.a0)
        self.assertEqual(z[1].codomain(), -self.a1)
        self.assertEqual(z[2].codomain(), -self.a2)
        

        # const TubeVector operator-(const TubeVector& x, const TubeVector& y)
        z = self.x - self.y
        self.assertEqual(z[0].codomain(), self.a0-self.b0)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        # const TubeVector operator-(const TubeVector& x, const ibex.IntervalVector& y)
        z = self.x - self.iy
        self.assertEqual(z[0].codomain(), self.a0-self.b0)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        # const TubeVector operator-(const ibex.IntervalVector& x, const TubeVector& y)
        z = self.ix - self.y
        self.assertEqual(z[0].codomain(), self.a0-self.b0)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        # const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y)
        # z = x - trajy
        # self.assertEqual(z[0].codomain(), self.a0-self.b0)
        # self.assertEqual(z[1].codomain(), self.a1-self.b1)
        # self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        # const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y)
        # z = trajx - y
        # self.assertEqual(z[0].codomain(), self.a0-self.b0)
        # self.assertEqual(z[1].codomain(), self.a1-self.b1)
        # self.assertEqual(z[2].codomain(), self.a2-self.b2)
        
    def test_VectorTube_operator_mul(self):

        # const TubeVector operator*(const Tube& x, const TubeVector& y)
        # z =  self.y.__rmul__(self.x[1])
        z =  self.x[1] * self.y
        self.assertEqual(z[0].codomain(), self.a1*self.b0)
        self.assertEqual(z[1].codomain(), self.a1*self.b1)
        self.assertEqual(z[2].codomain(), self.a1*self.b2)
        
        # const TubeVector operator*(const ibex.Interval& x, const TubeVector& y)
        z = Interval(2) * self.y
        self.assertEqual(z[0].codomain(), 2*self.b0)
        self.assertEqual(z[1].codomain(), 2*self.b1)
        self.assertEqual(z[2].codomain(), 2*self.b2)
        

        # const TubeVector operator*(const Tube& x, const ibex.IntervalVector& y)
        z = self.x[1] * self.iy
        self.assertEqual(z[0].codomain(), self.a1*self.b0)
        self.assertEqual(z[1].codomain(), self.a1*self.b1)
        self.assertEqual(z[2].codomain(), self.a1*self.b2)
        

        # const TubeVector operator*(const Trajectory& x, const TubeVector& y)
        # z = trajx[1] * y
        # self.assertEqual(z[0].codomain(), self.a1*self.b0)
        # self.assertEqual(z[1].codomain(), self.a1*self.b1)
        # self.assertEqual(z[2].codomain(), self.a1*self.b2)
        
    def test_VectorTube_operator_truediv(self):
        # const TubeVector operator/(const TubeVector& x, const Tube& y)
        z = self.x / self.y[0]
        self.assertEqual(z[0].codomain(), self.a0/self.b0)
        self.assertEqual(z[1].codomain(), self.a1/self.b0)
        self.assertEqual(z[2].codomain(), self.a2/self.b0)
        

        # const TubeVector operator/(const TubeVector& x, const ibex.Interval& y)
        z = self.x / self.iy[0]
        self.assertEqual(z[0].codomain(), self.a0/self.b0)
        self.assertEqual(z[1].codomain(), self.a1/self.b0)
        self.assertEqual(z[2].codomain(), self.a2/self.b0)
        

        # const TubeVector operator/(const ibex.IntervalVector& x, const Tube& y)
        z = self.ix / self.y[0]
        self.assertEqual(z[0].codomain(), self.a0/self.b0)
        self.assertEqual(z[1].codomain(), self.a1/self.b0)
        self.assertEqual(z[2].codomain(), self.a2/self.b0)
        

        # const TubeVector operator/(const TubeVector& x, const Trajectory& y)
        # z = self.x / trajy[0]
        # self.assertEqual(z[0].codomain(), self.a0/self.b0)
        # self.assertEqual(z[1].codomain(), self.a1/self.b0)
        # self.assertEqual(z[2].codomain(), self.a2/self.b0)
        
    def test_VectorTube_operator_or(self):

        # const TubeVector operator|(const TubeVector& x, const TubeVector& y)
        z = self.x | self.y
        self.assertEqual(z[0].codomain(), (Interval(self.a0) | self.b0))
        self.assertEqual(z[1].codomain(), (Interval(self.a1) | self.b1))
        self.assertEqual(z[2].codomain(), (Interval(self.a2) | self.b2))
        

        # const TubeVector operator|(const TubeVector& x, const ibex.IntervalVector& y)
        z = self.x | self.iy
        self.assertEqual(z[0].codomain(), (Interval(self.a0) | self.b0))
        self.assertEqual(z[1].codomain(), (Interval(self.a1) | self.b1))
        self.assertEqual(z[2].codomain(), (Interval(self.a2) | self.b2))
        

        # const TubeVector operator|(const ibex.IntervalVector& x, const TubeVector& y)
        z = self.ix | self.y
        self.assertEqual(z[0].codomain(), (Interval(self.a0) | self.b0))
        self.assertEqual(z[1].codomain(), (Interval(self.a1) | self.b1))
        self.assertEqual(z[2].codomain(), (Interval(self.a2) | self.b2))
        

        # const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y)
        # z = x | trajy
        # self.assertEqual(z[0].codomain(), (Interval(self.a0) | self.b0))
        # self.assertEqual(z[1].codomain(), (Interval(self.a1) | self.b1))
        # self.assertEqual(z[2].codomain(), (Interval(self.a2) | self.b2))
        

        # const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y)
        # z = trajx | y
        # self.assertEqual(z[0].codomain(), (Interval(self.a0) | self.b0))
        # self.assertEqual(z[1].codomain(), (Interval(self.a1) | self.b1))
        # self.assertEqual(z[2].codomain(), (Interval(self.a2) | self.b2))
        

    def test_VectorTube_operator_and(self):
    
        self.iy |= IntervalVector(3, Interval(-1))
        self.y.set(self.iy)

        # const TubeVector operator&(const TubeVector& x, const TubeVector& y)
        z = self.x & self.y
        self.assertEqual(z[0].codomain(), Interval.EMPTY_SET)
        self.assertEqual(z[1].codomain(), self.a1)
        self.assertEqual(z[2].codomain(), self.a2)
        

        # const TubeVector operator&(const TubeVector& x, const ibex.IntervalVector& y)
        z = self.x & self.iy
        self.assertEqual(z[0].codomain(), Interval.EMPTY_SET)
        self.assertEqual(z[1].codomain(), self.a1)
        self.assertEqual(z[2].codomain(), self.a2)
        

        # const TubeVector operator&(const ibex.IntervalVector& x, const TubeVector& y)
        z = self.ix & self.y
        self.assertEqual(z[0].codomain(), Interval.EMPTY_SET)
        self.assertEqual(z[1].codomain(), self.a1)
        self.assertEqual(z[2].codomain(), self.a2)
        

        # const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y)
        # z = self.y & trajx
        # self.assertEqual(z[0].codomain(), Interval.EMPTY_SET)
        # self.assertEqual(z[1].codomain(), self.a1)
        # self.assertEqual(z[2].codomain(), self.a2)
        

        # //const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y)
        # z = trajx & self.y
        # self.assertEqual(z[0].codomain(), Interval.EMPTY_SET)
        # self.assertEqual(z[1].codomain(), self.a1)
        # self.assertEqual(z[2].codomain(), self.a2)

class TestArithmeticOnVectorTubeIops(unittest.TestCase):
  
    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)

    def setUp(self):
        tdomain = Interval(0.,10.)
        dt = 0.1

        self.x = TubeVector(tdomain, dt, 3)
        self.y = TubeVector(self.x)
        self.z = TubeVector(self.x)

        # TrajectoryVector trajx(3), trajy(3)
        # IntervalVector ix(3), iy(3)
        self.a0, self.a1, self.a2 = Interval(1.), Interval(2.), Interval(3.)
        self.b0, self.b1, self.b2 = Interval(0.), Interval(10.), Interval(20.)
        self.ix = IntervalVector([self.a0, self.a1, self.a2])
        self.iy = IntervalVector([self.b0, self.b1, self.b2])

        # ix[0] = a0 ix[1] = a1 ix[2] = a2
        # iy[0] = b0 iy[1] = b1 iy[2] = b2

        self.x.set(self.ix)
        self.y.set(self.iy)
        self.iy[0] = self.b0
        self.iy[1] = self.b1
        self.iy[2] = self.b2
        self.y.set(self.iy)

    def test_VectorTube_operator_iadd(self):


        #const TubeVector& operator+=(const ibex.Interval& self.x)
        z = TubeVector(self.x)
        z+=self.iy[1]
        self.assertEqual(z[0].codomain(), self.a0+self.b1)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b1)
        

        #const TubeVector& operator+=(const Trajectory& self.x)
        # z = TubeVector(self.x)
        # z+=trajy[1]
        # self.assertEqual(z[0].codomain(), self.a0+self.b1)
        # self.assertEqual(z[1].codomain(), self.a1+self.b1)
        # self.assertEqual(z[2].codomain(), self.a2+self.b1)
        

        #const TubeVector& operator+=(const Tube& self.x)
        z = TubeVector(self.x)
        z+=self.y[1]
        self.assertEqual(z[0].codomain(), self.a0+self.b1)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b1)
        

        #const TubeVector& operator+=(const ibex.IntervalVector& self.x)
        z = TubeVector(self.x)
        z+=self.iy
        self.assertEqual(z[0].codomain(), self.a0+self.b0)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b2)
        

        #const TubeVector& operator+=(const TrajectoryVector& self.x)
        # z = TubeVector(self.x)
        # z+=trajy
        # self.assertEqual(z[0].codomain(), self.a0+self.b0)
        # self.assertEqual(z[1].codomain(), self.a1+self.b1)
        # self.assertEqual(z[2].codomain(), self.a2+self.b2)
        

        #const TubeVector& operator+=(const TubeVector& self.x)
        z = TubeVector(self.x)
        z+=self.y
        self.assertEqual(z[0].codomain(), self.a0+self.b0)
        self.assertEqual(z[1].codomain(), self.a1+self.b1)
        self.assertEqual(z[2].codomain(), self.a2+self.b2)
        

    def test_VectorTube_operator_isub(self):

        #const TubeVector& operator-=(const ibex.Interval& self.x)
        z = TubeVector(self.x)
        z-=self.iy[1]
        self.assertEqual(z[0].codomain(), self.a0-self.b1)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b1)
        

        #const TubeVector& operator-=(const Trajectory& self.x)
        # z = TubeVector(self.x)
        # z-=trajy[1]
        # self.assertEqual(z[0].codomain(), self.a0-self.b1)
        # self.assertEqual(z[1].codomain(), self.a1-self.b1)
        # self.assertEqual(z[2].codomain(), self.a2-self.b1)
        

        #const TubeVector& operator-=(const Tube& self.x)
        z = TubeVector(self.x)
        z-=self.y[1]
        self.assertEqual(z[0].codomain(), self.a0-self.b1)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b1)
        

        #const TubeVector& operator-=(const ibex.IntervalVector& self.x)
        z = TubeVector(self.x)
        z-=self.iy
        self.assertEqual(z[0].codomain(), self.a0-self.b0)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        #const TubeVector& operator-=(const TrajectoryVector& self.x)
        # z = TubeVector(self.x)
        # z-=trajy
        # self.assertEqual(z[0].codomain(), self.a0-self.b0)
        # self.assertEqual(z[1].codomain(), self.a1-self.b1)
        # self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

        #const TubeVector& operator-=(const TubeVector& self.x)
        z = TubeVector(self.x)
        z-=self.y
        self.assertEqual(z[0].codomain(), self.a0-self.b0)
        self.assertEqual(z[1].codomain(), self.a1-self.b1)
        self.assertEqual(z[2].codomain(), self.a2-self.b2)
        

    def test_VectorTube_operator_imul(self):

        #const TubeVector& operator*=(const ibex.Interval& self.x)
        z = TubeVector(self.x)
        z*=self.iy[1]
        self.assertEqual(z[0].codomain(), self.a0*self.b1)
        self.assertEqual(z[1].codomain(), self.a1*self.b1)
        self.assertEqual(z[2].codomain(), self.a2*self.b1)
        

        #const TubeVector& operator*=(const Trajectory& self.x)
        # z = TubeVector(self.x)
        # z*=trajy[1]
        # self.assertEqual(z[0].codomain(), self.a0*self.b1)
        # self.assertEqual(z[1].codomain(), self.a1*self.b1)
        # self.assertEqual(z[2].codomain(), self.a2*self.b1)
        

        #const TubeVector& operator*=(const Tube& self.x)
        z = TubeVector(self.x)
        z*=self.y[1]
        self.assertEqual(z[0].codomain(), self.a0*self.b1)
        self.assertEqual(z[1].codomain(), self.a1*self.b1)
        self.assertEqual(z[2].codomain(), self.a2*self.b1)
        

    def test_VectorTube_operator_itrudiv(self):

        #const TubeVector& operator/=(const ibex.Interval& self.x)
        z = TubeVector(self.x)
        z/=self.iy[1]
        self.assertApproxIntv(z[0].codomain(), self.a0/self.b1)
        self.assertApproxIntv(z[1].codomain(), self.a1/self.b1)
        self.assertApproxIntv(z[2].codomain(), self.a2/self.b1)
        

        #const TubeVector& operator/=(const Trajectory& self.x)
        # z = TubeVector(self.x)
        # z/=trajy[1]
        # self.assertApproxIntv(z[0].codomain(), self.a0/self.b1)
        # self.assertApproxIntv(z[1].codomain(), self.a1/self.b1)
        # self.assertApproxIntv(z[2].codomain(), self.a2/self.b1)
        

        #const TubeVector& operator/=(const Tube& self.x)
        z = TubeVector(self.x)
        z/=self.y[1]
        self.assertApproxIntv(z[0].codomain(), self.a0/self.b1)
        self.assertApproxIntv(z[1].codomain(), self.a1/self.b1)
        self.assertApproxIntv(z[2].codomain(), self.a2/self.b1)
        

    def test_VectorTube_operator_ior(self):

        #const TubeVector& operator|=(const ibex.IntervalVector& self.x)
        z = TubeVector(self.x)
        z|=self.iy
        self.assertApproxIntv(z[0].codomain(), (self.a0 | self.b0))
        self.assertApproxIntv(z[1].codomain(), (self.a1 | self.b1))
        self.assertApproxIntv(z[2].codomain(), (self.a2 | self.b2))
        

        #const TubeVector& operator|=(const TrajectoryVector& self.x)
        # z = TubeVector(self.x)
        # z|=trajy
        # self.assertApproxIntv(z[0].codomain(), (self.a0 | self.b0))
        # self.assertApproxIntv(z[1].codomain(), (self.a1 | self.b1))
        # self.assertApproxIntv(z[2].codomain(), (self.a2 | self.b2))
        

        #const TubeVector& operator|=(const TubeVector& self.x)
        z = TubeVector(self.x)
        z|=self.y
        self.assertApproxIntv(z[0].codomain(), (self.a0 | self.b0))
        self.assertApproxIntv(z[1].codomain(), (self.a1 | self.b1))
        self.assertApproxIntv(z[2].codomain(), (self.a2 | self.b2))
        

    def test_VectorTube_operator_iand(self):
        self.iy |= IntervalVector(3, Interval(-1))
        self.y.set(self.iy)

        #const TubeVector& operator&=(const ibex.IntervalVector& self.x)
        z = TubeVector(self.x)
        z&=self.iy
        self.assertApproxIntv(z[0].codomain(), Interval.EMPTY_SET)
        self.assertApproxIntv(z[1].codomain(), self.a1)
        self.assertApproxIntv(z[2].codomain(), self.a2)
        

        #const TubeVector& operator&=(const TrajectoryVector& self.x)
        # z = TubeVector(self.x)
        # self.y&=trajx
        # self.assertApproxIntv(z[0].codomain(), self.a0)
        # self.assertApproxIntv(z[1].codomain(), self.a1)
        # self.assertApproxIntv(z[2].codomain(), self.a2)
        

        #const TubeVector& operator&=(const TubeVector& self.x)
        z = TubeVector(self.x)
        z&=self.y
        self.assertApproxIntv(z[0].codomain(), Interval.EMPTY_SET)
        self.assertApproxIntv(z[1].codomain(), self.a1)
        self.assertApproxIntv(z[2].codomain(), self.a2)
        
if __name__ ==  '__main__':
  unittest.main()