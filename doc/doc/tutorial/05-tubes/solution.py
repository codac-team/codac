from codac import *
import math

# E.1.
dt = 0.01
tdomain = Interval(0,5) # temporal domain [t0,tf]
actual_x = TrajectoryVector(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt)

# E.2.
b=(0.5,1)

# E.3.
beginDrawing()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(100, 100, 600, 300)
fig_map.add_trajectory(actual_x, "x*", 0, 1, "black")
fig_map.add_beacon(b, 0.1)                    # 0.1: landmark width
fig_map.axis_limits(-2.5,2.5,-0.1,0.1, True)
fig_map.show(0.5)                             # argument is robot size

# E.4.
actual_y = sqrt(sqr(actual_x[0]-b[0])+sqr(actual_x[1]-b[1]))

# E.5.
fig_dist = VIBesFigTube("Distance to the landmark")
fig_dist.set_properties(100, 100, 600, 300)
fig_dist.add_trajectory(actual_y, "y*")
fig_dist.show()

# E.6.
x = TubeVector(actual_x, dt)

# E.7.
x.inflate(0.2)

# E.8.
fig_map.add_tube(x, "x", 0, 1)
fig_map.show(0.5)

# E.9.
y = Tube(actual_y, dt)

# E.10.
b_box = IntervalVector(b) # creating a domain for b

cn = ContractorNetwork()
cn.add(ctc.dist, [x,b_box,y])
cn.contract()
fig_map.show(0.5)

# E.11.
actual_v = TrajectoryVector(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt)
v = TubeVector(actual_v, dt).inflate(0.01)

# E.12.
cn = ContractorNetwork()
cn.add(ctc.dist, [x,b_box,y])
cn.add(ctc.deriv, [x,v])
cn.contract()
fig_map.show(0.5)

# E.13. (optional)
n = RandTrajectory(tdomain, 0.01, Interval(-0.2,0.2))
measured_y = actual_y + n
y = Tube(measured_y, dt)

# E.14. (optional)
y.inflate(0.2)

# E.15. (optional)
x = TubeVector(actual_x, dt).inflate(0.2) # reinitialization
cn = ContractorNetwork()
cn.add(ctc.dist, [x,b_box,y])
cn.add(ctc.deriv, [x,v])
cn.contract()
fig_map.show(0.5)

endDrawing()