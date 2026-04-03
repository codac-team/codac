# [D-q1-beg]
from codac import *
import random
import numpy as np

dt = 0.02 # temporal discretization
t0tf = Interval(0,15) # temporal domain [t0,tf]

# System input
t = ScalarVar()
# Input u(.) is given as an analytic trajectory
u = AnalyticTraj(AnalyticFunction([t],3*(sin(t)^2)+t/100), t0tf).sampled(dt)

# Implementing manually the evolution function (Eq. (2))
truth_heading = u.primitive()
truth_px = (cos(truth_heading)*10.).primitive()
truth_py = (sin(truth_heading)*10.).primitive()

# Actual state trajectory
# Note that this trajectory is unknown to the estimation process
s1,s2,s3 = ScalarVar(),ScalarVar(),ScalarVar()
f_concat = AnalyticFunction([s1,s2,s3], [s1,s2,s3])
truth_x = f_concat.traj_eval(truth_px,truth_py,truth_heading)

DefaultFigure.draw_trajectory(truth_x)
DefaultFigure.draw_tank(truth_x(t0tf.ub()), 1., [Color.dark_gray(),Color.yellow()])
# [D-q1-end]

# [D-q2-beg]
tdomain = create_tdomain(t0tf, dt) # temporal discretization over [t0,tf]
x = SlicedTube(tdomain, IntervalVector(3))
# [D-q2-end]


# [D-q2b-beg]
print(x(0.))
# [D-q2b-end]


# [D-q3-beg]
x = tube_cart_prod(
  SlicedTube(tdomain, IntervalVector(2)),
  # Heading measurement with bounded uncertainties:
  SlicedTube(tdomain, truth_heading).inflate(0.03)
)
# [D-q3-end]


# [D-q4-beg]
x.set([0,0,0], 0.) # setting a vector value (0,0,0) at t=0
print(x(0.))
# [D-q4-end]


# [D-q5-beg]
v = SlicedTube(tdomain, IntervalVector(3))
# [D-q5-end]


# [D-q6-beg]
vx,vv = VectorVar(3),VectorVar(3)
f_evol = AnalyticFunction([vx,vv], [
    vv[0]-10*cos(vx[2]),
    vv[1]-10*sin(vx[2]),
  ])

ctc_f = CtcInverse(f_evol, [0,0]) # [0,0] stands for the implicit form f(..)=0
# [D-q6-end]


# [D-q6b-beg]
ctc_deriv = CtcDeriv()
# [D-q6b-end]


# [D-q7-beg]
ctc_f.contract_tube(x,v)
ctc_deriv.contract(x,v)
# [D-q7-end]


# [D-q8-beg]
DefaultFigure.draw_tube(x, [Color.light_gray(),Color.light_gray()])
DefaultFigure.draw_trajectory(truth_x)
# [D-q8-end]


# [D-q9-beg]
b = [(6,12),(-2,-5),(-3,20),(3,4),(-10,0)]
# [D-q9-end]


# [D-q10-beg]
Y = []
for ti in np.arange(0,15):
  k = random.randint(0,len(b)-1) # a random landmark is perceived
  d = sqrt(sqr(truth_x(ti)[0]-b[k][0])+sqr(truth_x(ti)[1]-b[k][1]))
  Y.append([k,ti,d])
# [D-q10-end]

Y = []
for ti in np.arange(0,15):
  k = random.randint(0,len(b)-1) # a random landmark is perceived
  d = sqrt(sqr(truth_x(ti)[0]-b[k][0])+sqr(truth_x(ti)[1]-b[k][1]))
  # [D-q11-beg]
  d += Interval(-0.03,0.03)
  # or equivalently: d.inflate(3e-2)
  # [D-q11-end]
  Y.append([k,ti,d])


# [D-q12-beg]
vb = VectorVar(2)
vd = ScalarVar()
f_dist = AnalyticFunction([vx,vb,vd], sqrt(sqr(vx[0]-vb[0])+sqr(vx[1]-vb[1]))-vd)
ctc_dist = CtcInverse(f_dist, 0) # also expressed in a implicit form g(x,b,d)=0
# [D-q12-end]


# [D-q13-beg]
def contractors_list(x,v):
  ctc_deriv.contract(x,v)
  ctc_f.contract_tube(x,v)
  for yi in Y: # for each range-only measurement
    ti = yi[1]
    pi = x(ti)
    bi = IntervalVector(b[yi[0]])
    di = yi[2]
    pi,bi,di = ctc_dist.contract(pi,bi,di)
    x.set(pi, ti)
  return x,v

x,v = fixpoint(contractors_list, x,v)
# [D-q13-end]

# [D-q14-beg]

# [D-q14-end]