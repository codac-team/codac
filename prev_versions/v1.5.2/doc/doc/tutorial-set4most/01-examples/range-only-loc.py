from codac import *
import math
import random
import time
import numpy as np


dt = 0.02
iteration_dt = 0.2
tdomain = Interval(0,15) # [t0,tf]

# System input
u = Trajectory(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt)

# Actual trajectories (state + derivative)
# Note that these trajectories are unknown of the resolution
v_truth = TrajectoryVector(3)
x_truth = TrajectoryVector(3)
v_truth[2] = u
x_truth[2] = v_truth[2].primitive()
v_truth[0] = 10*cos(x_truth[2])
v_truth[1] = 10*sin(x_truth[2])
x_truth[0] = v_truth[0].primitive()
x_truth[1] = v_truth[1].primitive()

beginDrawing()
fig_map = VIBesFigMap("slam")
fig_map.set_properties(50, 50, 1200, 600)
#fig_map.axis_limits(IntervalVector([[-34.8, 40.2],[-17.3,20.2]]))
fig_map.add_trajectory(x_truth, "truth", 0, 1, "black")
fig_map.smooth_tube_drawing(True)
fig_map.show(1.)

###########################################
# Section C: Deadreckoning                #
###########################################

# Bounded trajectories (dead reckoning)
v = TubeVector(tdomain, dt, 3)
x = TubeVector(tdomain, dt, 3)

# Heading measurement with bounded uncertainties
x[2] = Tube(x_truth[2], dt) + Interval(-0.03,0.03)
x.set([0,0,0], 0.) # setting a vector value at t=0

ctc_f = CtcFunction(Function("x[3]", "v[3]", "(v[0]-10*cos(x[2]) ; v[1]-10*sin(x[2]))"))
ctc.deriv # object already instanciated in the library

cn = ContractorNetwork()
cn.add(ctc_f, [x,v])
cn.add(ctc.deriv, [x,v])

cn.contract(True)

fig_map.add_tube(x, "x", 0, 1)
fig_map.show(1.)

###########################################
# Section D: Range-only localisation      #
###########################################

v_b = [ (6,12), (-2,-5), (-3,20), (3,4), (-10,0) ]

for ti in np.arange(0,15):
  k = random.randint(0,len(v_b)-1) # a random landmark is perceived
  d = sqrt(sqr(x_truth(ti)[0]-v_b[k][0])+sqr(x_truth(ti)[1]-v_b[k][1]))
  d += Interval(-0.03,0.03)
  pi = IntervalVector(3)
  cn.add(ctc.eval, [ti,pi,x,v])
  cn.add(ctc.dist, [pi[0],pi[1],v_b[k][0],v_b[k][1],d])

cn.contract(True)
fig_map.add_landmarks(v_b, 0.4)
fig_map.show(1.)