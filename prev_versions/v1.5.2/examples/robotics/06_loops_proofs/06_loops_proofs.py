# Codac - Examples
# Reliable loop detection of a mobile robot
# ----------------------------------------------------------------------------

from codac import *
import sys # only for checking if this example still works


# =================== 0. Parameters, truth and data ====================

Tube.enable_syntheses() # faster integral computations

dt = 0.01
tdomain = Interval(-1.,10.)

x_truth = TrajectoryVector(tdomain, TFunction("(10*cos(t)+t;5*sin(2*t)+t)"))
x = TubeVector(tdomain, dt, 2)
v = TubeVector(tdomain, dt, TFunction("(-10*sin(t)+1+[-0.2,0.2];10*cos(2*t)+1+[-0.2,0.2])"))

x.set(x_truth(tdomain.lb()), tdomain.lb())

ctc.deriv.contract(x, v)


# =================== 1. Loops detection ====================

tplane = TPlane(x.tdomain())
tplane.compute_detections(dt*2., x, v)
tplane.compute_proofs(x, v)


# =================== 2. Graphics ====================

beginDrawing()

fig_tplane = VIBesFigPaving("t-plane", tplane)
fig_tplane.set_properties(550, 150, 500, 500)
fig_tplane.show()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(450, 50, 800, 800)
fig_map.add_tube(x, "x", 0, 1)
fig_map.add_trajectory(x_truth, "x*", 0, 1)
fig_map.show(1.)

v_loops = tplane.detected_loops()
for loop in v_loops:
  fig_tplane.draw_box(loop, "red")

v_loops = tplane.proven_loops()
for loop in v_loops:
  fig_tplane.draw_box(loop, "green")

endDrawing()


# Checking if this example still works:
sys.exit(0 if (tplane.nb_loops_detections() == 5 and tplane.nb_loops_proofs() == 4) else 1)