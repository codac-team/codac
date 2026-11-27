#!/usr/bin/env python3

import math
from codac import *

td = create_tdomain([0,5],0.01,True)
s = ScalarVar()
v = SlicedTube(td,AnalyticFunction([s],[-2*sin(s),2*cos(2*s)]))
v.inflate(0.01)

# Reproduce the preceding example to obtain the same shared temporal partition
x = SlicedTube(td,IntervalVector(2))
t = Interval(4.3,4.4)
z = IntervalVector([[-0.73,-0.69],[0.64,0.68]])
ctc_eval = CtcEval()
ctc_eval.contract(t,z,x,v)
CtcDeriv().contract(x,v)

y = SlicedTube(td,AnalyticFunction([s],[2*cos(s),sin(2*s)]))
y.inflate(0.05)
t = td.t0_tf()
z = IntervalVector([[-1,0],[0.4,1.2]])

z_initial = IntervalVector(z)
ctc_eval.contract(t,z,y,v)
assert not y.is_empty() and not y.is_unbounded()
print('Contracted tube:',y)
print('Contracted observation:',t,z)

fig = Figure2D('ceval_lissajous_box',GraphicOutput.VIBES | GraphicOutput.IPE)
fig.set_window_properties([50,50],[750,480])
fig.set_axes([[-2.5,2.5],[-1.6,1.6]])

# Draw the contracted tube and both observation boxes
fig.draw_tube(y,StyleGradientProperties(ColorMap.blue_tube(),'z:0'))
fig.draw_box(z_initial,StyleProperties(Color('#148EB5'),'z:2'))
fig.draw_box(z,StyleProperties(Color('#E02020'),'z:3'))

# Reference trajectory and feasible measurement
truth_color = Color.white()
truth = [Vector([2*math.cos(k/120),math.sin(2*k/120)]) for k in range(601)]
fig.draw_polyline(truth,StyleProperties(truth_color,'z:1'))
tr = 4.35
zr = Vector([2*math.cos(tr),math.sin(2*tr)])
assert t.contains(tr) and z.contains(zr) and y(tr).contains(zr)
theta = math.atan2(2*math.cos(2*tr),-2*math.sin(tr))
fig.draw_tank(Vector([zr[0],zr[1],theta]),0.25,
  StyleProperties([Color.black(),Color.yellow()],'z:4'))

