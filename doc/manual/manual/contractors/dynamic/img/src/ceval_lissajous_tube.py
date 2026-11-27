#!/usr/bin/env python3

import math
from codac import *

td = create_tdomain([0,5],0.01,True)
s = ScalarVar()
v = SlicedTube(td,AnalyticFunction([s],[-2*sin(s),2*cos(2*s)]))
v.inflate(0.01)

x = SlicedTube(td,IntervalVector(2))
t = Interval(4.3,4.4)
z = IntervalVector([[-0.73,-0.69],[0.64,0.68]])

CtcEval().contract(t,z,x,v)
CtcDeriv().contract(x,v)
assert not x.is_empty() and not x.is_unbounded()

fig = Figure2D('ceval_lissajous_tube',GraphicOutput.VIBES | GraphicOutput.IPE)
fig.set_window_properties([50,50],[750,480])
fig.set_axes([[-2.5,2.5],[-1.6,1.6]])

# Position tube after temporal propagation
fig.draw_tube(x,StyleGradientProperties(ColorMap.blue_tube(),'position_tube','z:-2'))

# Reference trajectory and observation
truth_color = Color.white()
truth = [Vector([2*math.cos(k/120),math.sin(2*k/120)]) for k in range(601)]
fig.draw_polyline(truth,StyleProperties(truth_color,'reference_trajectory','z:1'))
fig.draw_box(z,StyleProperties(Color('#148EB5'),'observation','z:2'))
tr = 4.35
zr = Vector([2*math.cos(tr),math.sin(2*tr)])
assert t.contains(tr) and z.contains(zr) and x(tr).contains(zr)
theta = math.atan2(2*math.cos(2*tr),-2*math.sin(tr))
fig.draw_tank(Vector([zr[0],zr[1],theta]),0.25,
  StyleProperties([Color.black(),Color.yellow()],'robot','z:4'))

print('Contracted observation:',t,z)
