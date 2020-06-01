# tubex-lib - Examples
# Static range-bearing localization
# ----------------------------------------------------------------------------

from pyibex import *
from tubex_lib import *
import math
import sys # only for checking if this example still works


# =================== 0. Parameters, truth and data ====================

# Truth (unknown pose)
x_truth = [0,0,math.pi/6] # (x,y,heading)

# Creating random map of landmarks
nb_landmarks = 1
map_area = IntervalVector(2, [-8,8])
v_map = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks)
# The following function generates a set of [range]x[bearing] values
v_obs = DataLoader.generate_static_observations(x_truth, v_map, False)

# Adding uncertainties on the measurements
for obs in v_obs: # for each observation:
  obs[0].inflate(0.5) # range
  obs[1].inflate(0.1) # bearing


# =============== 1. Defining domains for our variables ================

x = IntervalVector(2) # unknown position
heading = Interval(x_truth[2]).inflate(0.01) # measured heading


# =========== 2. Defining contractors to deal with equations ===========

ctc_plus = CtcFunction("a", "b", "c", "a+b=c")
ctc_minus = CtcFunction("a", "b", "c", "a-b=c")
# We also use the predefined contractor ctc::polar, no need to build it


# =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork()

for i in range(0,nb_landmarks):

  # Intermediate variables
  alpha = cn.create_dom(Interval())
  d = cn.create_dom(IntervalVector(2))

  cn.add(ctc_plus, [v_obs[i][1], heading, alpha])
  cn.add(ctc_minus, [v_map[i], x, d])
  cn.add(ctc.polar, [d, v_obs[i][0], alpha])


# ======================= 4. Solving the problem =======================

cn.contract()


# ============================ 5. Graphics =============================

beginDrawing()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(50, 50, 600, 600)

for iv in v_map:
  fig_map.add_beacon(iv.mid(), 0.2)

for i in range(0,nb_landmarks):
  fig_map.draw_pie(x_truth[0], x_truth[1], v_obs[i][0] | Interval(0), heading+v_obs[i][1], "lightGray")
  fig_map.draw_pie(x_truth[0], x_truth[1], v_obs[i][0], heading+v_obs[i][1], "gray")

fig_map.draw_vehicle(x_truth, 1) # last param: vehicle size
fig_map.draw_box(x) # estimated position
fig_map.show()

endDrawing()


# Checking if this example still works:
sys.exit(0 if x.contains(x_truth) else 1) # todo: x.contains(x_truth)