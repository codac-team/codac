#!/usr/bin/env python3

import math
from codac import *


def draw_boxes(fig, boxes, color, layer, z):
  style = StyleProperties([color,color],layer,f'z:{z}')
  for b in boxes:
    fig.draw_box(b,style)


def tube_boxes(x):
  return [IntervalVector([s.t0_tf(),s.codomain()]) for s in x if not s.is_gate()]


def draw_frame(name, x, t, z, contracted):
  fig = Figure2D(name,GraphicOutput.VIBES | GraphicOutput.IPE)
  fig.set_window_properties([50,50],[800,500])
  fig.set_axes([[-PI-0.1,PI/2+0.1],[-1.6,1.7]])
  draw_boxes(fig,initial_tube,Color('#D6D6D6'),'initial_tube',-3)
  if contracted:
    draw_boxes(fig,tube_boxes(x),Color('#888888'),'contracted_tube',-2)

  for b in initial_observations:
    fig.draw_box(b,StyleProperties(blue,'initial_observations','z:1'))
  if contracted:
    for ti, zi in zip(t,z):
      fig.draw_box(IntervalVector([ti,zi]),StyleProperties(red,'contracted_observations','z:2'))

  fig.draw_polyline(truth,StyleProperties(truth_color,'reference_trajectory','z:3'))
  for tr in ref_times:
    zr = math.sin(tr)+0.1+0.03*tr
    fig.draw_circle(Vector([tr,zr]),0.025,StyleProperties([truth_color,truth_color],'measurements','z:4'))

  return fig


td = create_tdomain([-PI,PI/2],0.01,True)
s = ScalarVar()
v = SlicedTube(td,AnalyticFunction([s],cos(s)+Interval(-0.1,0.1)))
x = v.primitive(Interval(-0.1,0.1))

t = [Interval(-0.5,0.3),Interval(-0.6,0.8),Interval(-2.3,-2.2)]
z = [Interval(0.3,1.1),Interval(-0.5,-0.4),Interval(-0.8,-0.7)]
initial_tube = tube_boxes(x)
initial_observations = [IntervalVector([ti,zi]) for ti, zi in zip(t,z)]
blue = Color('#148EB5')
red = Color('#E02020')
truth_color = Color('#003E5F')
truth = [Vector([ti,math.sin(ti)+0.1+0.03*ti])
         for ti in [-PI+1.5*PI*k/600 for k in range(601)]]
ref_times = [0.25,-0.55,-2.25]

# Keep every stage in a separate VIBes window and IPE file
figures = [draw_frame('ceval_multi_00_initial',x,t,z,False)]
ctc_eval = CtcEval()
ctc_deriv = CtcDeriv()
primitives = v.partial_primitives()

for i in range(2):
  for j in range(len(t)):
    ctc_eval.contract(t[j],z[j],x,v,primitives) # Optional primitives argument for faster integration

  assert not x.is_empty()
  figures.append(draw_frame(f'ceval_multi_{2*i+1:02d}_local',x,t,z,True))
  ctc_deriv.contract(x,v)
  assert not x.is_empty()
  figures.append(draw_frame(f'ceval_multi_{2*i+2:02d}_propagated',x,t,z,True))

for i, tr in enumerate(ref_times):
  zr = math.sin(tr)+0.1+0.03*tr
  assert t[i].contains(tr) and z[i].contains(zr) and x(tr).contains(zr)
  print('Contracted observation:',t[i],z[i])
