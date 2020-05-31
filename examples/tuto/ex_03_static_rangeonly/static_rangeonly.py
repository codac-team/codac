# tubex-lib - Examples
# Static range-only localization
# ----------------------------------------------------------------------------

from pyibex import *
from tubex_lib import *
import math
import sys # only for checking if this example still works


# =================== 0. Parameters, truth and data ====================

# Truth (unknown pose)
x_truth = [0,0,math.pi/6] # (x,y,heading)

# Creating random map of landmarks
nb_landmarks = 3
map_area = IntervalVector(2, [-8,8])
v_map = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks)
# The following function generates a set of [range]x[bearing] values
v_obs = DataLoader.generate_static_observations(x_truth, v_map, False)

# Generating range-only observations of these landmarks
v_range = []
for obs in v_obs:
  r = obs[0].inflate(0.1) # adding uncertainties
  v_range.append(r)


# =============== 1. Defining domains for our variables ================

x = IntervalVector(2)


# =========== 2. Defining contractors to deal with equations ===========

# We use the predefined contractor ctc.dist, no need to build it


# =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork()

for i in range(0,nb_landmarks):
  cn.add(ctc.dist, [x, v_map[i], v_range[i]])


# ======================= 4. Solving the problem =======================

cn.contract()


# ============================ 5. Graphics =============================

beginDrawing()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(50, 50, 600, 600)

for iv in v_map:
  fig_map.add_beacon(iv.mid(), 0.2)

for i in range(0,nb_landmarks):
  fig_map.draw_ring(v_map[i][0].mid(), v_map[i][1].mid(), v_range[i], "gray")

fig_map.draw_vehicle(x_truth, 1.) # last param: vehicle size
fig_map.draw_box(x) # estimated position
fig_map.show()

endDrawing()


# Checking if this example still works:
sys.exit(0 if x.contains(x_truth) else 1) # todo: x.contains(x_truth)