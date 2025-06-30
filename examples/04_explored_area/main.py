from codac import *

# Example: A sampled trajectory, 'sampled_f' (composed of time-stamped positions with
# linear interpolation between them), is first obtained by discretizing an analytical
# expression (the function 'f,' which represents a Lissajous curve) and then appending
# an additional position. This trajectory is subsequently used in another analytical
# expression (function 'h'). The projection of an inverse separator is then employed
# to validate the result.

t = ScalarVar()
f = AnalyticFunction(
  [t],
  [ 2*cos(t), sin(2*t) ]
)

tdomain = [0,5]
sampled_f = AnalyticTraj(f,tdomain).sampled(0.8)
sampled_f.set([0,-1], 6.) # appending the position (0,-1) at t=6

w = VectorVar(3)
g = sampled_f.as_function()
h = AnalyticFunction(
  [w], # w=(x,y,t)
  sqr(w[0]-g(w[2])[0])+sqr(w[1]-g(w[2])[1])
)

s_h = SepInverse(h, [0,0.1])
s_projh = SepProj(s_h, [0,1], [sampled_f.tdomain()])

DefaultFigure.set_window_properties([75,75],[700,700])
draw_while_paving([[-3,3],[-2,2]], s_projh, 5e-2)
DefaultFigure.draw_trajectory(sampled_f)
DefaultFigure.draw_trajectory(AnalyticTraj(f,tdomain), Color.dark_gray())