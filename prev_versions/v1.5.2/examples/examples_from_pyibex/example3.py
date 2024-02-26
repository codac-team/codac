# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------
# Simple range-only localization using separators
# We consider the localisation of a robot measuring distances w.r.t three landmaks.

from codac import *
import numpy as np

# Theorical (unknown) position of the robot
robot = [-2,4]

# Position of the landmarks
landmarks = [[6,12], [-2,-5], [-3,10]]

# Distances measured w.r.t to landmaks
dist = [Interval(11,12), Interval(8,10), Interval(5,7)]

# Creation of a separator
seps = [] # empty list of separators
# Iterate over landmarks and distances
for (m_x, m_y), d in zip(landmarks, dist):
  # Create the constraint function
  f = Function("x[2]", "(x[0]-%f)^2+(x[1]-%f)^2"%(m_x, m_y))
  # Create the fwdbwd separator for: f(x) \in d
  sep = SepFwdBwd(f, sqr(d))
  seps.append(sep)

# Create the separator as intersection of the previous separators
sep = SepInter(seps)

# Create the initial box
x0 = IntervalVector([[-12,11], [-6,17]])

# Init graphics
beginDrawing()
fig = VIBesFig('Localization')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x0)

# Running the paver
SIVIA(x0, sep, 0.1)

# Displaying ranges of measurements
for (x, y), d in zip(landmarks, dist):
  fig.draw_circle(x, y, 0.1, "[k]")
  fig.draw_circle(x, y, d.lb(), "k")
  fig.draw_circle(x, y, d.ub(), "k")
fig.draw_vehicle(robot[0], robot[1], np.rad2deg(0.3), 1, 'k[y]')

endDrawing()