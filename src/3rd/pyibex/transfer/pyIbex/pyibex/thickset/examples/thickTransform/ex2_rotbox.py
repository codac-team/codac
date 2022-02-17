from pyibex import *
from pyibex.thickset import *
from pyibex.geometry import CtcPolar
from vibes import vibes
import math
import numpy as np
vibes.beginDrawing()


class ThickRotation(ThickTest):
  def __init__(self, th1, th2, y):
    ThickTest.__init__(self, 2)
    self.th = Interval(th1, th2)
    self.y = IntervalVector(y) # IntervalVector([[1,3], [4,6]])
    fthin = Function("x1", "x2", "(x1*cos(%s) - x2*sin(%s), x1*sin(%s) + x2*cos(%s))"% ((Interval(th1, th2),  )*4  ) )
    self.thinTest = ThinfIn(fthin, y)
    self.ctcpolar = CtcPolar()
    self.rho, self.theta =  Interval(1, 100) , Interval(0).inflate(2*math.pi/3.)
    self.ctcpolar.contract(y[0], y[1], self.rho, self.theta)

  def test(self, X):
    b = self.thinTest.test(X)
    if is_singleton(b):
      return b

    # penombra
    # tmp = self.ctcpolar.RTfromXY(X[0], X[1])
    # rho, theta = tmp[0], tmp[1]
    rho, theta =  Interval(1, 100) , Interval(0).inflate(2*math.pi/3.)
    self.ctcpolar.contract(X[0], X[1], rho, theta)
    print(rho, theta, self.rho, self.theta)
    titv = ThickInterval(theta + self.th.lb(), theta + self.th.ub())
    if rho.is_subset(self.rho):
      if titv.superset().is_subset(self.theta):
        return IN
      elif rho.is_disjoint(self.rho) or titv.superset().is_disjoint(self.theta):
        return OUT
    # titv = ThickInterval(theta + self.th.lb(), theta + self.th.ub())
      b1 = titv.intersects(self.theta)
      b2 = titv.isNotInclude(self.theta)
    # b1 = isThickIntersect(theta + self.th.lb(), theta + self.th.ub(), self.theta)
    # b2 = isThickNotInclude(theta + self.th.lb(), theta + self.th.ub(), self.theta)
      if b1 and b2:
        return MAYBE

    return UNK





ctcpolar = CtcPolar()
th1 = math.pi/4.
th2 = math.pi/3.
def flb_polar(X0):
  rho, theta = Interval(1, 100) , Interval(0).inflate(2*math.pi/3.)
  # print(rho, theta, X0)
  ctcpolar.contract(X0[0], X0[1], rho, theta)
  theta = theta + th1
  X = IntervalVector(2, [-100,100])
  ctcpolar.contract(X[0], X[1], rho, theta)

  # print(rho, theta, X)
  return X


def fub_polar(X0):
  rho, theta = Interval(1, 100) , Interval(0).inflate(2*math.pi/3.)
  # print(rho, theta, X0)
  ctcpolar.contract(X0[0], X0[1], rho, theta)
  theta = theta + th2
  X = IntervalVector(2, [-100,100])
  ctcpolar.contract(X[0], X[1], rho, theta)

  # print(rho, theta, X)
  return X


flb = Function("x1", "x2", "(x1*cos(%f) - x2*sin(%f), x1*sin(%f) + x2*cos(%f))"% ((th1,  )*4  ) )
fub = Function("x1", "x2", "(x1*cos(%f) - x2*sin(%f), x1*sin(%f) + x2*cos(%f))"% ((th2,  )*4  ) )
print(flb)

fthin = Function("x1", "x2", "(x1*cos(%s) - x2*sin(%s), x1*sin(%s) + x2*cos(%s))"% ((Interval(th1, th2),  )*4  ) )

thinTest = ThinfIn(fthin, IntervalVector([[1,3], [4,6]]))

# test1 = ThickfIn(flb_polar, flb_polar, IntervalVector([[1,3], [4,6]]))
# test2 = ThickfIn(fub_polar, fub_polar, IntervalVector([[1,3], [4,6]]))
# test = ThickOr([test1, test2])
test = ThickRotation(th1, th2, IntervalVector([[1,3], [4,6]]))
P1 = ThickPaving(IntervalVector(2, [-20,20]), test, 0.05)
P1.visit(ToVibes(10000, "test"))

R1 = np.array([[np.cos(th1), np.sin(th1)], [-np.sin(th1), np.cos(th1)]])
R2 = np.array([[np.cos(th2), np.sin(th2)], [-np.sin(th2), np.cos(th2)]])
vibes.selectFigure('test')
for x in np.linspace(1.,3.,10):
  for y in np.linspace(4.,6.,10):
    v = np.array([x,y])
    v1 = R1.dot(v)
    v2 = R2.dot(v)
    # print(x,y)
    vibes.drawCircle(x,y,0.05, '[g]')
    vibes.drawCircle(v1[0],v1[1],0.05, '[b]')
    vibes.drawCircle(v2[0],v2[1],0.05, '[orange]')

# vibes.drawBox(1,3,4,6, '[k]')
# R1 = np.array([[np.cos(-th1), np.sin(-th1)], [-np.sin(-th1), np.cos(-th1)]])
# R2 = np.array([[np.cos(-th2), np.sin(-th2)], [-np.sin(-th2), np.cos(-th2)]])
# vibes.selectFigure('test')
# for x in np.linspace(5.,6.,10):
#   for y in np.linspace(1.,2.,10):
#     v = np.array([x,y])
#     v1 = R1.dot(v)
#     v2 = R2.dot(v)
#     # print(x,y)
#     vibes.drawCircle(x,y,0.05, '[g]')
#     vibes.drawCircle(v1[0],v1[1],0.05, '[b]')
#     vibes.drawCircle(v2[0],v2[1],0.05, '[orange]')
