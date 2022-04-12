# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------
# Example from Applied Interval Analysis (L. Jaulin et.al. ) §6.3.4

from codac import *
from codac.unsupported import *
import math

def export_data(Y, T):
  vibes.newFigure('Data')
  vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})
  print("#"*50)
  print("Data to use")
  for i,(y,t) in enumerate(zip(Y[::-1], T[::-1])):
    # print(" %d & %s & %s \\\\ \\hline"%(i+1,t,y))
    print("\\draw[boxColor] (%f, %f) rectangle (%f, %f);"%(t[0], y[0], t[1], y[1]))
    # vibes.drawBox(t[0], t[1], y[0], y[1], 'k[lightGray]')
  print("#"*50)


def test_paramEstim( useSepProj=False):

  f = Function("p1", "p2", "t", "20*exp(-p1*t)-8*exp(-p2*t)");

  Y = [
    Interval(2.7,12.1),
    Interval(1.04,7.14),
    Interval(-0.13,3.61),
    Interval(-0.95,1.15),
    Interval(-4.85,-0.29),
    Interval(-5.06,-0.36),
    Interval(-4.1,-0.04),
    Interval(-3.16,0.3),
    Interval(-2.5,0.51),
    Interval(-2,0.67)
  ]

  T = [
    Interval(-0.25,1.75),
    Interval(0.5,2.5),
    Interval(1.25,3.25),
    Interval(2,4),
    Interval(5,7),
    Interval(8,10),
    Interval(12,14),
    Interval(16,18),
    Interval(20,22),
    Interval(24,26)
  ]

  seps = []

  for y_i,t_i in zip(Y, T):
    if useSepProj == False:
      seps.append( SepCtcPairProj( SepFwdBwd(f, y_i ), t_i, 1e-3) )
    else:
      seps.append( SepProj( SepFwdBwd(f, y_i ), t_i, 1e-3) )

  sep = SepInter(seps)

  X0 = IntervalVector(2,Interval(0,1.2));
  X0[1] = Interval(0,0.5);


  return sep, X0, 0.01




sep, X0, eps = test_paramEstim(True)



beginDrawing()
fig = VIBesFig('Result')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(X0)

# run SIVIA
SIVIA(X0, sep, eps)

endDrawing()