from codac import *

wpts = [
  [2,1], [4,0], [6,2], [8,0], [6,2]
]

s = RobotSimulator()
u = SampledVectorTraj()
x = s.simulate([0,0,0,0], 1e-2, wpts, u)

g = Figure2D("Robot simulation", GraphicOutput.VIBES)

g.draw_tank(x(5.), 0.5, [Color.black(),Color.yellow()])
g.draw_trajectory(x)
for wi in wpts:
  g.draw_circle(wi, 0.1, Color.red())

g.set_axes( \
  axis(0,x.codomain()[0].inflate(1.)), \
  axis(1,x.codomain()[1].inflate(1.)) \
).auto_scale()