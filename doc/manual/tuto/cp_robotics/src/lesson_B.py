from codac import *

class MyCtc(Ctc_IntervalVector):

  def __init__(self, M_: list):
    Ctc_IntervalVector.__init__(self, 2) # the contractor acts on 2d boxes
    self.M = M_                          # attribute needed later on for the contraction

  # [B-q2-beg]
  def contract(self, a):
    u = IntervalVector.empty(2)
    for mi in self.M:
      u |= a & mi
    a = IntervalVector(u)
    return a
  # [B-q2-end]

    # Insert contraction formula here (question B.2)

    return a # in Python, the updated value must be returned

# [B-q3-beg]
M = [
  IntervalVector([1.5,2.5]),
  IntervalVector([3,1]),
  IntervalVector([2,2]),
  IntervalVector([2.5,3]),
  IntervalVector([3.5,2]),
  IntervalVector([4,1]),
  IntervalVector([1.5,0.5])
]

for Mi in M:
  Mi.inflate(0.05)

a1 = IntervalVector([[1.25,3],[1.6,2.75]])
a2 = IntervalVector([[2,3.5],[0.6,1.2]])
a3 = IntervalVector([[1.1,3.25],[0.2,1.4]])

ctc_constell = MyCtc(M)

a1 = ctc_constell.contract(a1)
a2 = ctc_constell.contract(a2)
a3 = ctc_constell.contract(a3)

print(a1, '\n', a2, '\n', a3)
# [B-q3-end]

# [B-q4-beg]
x_truth = Vector([2,1,PI/6])
# [B-q4-end]

# [B-q5-beg]
DefaultFigure.set_axes(axis(0,[1,4.5]), axis(1,[0,3.5]))
DefaultFigure.draw_tank(x_truth, 0.4, [Color.black(),Color.yellow()])
for mi in M:
  DefaultFigure.draw_box(mi, [Color.dark_green(),Color.green()])
# [B-q5-end]

# [B-q6-beg]
def g(x, mi):
  r = sqrt(sqr(mi[0]-x[0]) + sqr(mi[1]-x[1]))
  b = atan2(mi[1]-x[1], mi[0]-x[0]) - x[2]
  return IntervalVector([r,b]).inflate([0.02,0.02])
# [B-q6-end]

# [B-q7-beg]
obs = []
for mi in M:
  obs.append(cart_prod(g(x_truth,mi),mi))
  # We append the position of the landmark to the measurement
  # yi = [range]×[bearing]×[2d position]

for yi in obs:
  DefaultFigure.draw_pie(x_truth.subvector(0,1), yi[0],     x_truth[2]+yi[1], Color.red())
  DefaultFigure.draw_pie(x_truth.subvector(0,1), yi[0] | 0, x_truth[2]+yi[1], Color.light_gray())
# [B-q7-end]


ctc_polar = CtcPolar()

x1,x2,x3 = VectorVar(2), VectorVar(3), VectorVar(2)
f_minus = AnalyticFunction([x1,x2,x3], [
    x1[0]-x2[0]-x3[0],
    x1[1]-x2[1]-x3[1]
])
ctc_minus = CtcInverse(f_minus, [0,0])

x1,x2,x3 = ScalarVar(), ScalarVar(), ScalarVar()
f_plus = AnalyticFunction([x1,x2,x3], x1+x2-x3)
ctc_plus = CtcInverse(f_plus, 0)


# [B-q7b-beg]
def ctc_one_obs(x,yi,mi,ai,di):
  mi,x,di = ctc_minus.contract(mi,x,di)
  x[2],yi[1],ai = ctc_plus.contract(x[2],yi[1],ai)
  di[0],di[1],yi[0],ai = ctc_polar.contract(di[0],di[1],yi[0],ai)
  return x,yi,mi,ai,di

def ctc_all_obs(x):
  for yi in obs:
    # Intermediate variables for each observation;
    ai = Interval()
    di = IntervalVector(2)
    mi = yi.subvector(2,3) # the identity (position) of the landmark is known
    # Running a fixpoint for each observation
    x,yi,mi,ai,di = fixpoint(ctc_one_obs, x,yi,mi,ai,di)
  return x

x = cart_prod([-oo,oo],[-oo,oo],x_truth[2])
x = fixpoint(ctc_all_obs, x)
# [B-q7b-end]

# [B-q8-beg]
print(x)
DefaultFigure.draw_box(x) # does not display anything if unbounded
# [B-q8-end]

# [B-q9-beg]
ctc_constell = MyCtc(M)
x = cart_prod([-oo,oo],[-oo,oo],x_truth[2]) # reset for testing

def ctc_one_obs_datasso(x,yi,mi,ai,di):
  mi,x,di = ctc_minus.contract(mi,x,di)
  x[2],yi[1],ai = ctc_plus.contract(x[2],yi[1],ai)
  di[0],di[1],yi[0],ai = ctc_polar.contract(di[0],di[1],yi[0],ai)
  # ==== Added contractor ====
  mi = ctc_constell.contract(mi) # new contractor for data association
  # ==========================
  return x,yi,mi,ai,di

def ctc_all_obs_datasso(x):
  for yi in obs:
    ai = Interval()
    di = IntervalVector(2)
    # ==== Changed domain ====
    mi = IntervalVector(2) # the identity (position) of the landmark is not known
    # ========================
    x,yi,mi,ai,di = fixpoint(ctc_one_obs_datasso, x,yi,mi,ai,di)
  return x

x = fixpoint(ctc_all_obs_datasso, x)
# [B-q9-end]


def ctc_all_obs_datasso(x):
  for yi in obs:
    ai = Interval()
    di = IntervalVector(2)
    # ==== Changed domain ====
    mi = IntervalVector(2) # the identity (position) of the landmark is not known
    # ========================
    x,yi,mi,ai,di = fixpoint(ctc_one_obs_datasso, x,yi,mi,ai,di)
    # [B-q10-beg]
    if mi.max_diam() <= 1:
      DefaultFigure.draw_point(mi.mid())
    # [B-q10-end]
  return x

x = fixpoint(ctc_all_obs_datasso, x)