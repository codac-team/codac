#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : example3.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : May 28, 2015
#============================================================================

#example 3, simple range only localisation

from pyibex import *
import numpy as np
from vibes import vibes

"""
We consider the localisation of a robot which measures distances
w.r.t three landmaks.
This example uses:
    - Interval and IntervalVector
    - Function
    - SepFwdBwd
    - SepQInter (Q intersection of separators)
"""

if __name__ == '__main__':

  # theorical position of the robot
  robot = [-2, 4]

  #position of the landmarks
  landmarks = [[6, 12], [-2, -5], [-3, 10]]

  # distances measured w r t to landmaks i
  dist = [Interval(11, 12), Interval(8, 10), Interval(5, 7)]


  # Creation of a separator with
  seps = [] # empty list of separator
  # iterate over landmarks and distances
  for (m_x, m_y), d in zip(landmarks, dist):
      # create the constraint function
      f = Function("x_1", "x_2", "(x_1 - %f)^2 + (x_2 - %f)^2"%(m_x, m_y))
      # create the fwdbwf separator f(x) in d
      sep = SepFwdBwd(f, sqr(d))
      seps.append(sep)

  # create the separator using the QIntersection
  sep = SepQInterProjF(seps)
  sep.q = 0

  # init drawing area
  vibes.beginDrawing()
  vibes.newFigure('Result')
  vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})

  #configure pySIVIA output
  params = {'color_in': '#888888[#444444]', 'color_out':
            '#888888[#DDDDDD]', 'color_maybe': '#888888[w]', 'use_patch' : True}

  # create the initial box X0 = [-10, 10] x [-10, 10]
  X0 = IntervalVector([[-12, 11], [-6, 17]])  # '#888888[#DDDDDD]'

  # run SIVIA
  (res_in, res_out, res_y) = pySIVIA(X0, sep, 0.1, **params)


  vibes.drawAUV(robot[0], robot[1], np.rad2deg(0.3), 1, color='[y]')
  for (x, y), d in zip(landmarks, dist):
      vibes.drawCircle(x,y, 0.1, "[k]")
      vibes.drawCircle(x,y, d.lb(), "k")
      vibes.drawCircle(x,y, d.ub(), "k")

  #equalize axis lenght
  vibes.axisEqual()


  vibes.endDrawing()
