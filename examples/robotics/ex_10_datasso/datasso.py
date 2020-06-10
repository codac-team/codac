# tubex-lib - Examples
# Set-membership state estimation by solving data association 
# ----------------------------------------------------------------------------
# Example from the paper "Set-membership state estimation by solving data association"
# Simon Rohou, Benoît Desrochers, Luc Jaulin
# ICRA 2020, Paris

from pyibex import *
from tubex_lib import *
import math
import sys # only for checking if this example still works


# =========== CREATING DATA ===========

tdomain = Interval(0,6)

# Truth (analytic function)

state_truth = TrajectoryVector(tdomain, \
  # Lissajous function (px,py,theta):
  TFunction("(240*cos(t) ; \
              120*sin(2*t) ; \
              atan2(240*cos(2*t),-240*sin(t)))"))

# Sets of trajectories

dt = 0.01 # tube timestep
state_truth[2].sample(dt).make_continuous()

x = TubeVector(tdomain, dt, 2) # unbounded 2d tube vector

heading = Tube(state_truth[2], dt) # heading computed from truth...
#heading.inflate(1.2) # ...with some uncertainty

# Derivatives of positions, with uncertainties:
v = TubeVector(tdomain, dt,
               # Lissajous derivative
               TFunction("(-240*sin(t) ; \
                           240*cos(2*t))"))
v.inflate(10)

# Sets of observations

# Creating random map of landmarks
nb_landmarks = 150
map_area = IntervalVector(2, [-400,400])
v_map = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks)

# Generating observations obs=(t,range,bearing) of these landmarks
max_nb_obs = 50
visi_range = Interval(0,75) # [0m,75m]
visi_angle = Interval(-math.pi/4,math.pi/4) # frontal sonar
v_obs = DataLoader.generate_observations(state_truth, v_map, max_nb_obs, True, visi_range, visi_angle)

# Adding uncertainties on the measurements
for obs in v_obs:
  obs[1].inflate(2) # range
  obs[2].inflate(0.1) # bearing

# Association set
m = [IntervalVector(2)] * len(v_obs) # unknown association


# =========== CUSTOM-BUILT CONTRACTORS ===========

ctc_plus = CtcFunction(Function("a", "b", "c", "a-(b+c)")) # algebraic constraint a=b+c
ctc_minus = CtcFunction(Function("a", "b", "c", "a-(b-c)")) # algebraic constraint a=b-c
ctc_constell = CtcConstell(v_map) # constellation constraint


# =========== CONTRACTOR NETWORK ===========

x0 = IntervalVector(state_truth(tdomain.lb())[0:2])
#x0.inflate(10)
x.set(x0,tdomain.lb())

xf = IntervalVector(state_truth(tdomain.ub())[0:2])
#xf.inflate(10)
x.set(xf,tdomain.ub())

cn = ContractorNetwork()
cn.add(ctc.deriv, [x, v])

for i in range (0,len(v_obs)):
  
  # Measurement i
  t  = Interval(v_obs[i][0]) # time
  y1 = Interval(v_obs[i][1]) # range
  y2 = Interval(v_obs[i][2]) # bearing

  #t.set_empty()

  t.inflate(0.1)
  t&=tdomain
  y1.inflate(1)
  y2.inflate(1)

  # Intermediate variables:
  psi = cn.create_dom(Interval()) # robot heading
  a = cn.create_dom(Interval())
  d = cn.create_dom(IntervalVector(2))
  p = cn.create_dom(IntervalVector(2))
  
  cn.add(ctc_constell, [m[i]])
  cn.add(ctc_minus, [d, m[i], p])
  cn.add(ctc_plus, [a, psi, y2])
  cn.add(ctc.polar, [d, y1, a])
  cn.add(ctc.eval, [t, p, x, v])
  cn.add(ctc.eval, [t, psi, heading])

cn.contract(True)

# =========== GRAPHICS ===========

beginDrawing()

fig_heading = VIBesFigTube("heading")
fig_heading.add_tube(heading, "heading")
fig_heading.add_trajectory(state_truth[2], "heading*")
fig_heading.show()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(1450, 50, 1000, 600)
fig_map.add_tube(x, "x", 0, 1)
fig_map.add_trajectory(state_truth, "x*", 0, 1, 2, "blue")
fig_map.add_observations(v_obs, state_truth)
for b in v_map:
  fig_map.add_beacon(b.inflate(2))
fig_map.smooth_tube_drawing(True)
fig_map.show()
fig_map.axis_limits(-340, 340, -1, 1, True)


# =========== ENDING ===========

#  cout << endl << v_map.size() << " landmarks" << endl
#  int identified = 0
#  for(size_t i = 0 ; i < m.size() ; i++)
#    if(m[i].volume() == 0 && !m[i].is_empty()) # degenerate box
#    {
#      identified ++
#      fig_map.add_beacon(m[i], 2., "#00A53B[#00A53B]")
#    }
#  cout << identified << "/" << m.size() << " observations identified" << endl << endl
#  fig_map.show(20.)

endDrawing()
# Checking if this example still works:
#bool success = x.contains(state_truth.subvector(0,1)) == BoolInterval::YES
#return success ? EXIT_SUCCESS : EXIT_FAILURE