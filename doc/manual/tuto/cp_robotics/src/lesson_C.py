from codac import *

class MyCtc(Ctc_IntervalVector):

  def __init__(self, M_: list):
    Ctc_IntervalVector.__init__(self, 2) # the contractor acts on 2d boxes
    self.M = M_                          # attribute needed later on for the contraction

  def contract(self, a):
    u = IntervalVector.empty(2)
    for mi in self.M:
      u |= a & mi
    a = IntervalVector(u)
    return a

    # Insert contraction formula here (question B.2)

    return a # in Python, the updated value must be returned


# [C-q4-beg]
def g(t,x,M):

  obs = [] # several landmarks can be seen at some ti
  scope_range = Interval(0,10)
  scope_angle = Interval(-PI/4,PI/4)

  for mi in M:
  
    r = sqrt(sqr(mi[0]-x[0]) + sqr(mi[1]-x[1]))
    a = atan2(mi[1]-x[1], mi[0]-x[0]) - x[2]

    # If the landmark is seen by the robot:
    if scope_range.is_superset(r) and scope_angle.is_superset(a):
      obs.append(IntervalVector([t,r,a]))

  return obs
# [C-q4-end]


# [C-q2-beg]
srand() # initialize the random seed (from C++)
N = 50 # number of landmarks
X = IntervalVector([[-40,40],[-40,40]]) # landmarks distribution zone

M = [] # creating the landmarks
for i in range (0,N):
  M.append(IntervalVector(X.rand()).inflate(0.2))

fig = Figure2D("Robot simulation", GraphicOutput.VIBES)
fig.set_axes(
  axis(0, X[0].inflate(10), "x_1"),
  axis(1, X[1].inflate(10), "x_2")
).auto_scale()

for mi in M: # displaying the landmarks
  fig.draw_box(mi, [Color.dark_green(),Color.green()])
# [C-q2-end]


# [C-q3-beg]
wpts = [] # creating random waypoints for simulating the robot trajectory
X = IntervalVector([[-35,35],[-35,35]]) # robot evolution zone
for i in range (0,5): # 5 waypoints
  wpts.append(X.rand())

s = RobotSimulator()
s.w_max = 0.2 # maximum turning speed
u = SampledVectorTraj() # the simulator will return the inputs (not used)
x_truth = s.simulate(
  [0,0,0,0], # initial state (will be supposed unknown)
  1e-2, # simulation time step
  wpts, u)
# [C-q3-end]


# [C-q5-beg]
prev_t = 0.
time_between_obs = 3.
obs = []

t = x_truth.tdomain().lb()
while t < x_truth.tdomain().ub():

  if t-prev_t > time_between_obs:
    obs_ti = g(t,x_truth(t),M) # computing the observation vector

    for yi in obs_ti:
      prev_t = yi[0].mid()
      fig.draw_pie(x_truth(t).subvector(0,1), yi[1]|0., yi[2]+x_truth(t)[2], Color.light_gray())
      fig.draw_pie(x_truth(t).subvector(0,1), yi[1],    yi[2]+x_truth(t)[2], Color.red())

    obs.extend(obs_ti)

  t += 1e-2
# [C-q5-end]


# [C-q6-beg]
_x = VectorVar(4)
h = AnalyticFunction([_x],
  [
    # Positions are not known..
    Interval(-oo,oo), 
    Interval(-oo,oo),
    # But headings (x3) and velocities (x4) are bounded..
    _x[2] + 2e-2*Interval(-1,1),
    _x[3] + 2e-2*Interval(-1,1)
  ])

tdomain = create_tdomain(x_truth.tdomain(), 5e-2, True)
# The tube x is created from the interval evaluation of the actual trajectory
x = h.tube_eval(SlicedTube(tdomain,x_truth))
# The tube v is created as a four-dimensional tube of infinite values
v = SlicedTube(tdomain, IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo],[-oo,oo]]))
# [C-q6-end]


x1,x2,x3 = VectorVar(2), VectorVar(4), VectorVar(2)
f_minus = AnalyticFunction([x1,x2,x3], [
    x1[0]-x2[0]-x3[0],
    x1[1]-x2[1]-x3[1]
])
ctc_minus = CtcInverse(f_minus, [0,0])

x1,x2,x3 = ScalarVar(), ScalarVar(), ScalarVar()
f_plus = AnalyticFunction([x1,x2,x3], x1+x2-x3)
ctc_plus = CtcInverse(f_plus, 0)


# [C-q7-beg]
ctc_deriv = CtcDeriv()

_x, _v = VectorVar(4), VectorVar(4)
f = AnalyticFunction([_x,_v],
  [
    _v[0]-_x[3]*cos(_x[2]),
    _v[1]-_x[3]*sin(_x[2])
  ])

ctc_f = CtcInverse(f, [0,0])
# + other contractors from previous lessons:
# ctc_plus, ctc_minus, ctc_polar, ctc_constell
# [C-q7-end]

ctc_polar = CtcPolar()
ctc_constell = MyCtc(M)


# [C-q8-beg]
def ctc_one_obs(xi,yi,mi,ai,si):

  mi,xi,si = ctc_minus.contract(mi,xi,si)
  xi[2],yi[2],ai = ctc_plus.contract(xi[2],yi[2],ai)
  si[0],si[1],yi[1],ai = ctc_polar.contract(si[0],si[1],yi[1],ai)
  mi = ctc_constell.contract(mi)

  return xi,yi,mi,ai,si

def ctc_all_obs(x):
  global v

  for yi in obs:
    xi = x(yi[0]) # evalution of the tube at time ti=yi[0]
    ai = Interval()
    si = IntervalVector(2)
    mi = IntervalVector(2)
    xi,yi,mi,ai,si = fixpoint(ctc_one_obs, xi,yi,mi,ai,si)
    x.set(xi,yi[0]) # restriction on the tube x at time ti=yi[0]

  x,v = ctc_f.contract_tube(x,v)
  x = ctc_deriv.contract(x,v)

  return x

x = fixpoint(ctc_all_obs, x)
# [C-q8-end]

print(x)

# [C-q9-beg]
fig.draw_tube(x)
fig.draw_trajectory(x_truth)
xf = x_truth(x_truth.tdomain().ub())
fig.draw_tank(xf, 2., [Color.black(),Color.yellow()])
fig.draw_pie(xf, [0,10], xf[2]+Interval(-PI/4,PI/4), Color.dark_gray())
for mi in M:
   fig.draw_box(mi, [Color.dark_green(),Color.green()])
# [C-q9-end]