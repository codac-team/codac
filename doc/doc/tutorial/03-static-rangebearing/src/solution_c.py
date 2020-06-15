from pyibex import *
from tubex_lib import *
import math

x_truth = (2,1,math.pi/6) # actual state vector (pose)
y_truth = (6,math.pi/6) # actual measurement

x = IntervalVector(3) # estimated state vector
x[2] = Interval(x_truth[2]) # the heading is known

m = IntervalVector((5,6.2)) # landmark
m.inflate(0.2) # uncertainties related to the landmark position

y = IntervalVector(y_truth) # bounded measurement
y[0].inflate(0.3) # range
y[1].inflate(0.1) # bearing

# Contractors
ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # a+b=c
ctc_minus = CtcFunction(Function("a", "b", "c", "a-b-c")) # a-b=c
# Note that ctc.polar already exists

cn = ContractorNetwork()

# Intermediate variables
theta = Interval()
d = IntervalVector(2)
# Note that these intermediate variables do not have to be
# initialized with prior values.

# Solver
cn.add(ctc_plus, [y[1], x[2], theta])
cn.add(ctc_minus, [m[0], x[0], d[0]])
cn.add(ctc_minus, [m[1], x[1], d[1]])
cn.add(ctc.polar, [d, y[0], theta])
# For this ^, we could also write cn.add(ctc.polar, [d[0], d[1], y[0], theta])
cn.contract()

beginDrawing()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(100,100,500,500)
fig_map.axis_limits(0,7,0,7)
fig_map.draw_vehicle(x_truth,1)
fig_map.draw_box(m, "red")
fig_map.draw_pie(x_truth[0],x_truth[1],(Interval(0.1)|y[0]),y[1]+x_truth[2],"lightGray")
fig_map.draw_pie(x_truth[0],x_truth[1],y[0],y[1]+x_truth[2])
fig_map.draw_box(x.subvector(0,1))
fig_map.show()

endDrawing()