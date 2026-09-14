from codac import *
import math
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

fig_save = Figure2D("Animation with exports", GraphicOutput.VIBES | GraphicOutput.IPE)
fig_save.set_window_properties([600,50],[500,500]) # position, window size
fig_save.set_axes(axis(0,[-0.5,4]), axis(1,[-0.5,1.5]))

X = Vector([0,0,0])
t = 0
cmap = ColorMap.rainbow()

while t <2*PI:
  fig_save.draw_tank(X, 0.05, cmap.color(t/(2*PI)))
  X+=Vector([0.1*math.cos(X[2]),0.1*math.sin(X[2]),0.1*math.cos(t)])
  time.sleep(0.1)
  if (Interval(PI).inflate(0.1).contains(t)):
    fig_save.save("animation_tank.png")
    fig_save.save("animation_tank.jpg")
    fig_save.save("animation_tank.bmp")
    fig_save.save("animation_tank.svg")
  t += PI/20