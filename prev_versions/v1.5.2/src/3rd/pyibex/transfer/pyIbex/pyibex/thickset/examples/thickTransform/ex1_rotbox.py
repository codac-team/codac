from pyibex import *
from pyibex.thickset import *
from pyibex.geometry import CtcPolar
from vibes import vibes
import math
import numpy as np
vibes.beginDrawing()

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



test1 = ThickfIn(flb, flb, IntervalVector([[1,2], [4,5]]))
test2 = ThickfIn(fub, fub, IntervalVector([[1,2], [4,5]]))
test = ThickOr([test1, test2])
P1 = ThickPaving(IntervalVector(2, [-20,20]), test, 0.05)
P1.visit(ToVibes(10000, "test"))

test1_polar = ThickfIn(flb_polar, flb_polar, IntervalVector([[1,2], [4,5]]))
test2_polar = ThickfIn(fub_polar, fub_polar, IntervalVector([[1,2], [4,5]]))
test_polar = ThickOr([test1_polar , test2_polar ])
P2 = ThickPaving(IntervalVector(2, [-20,20]), test_polar, 0.05)
P2.visit(ToVibes(10000, "test_polar"))
vibes.selectFigure("test_polar")
vibes.drawBox(1,2,4,5, 'k[k]')

R1 = np.array([[np.cos(th1), np.sin(th1)], [-np.sin(th1), np.cos(th1)]])
R2 = np.array([[np.cos(th2), np.sin(th2)], [-np.sin(th2), np.cos(th2)]])

for x in np.linspace(1.,2.,10):
  for y in np.linspace(4.,5.,10):
    v = np.array([x,y])
    v1 = R1.dot(v)
    v2 = R2.dot(v)
    # print(x,y)
    vibes.drawCircle(x,y,0.05, '[g]')
    vibes.drawCircle(v1[0],v1[1],0.05, '[b]')
    vibes.drawCircle(v2[0],v2[1],0.05, '[orange]')
