#!/usr/bin/env python3

import math
from codac import *


def draw_tube(fig, x, color, layer, z):
  style = StyleProperties([color,color],layer,f'z:{z}')
  for s in x:
    if not s.is_gate():
      fig.draw_box(IntervalVector([s.t0_tf(),s.codomain()]),style)


td = create_tdomain([-PI,PI/2],0.01,True)
s = ScalarVar()
v = SlicedTube(td,AnalyticFunction([s],cos(s)+Interval(-0.1,0.1)))
x = v.primitive(Interval(-0.1,0.1))
t, z = Interval(-0.5,0.3), Interval(0.3,1.1)

fig = Figure2D('ceval_x',GraphicOutput.VIBES | GraphicOutput.IPE)
fig.set_window_properties([50,50],[800,500])
fig.set_axes([[-PI-0.1,PI/2+0.1],[-1.6,1.7]])

# Initial tube and observation
blue = Color('#148EB5')
red = Color('#E02020')
truth_color = Color('#003E5F')
draw_tube(fig,x,Color('#D6D6D6'),'initial_tube',-3)
fig.draw_box(IntervalVector([t,z]),StyleProperties(blue,'initial_observation','z:1'))

# Local contraction followed by global temporal propagation
CtcEval().contract(t,z,x,v)
#CtcDeriv().contract(x,v)
assert not x.is_empty()
draw_tube(fig,x,Color('#888888'),'contracted_tube',-2)
fig.draw_box(IntervalVector([t,z]),StyleProperties(red,'contracted_observation','z:2'))

# Reference trajectory and a feasible measurement
truth = [Vector([ti,math.sin(ti)+0.1+0.03*ti])
         for ti in [-PI+1.5*PI*k/600 for k in range(601)]]
fig.draw_polyline(truth,StyleProperties(truth_color,'reference_trajectory','z:3'))
tr = 0.25
zr = math.sin(tr)+0.1+0.03*tr
assert t.contains(tr) and z.contains(zr) and x(tr).contains(zr)
fig.draw_circle(Vector([tr,zr]),0.025,StyleProperties([truth_color,truth_color],'measurement','z:4'))

print('Contracted observation:',t,z)
