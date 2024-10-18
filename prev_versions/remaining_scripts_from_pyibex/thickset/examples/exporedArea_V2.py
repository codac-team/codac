# Exploered area v2 : 04/09/2016

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from pyibex import *
from pyibex.thickset import *
from pyibex_thickset.sweeptest import *
from vibes import vibes
import os

# def SweepTest(ThickTest):
#   def __init__(self, x, y, ux, uy):
#     ThickTest.__init__(self, 2)
#     self.x, self.y, self.ux, self.uy = x, y, ux, uy
#
#   def


# data = pd.read_pickle('test.pickle')
print("Read Data")
# data = pd.read_csv('NavDaurade_20151019.csv.bz2')
dirname = os.path.dirname(__file__)
data = pd.read_csv(dirname+'/NavDaurade_20151019.csv')

X, Y, ux, uy = [], [], [], []
x,y = Interval(0), Interval(0)

print("Compute trajectory")
for i,(dvx, dvy, hdg, alt) in enumerate(zip(data.vx, data.vy, data.heading, data.altitude)):
  dt = 0.05
  # dt = d[i+1, 3] - d[i, 3]
  hdg = -np.deg2rad(hdg)# - 0.015
  dvx = Interval(dvx).inflate(0.005)
  dvy = Interval(dvy).inflate(0.005)


  # downsampling displayed data
  if i%20 == 0:
    X += [Interval(y)]
    Y += [Interval(x)]
    ux += [cos(Interval(hdg))]
    uy += [sin(Interval(hdg))]

  y = y + dt * ( np.cos(hdg)*dvx - np.sin(hdg)*dvy )
  x = x + dt * ( np.sin(hdg)*dvx + np.cos(hdg)*dvy )

print(len(X))
# exit()

vibes.beginDrawing()
vibes.newFigure('Trajectory')
for bx, by in zip(X,Y):
  vibes.drawBox(bx[0], bx[1], by[0], by[1])
vibes.setFigureProperties(dict(x=0, y=10, width=500, height=500))
vibes.axisEqual()

print("Run SIVIA")
X0 = IntervalVector([[-500, 220], [-100, 570]])
vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], 'b')
vibes.newFigure('ExArea')
sweptTest = SweepTest(X,Y,ux,uy, 60.)
M = ThickPaving(X0, sweptTest, 10, opInter)

print("CICIC")

# M.Sivia(sweptTest, 2, opInter)

M.visit(ToVibes(2000))
vibes.drawArrow([-400, -50], [-400, 0], 10, 'w[w]')
vibes.drawArrow([-400, -50], [-350, -50], 10, 'w[w]')
for bx, by in zip(X,Y):
  vibes.drawBox(bx[0], bx[1], by[0], by[1], 'k')
vibes.setFigureProperties(dict(x=0, y=10, width=500, height=500))
vibes.axisEqual()
