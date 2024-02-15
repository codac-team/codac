from codac import *
import numpy as np

# =================== Parameters, truth and data ===================

beginDrawing()

v_corners = [
  [-4.5,6],[2.5,6.5],[2.8,-1.2],[4.5,-0.8],[4.8,4],
  [11.5,6],[19,4],[19.5,-1],[25,-1.5],[24.5,-4.5],
  [16.8,-5],[16.5,1.2],[14.7,1],[14.3,-5],[8.7,-4.5],
  [8.5,1.2],[6.5,1],[6.3,-4.8],[3,-5],[-4,1]
]

v_walls = [] # building walls from corners
for i in range(len(v_corners)):
  v_walls.append(Wall(v_corners[i],v_corners[(i+1)%len(v_corners)]))

dt = 0.01                # timestep for tubes accuracy
dt_traj = 0.1*dt         # timestep for simulation
tdomain = Interval(0,10) # temporal limits [t_0,t_f]

# Analytical true trajectory (not used for resolution)
x_truth = TrajectoryVector(tdomain, TFunction("( \
  2*t+(1+cos(t*pi))/2 ; \
  3*tanh(4*(1+cos((t+0.5)/2*pi))/2-2) ; \
  atan2( \
    -3*pi*(1-sqr(tanh(2*cos((2*t+1)*pi/4))))*sin((2*t+1)*pi/4), \
    2-pi*sin(pi*t)/2) ; \
  sqrt( \
    sqr(  -3*pi*(1-sqr(tanh(2*cos((2*t+1)*pi/4))))*sin((2*t+1)*pi/4)  ) + \
    sqr(  2-pi*sin(pi*t)/2  ) \
  ))"))

# Continuous measurements coming from the truth
measured_psi = x_truth[2].sample(dt_traj).make_continuous()
measured_psi += RandTrajectory(tdomain, dt_traj, Interval(-0.05,0.05)) # adding some noise
measured_speed = x_truth[3].sample(dt_traj)
measured_speed += RandTrajectory(tdomain, dt_traj, Interval(-0.1,0.1)) # adding some noise

# =============== Defining domains for the variables ===============

x = TubeVector(tdomain, dt, 4) # 4d tube for state vectors
v = TubeVector(tdomain, dt, 4) # 4d tube for derivatives of the states
u = TubeVector(tdomain, dt, 2) # 2d tube for inputs of the system

x[2] = Tube(measured_psi, dt).inflate(0.05) # measured_psi/speed are continuous measurements..
x[3] = Tube(measured_speed, dt).inflate(0.1) # ..of the headings and speeds

# =============== Defining contractors to deal with equations ===============

ctc_f = CtcFunction( # constraint v=f(x,u) <=> v-f(x,u)=0
  Function("v[4]", "x[4]", "u[2]",
           "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"))

ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # constraint a+b-c=0 <=> a+c=c
ctc_minus = CtcFunction(Function("a", "b", "c", "a-b-c")) # constraint a-b-c=0 <=> a-b=c

v_ctc_segm = []
ctc_walls = CtcUnion(2) # contractor for the constraint \mathbf{q} \in \matbb{W} with \mathbb{W} the set of walls
for wi in v_walls:
  v_ctc_segm.append(CtcSegment(wi.c1[0],wi.c1[1],wi.c2[0],wi.c2[1]))
  ctc_walls |= v_ctc_segm[-1]

fig = VIBesFigMap("Set inversion")
SIVIA([[-5,26],[-8.5,8.5]], ctc_walls, 0.5, fig_name="Set inversion")
fig.set_properties(50, 50, 900, 500)
fig.axis_limits(-5,26,-8.5,8.5)

# We also use the predefined contractors ctc::polar, ctc::eval, ctc::dist,  no need to build them

# =============== Adding the contractors to a network ===============

cn = ContractorNetwork() # creating a network
cn.add(ctc_f, [v, x, u]) # adding the f contractor, that is common over [t0,tf]

fig = VIBesFigMap("Map")
fig.set_properties(50, 50, 900, 500)
fig.add_tube(x, "x", 0, 1)
fig.add_trajectory(x_truth, "x*", 0, 1, 2)
fig.smooth_tube_drawing(True)
fig.show(0.)
fig.axis_limits(-5,26,-8.5,8.5)

for wi in v_walls: # drawing the map of walls
  fig.draw_line([wi.c1[0],wi.c2[0]], [wi.c1[1],wi.c2[1]], "lightGray")

# For each observation of the environment:
for t in np.arange(tdomain.lb(), tdomain.ub(), 0.8): 
  pi_truth = x_truth(t)[0:2]
  ti = Interval(t)
  pi = IntervalVector(4)
  qi = IntervalVector(2)
  cn.add(ctc.eval, [ti, pi, x, v]) # <=> x(ti)=pi
  cn.add(ctc_walls, [qi]) # <=> \mathbf{q]_i \in \mathbb{W}

  di = IntervalVector(2)
  yi = IntervalVector(2)
  yi[0] = Interval(shorter_dist_to_walls(v_walls, pi_truth, x_truth(t)[2]))
  yi[1] = Interval(x_truth(t)[2]) # bearing is zero, azimuth is the heading

  yi[0].inflate(0.02)
  yi[1].inflate(0.05)

  cn.add(ctc_minus, [qi, cn.subvector(pi,0,1), di]) # <=> qi-pi[0,1]=di
  cn.add(ctc.polar, [di, yi]) # di[0]=y1[0]*cos(yi[1]), di[1]=y1[0]*sin(yi[1])

  fig.draw_pie(pi_truth[0], pi_truth[1], yi[0] | Interval(0), yi[1], "lightGray[#FFFFFF88]")
  fig.draw_pie(pi_truth[0], pi_truth[1], yi[0], yi[1])

  fig.draw_vehicle([x_truth(t)[0], x_truth(t)[1], x_truth(t)[2]], 0.7)

cn.contract(True)

fig.show(0.)
fig.axis_limits(-5,26,-8.5,8.5)
endDrawing()