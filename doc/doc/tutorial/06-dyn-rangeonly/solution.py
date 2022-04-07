from codac import *
import math

# F.1.
dt = 0.01
tdomain = Interval(0,5) # temporal domain [t0,tf]
actual_v = TrajectoryVector(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt)
v = TubeVector(actual_v, dt).inflate(0.03)
x = TubeVector(tdomain, dt, 2)

# F.2.
cn = ContractorNetwork()
yi = IntervalVector([[-0.84,-0.83],[-0.76,-0.75]])
ti = 2
cn.add(ctc.eval, [ti,yi,x,v])
cn.contract()

actual_x = TrajectoryVector(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt)
beginDrawing()
fig_map = VIBesFigMap("Map")
fig_map.set_properties(100, 100, 600, 300)
fig_map.add_trajectory(actual_x, "x*", 0, 1, "black")
fig_map.add_tube(x, "x", 0, 1)
fig_map.add_beacon(actual_x(ti), 0.05)
fig_map.axis_limits(-2.5,2.5,-0.1,0.1, True)
fig_map.show(0.5)


# Other questions are related to the example Getting Started

endDrawing()