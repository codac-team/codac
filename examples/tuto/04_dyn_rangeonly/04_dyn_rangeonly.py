# tubex-lib - Examples
# Dynamic range-only localization
# ----------------------------------------------------------------------------

from pyibex import *
from tubex_lib import *
import sys # only for checking if this example still works


# =================== 0. Parameters, truth and data ====================

dt = 0.01                                         # timestep for tubes accuracy
tdomain = Interval(0, 3)                          # temporal limits [t_0,t_f]=[0,3]

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


# =============== 1. Defining domains for our variables ================

x = TubeVector(tdomain, dt, 4)                    # 4d tube for state vectors
v = TubeVector(tdomain, dt, 4)                    # 4d tube for derivatives of the states
u = TubeVector(tdomain, dt, 2)                    # 2d tube for inputs of the system

x[2] = Tube(measured_psi, dt).inflate(0.01)       # measured_psi is a set of measurements
x[3] = Tube(measured_speed, dt).inflate(0.01)

e_y = Interval(-0.1,0.1)
y = [Interval(1.9+e_y), Interval(3.6+e_y), \
     Interval(2.8+e_y)]                           # set of range-only observations
b = [[8,3],[0,5],[-2,1]]                          # positions of the three 2d landmarks
t = [0.3, 1.5, 2.0]                               # times of measurements


# =========== 2. Defining contractors to deal with equations ===========

ctc_f = CtcFunction(
  Function("v[4]", "x[4]", "u[2]",
           "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"))


# =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork()   # creating a network

cn.add(ctc_f, [v, x, u])   # adding the f constraint

for i in range (0,len(y)): # we add the observ. constraint for each range-only measurement

  p = cn.create_dom(IntervalVector(4)) # intermed. variable (state at t_i)

  # Distance constraint: relation between the state at t_i and the ith beacon position
  cn.add(ctc.dist, [cn.subvector(p,0,1), b[i], y[i]])

  # Eval constraint: relation between the state at t_i and all the states over [t_0,t_f]
  cn.add(ctc.eval, [t[i], p, x, v])


# ======================= 4. Solving the problem =======================

cn.contract(True)


# ============================ 5. Graphics =============================

beginDrawing()
fig = VIBesFigMap("fig")
fig.set_properties(50, 50, 900, 550)
fig.add_trajectory(x_truth, "xtruth", 0, 1)
fig.add_tube(x, "x", 0, 1)
fig.smooth_tube_drawing(True)

for i in range(0,3):
  fig.add_beacon(b[i], 0.2) # drawing beacons
  fig.draw_ring(b[i][0], b[i][1], y[i], "darkGray") # drawing range-only measurements
  fig.draw_vehicle(t[i], x_truth, size=0.7)

fig.show(0.)
endDrawing()


# Checking if this example still works:
sys.exit(0 if x.contains(x_truth) == BoolInterval.YES else 1)