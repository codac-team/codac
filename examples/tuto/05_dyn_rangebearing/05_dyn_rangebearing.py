# tubex-lib - Examples
# Dynamic range-only localization
# ----------------------------------------------------------------------------

from pyibex import *
from tubex_lib import *
import sys # only for checking if this example still works


# =================== 0. Parameters, truth and data ====================

dt = 0.05                                      # timestep for tubes accuracy
tdomain = Interval(0,3)                        # temporal limits [t_0,t_f]=[0,3]

x_truth = TrajectoryVector(tdomain, TFunction("( \
  10*cos(t)+t ; \
  5*sin(2*t)+t ; \
  atan2((10*cos(2*t)+1),(-10*sin(t)+1)) ; \
  sqrt((-10*sin(t)+1)^2+(10*cos(2*t)+1)^2))")) # actual trajectory

# Continuous measurements coming from the truth
measured_psi = x_truth[2].sample(dt).make_continuous()
measured_psi += RandTrajectory(tdomain, dt, Interval(-0.01,0.01)) # adding some noise
measured_speed = x_truth[3].sample(dt);
measured_speed += RandTrajectory(tdomain, dt, Interval(-0.01,0.01)) # adding some noise

# Creating random map of landmarks
map_area = IntervalVector(2, [-8,8])
v_map = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks = 30)

# The following function generates a set of [range]x[bearing] values
v_obs = DataLoader.generate_observations(x_truth, v_map, nb_obs = 10)

# Adding uncertainties on the measurements
for obs in v_obs: # for each observation:
  obs[1].inflate(0.3) # range
  obs[2].inflate(0.1) # bearing


# =============== 1. Defining domains for our variables ================

x = TubeVector(tdomain, dt, 4)                    # 4d tube for state vectors
v = TubeVector(tdomain, dt, 4)                    # 4d tube for derivatives of the states
u = TubeVector(tdomain, dt, 2)                    # 2d tube for inputs of the system

x[2] = Tube(measured_psi, dt).inflate(0.01)       # measured_psi is a set of measurements
x[3] = Tube(measured_speed, dt).inflate(0.01)


# =========== 2. Defining contractors to deal with equations ===========

ctc_f = CtcFunction(
  Function("v[4]", "x[4]", "u[2]",
           "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"))

ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # a+b=c
ctc_minus = CtcFunction(Function("a", "b", "c", "a-b-c")) # a-b=c
# We also use the predefined contractors ctc.polar, ctc.eval, no need to build them


# =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork()   # creating a network
cn.add(ctc_f, [v, x, u])   # adding the f constraint

for y in v_obs: # we add the observ. constraint for each range-only measurement

  # Intermediate variables
  alpha = cn.create_dom(Interval()) # absolute angle robot-landmark
  d = cn.create_dom(IntervalVector(2)) # dist robot-landmark
  p = cn.create_dom(IntervalVector(4)) # state at t_i

  cn.add(ctc_plus, [y[2], p[2], alpha])
  cn.add(ctc_minus, [cn.subvector(y,3,4), cn.subvector(p,0,1), d])
  cn.add(ctc.polar, [d, y[1], alpha])
  cn.add(ctc.eval, [y[0], p, x, v])


# ======================= 4. Solving the problem =======================

cn.contract(True)


# ============================ 5. Graphics =============================

beginDrawing()
fig = VIBesFigMap("fig")
fig.set_properties(50, 50, 900, 550)
fig.add_trajectory(x_truth, "xtruth", 0, 1, 2)
fig.add_tube(x, "x", 0, 1)
fig.smooth_tube_drawing(True)

for b in v_map:
  fig.add_beacon(b.mid(), 0.2) # drawing beacons

for y in v_obs:
  t_obs = y[0].mid()
  t_state = x_truth(t_obs)
  fig.draw_pie(t_state[0], t_state[1], y[1] | Interval(0.01), t_state[2] + y[2], "lightGray") # drawing range-bearing measurements
  fig.draw_pie(t_state[0], t_state[1], y[1], t_state[2] + y[2], "darkGray") # drawing range-bearing measurements
  fig.draw_vehicle(t_obs, x_truth, size=0.7)

fig.show(0.)
endDrawing()


# Checking if this example still works:
sys.exit(0 if x.contains(x_truth) == BoolInterval.YES else 1)