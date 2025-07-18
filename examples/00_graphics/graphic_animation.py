from codac import *
import time

fig = Figure2D("Animation", GraphicOutput.VIBES | GraphicOutput.IPE)
fig.set_window_properties([50,50],[500,500]) # position, window size
fig.set_axes(axis(0,[-10,10]), axis(1,[-10,10]))

theta = 0
steps = 30
for i in range (steps):
  fig.clear()
  fig.draw_point([5*cos(theta).mid(), 5*sin(theta).mid()], Color.red())
  theta += 2*PI / steps
  time.sleep(0.1)