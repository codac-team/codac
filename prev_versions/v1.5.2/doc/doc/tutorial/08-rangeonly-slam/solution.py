from codac import *
import math
import random
import time


# =========== CREATING DATA ===========

dt = 0.05
iteration_dt = 0.2
tdomain = Interval(0,15) # [t0,tf]

# Initial pose x0=(0,0,2)
x0 = (0,0,2)

# System input
u = Trajectory(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt)

# Noise
i_n = Interval(-0.03,0.03) # the noises are known to be bounded by i_n

n_u = RandTrajectory(tdomain, dt, i_n) # input noise
n_theta = RandTrajectory(tdomain, dt, i_n) # heading noise

# Actual trajectories (state + derivative)
v_truth = TrajectoryVector(3)
x_truth = TrajectoryVector(3)
v_truth[2] = u + n_u
x_truth[2] = v_truth[2].primitive() + x0[2]
v_truth[0] = 10*cos(x_truth[2])
v_truth[1] = 10*sin(x_truth[2])
x_truth[0] = v_truth[0].primitive() + x0[0]
x_truth[1] = v_truth[1].primitive() + x0[1]

# Bounded trajectories (dead reckoning)
v = TubeVector(tdomain, dt, 3)
x = TubeVector(tdomain, dt, 3)
v[2] = Tube(u, dt).inflate(i_n.rad()) # command u with bounded uncertainties
x[2] = Tube(x_truth[2]+n_theta, dt).inflate(i_n.rad()) # heading measurement with bounded uncertainties
v[0] = 10*cos(x[2])
v[1] = 10*sin(x[2])
x = v.primitive()+IntervalVector(x0) # dead reckoning

# Set of landmarks
v_m = [ (6,12), (-2,-5), (-3,20), (3,4) ]


# =========== GRAPHICS ===========

beginDrawing()

fig_map = VIBesFigMap("slam")
fig_map.set_properties(50, 50, 1200, 600)
fig_map.add_tube(x, "x", 0, 1)
fig_map.add_trajectory(x_truth, "truth", 0, 1, "white")
fig_map.smooth_tube_drawing(True)
fig_map.add_landmarks(v_m, 0.4)
fig_map.show(1.)


# =========== CONTRACTOR NETWORK ===========

v_m_boxes = [IntervalVector(2) for _ in v_m]

# Contractor Network:

cn = ContractorNetwork()

t = tdomain.lb()
prev_t_obs = t

while t < tdomain.ub():

  if t-prev_t_obs > 2*dt: # new observation each 2*delta
    
    # Creating new observation to a random landmark

    landmark_id = random.randint(0,len(v_m)-1) # a random landmark is perceived

    pos_x = x_truth(t)[0:2]
    pos_b = v_m[landmark_id]

    yi = Interval(sqrt(pow(pos_x[0]-pos_b[0],2)+pow(pos_x[1]-pos_b[1],2)))
    yi.inflate(0.03) # adding range bounded uncertainty

    prev_t_obs = t

    # Adding related observation constraints to the network

    # Alias (for ease of reading)
    b = v_m_boxes[landmark_id]

    # Intermediate variables
    ti = Interval(t)
    xi = IntervalVector(3)

    # Contractors
    cn.add(ctc.eval, [ti, xi, x, v])
    cn.add(ctc.dist, [xi[0], xi[1], b[0], b[1], yi])

  contraction_dt = cn.contract_during(iteration_dt)
  if iteration_dt>contraction_dt:
    time.sleep(iteration_dt-contraction_dt) # iteration delay

  # Display the current slice [x](t)
  fig_map.draw_box(x(t).subvector(0,1))

  t+=dt

cn.contract(True) # lets the solver run the remaining contractions

fig_map.show()
for b in v_m_boxes:
  fig_map.draw_box(b)

endDrawing()