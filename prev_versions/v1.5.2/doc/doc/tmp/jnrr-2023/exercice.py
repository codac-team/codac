from codac import *
import math
import random
import time
import numpy as np

dt = 0.02 # temporal discretization 
tdomain = Interval(0,15) # [t0,tf]

# System input
u = Trajectory(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt)

# Actual state trajectory
# Note that this trajectory is unknown of the resolution
x_truth = TrajectoryVector(3)
x_truth[2] = u.primitive()
x_truth[0] = (10*cos(x_truth[2])).primitive()
x_truth[1] = (10*sin(x_truth[2])).primitive()

beginDrawing()
fig_map = VIBesFigMap("Top view")
fig_map.set_properties(50, 50, 800, 600)
fig_map.add_trajectory(x_truth, "x*", 0, 1, "black")
fig_map.smooth_tube_drawing(True)
fig_map.show(1.)

