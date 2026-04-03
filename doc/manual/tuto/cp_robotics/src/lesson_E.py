# [E-q1-beg]
from codac import *
# We explicitly import `builtins` because the previous line shadows Python's `min` and `max`
import builtins as py
import math
# [E-q1-end]


# [E-q2-beg]
# Loading trajectories
pos = SampledTraj_Vector()
with open("./data/herdeudedac_tiles_pos.cdc", "rb") as f:
  deserialize(f, pos)
u = SampledTraj_Vector()
with open("./data/herdeudedac_tiles_odo.cdc", "rb") as f:
  deserialize(f, u)
u = u.sampled_as(pos)
# [E-q2-end]


# [E-q3-beg]
def tile_times(file):
  with open(file, "r", encoding="utf-8") as f:
    t = []
    for token in f:
      token = token.strip()
      if token:
        t.append(float(token))
    return t
# [E-q3-end]

# [E-q3b-beg]
# Observations (times of tiles detections)
T = tile_times("./data/herdeudedac_tiles_detections.txt")
# [E-q3b-end]


# [E-q4b-beg]
# Map parameters
L = 0.3 # tile width
L_eps = 1e-2 # tile thickness
# [E-q4b-end]

# [E-q4-beg]
def draw_map(X, L):
  for d in range(2):
    kmin = py.min(0, math.floor(X[d].lb() / L) + 1)
    kmax = py.max(0, math.ceil(X[d].ub() / L) - 1)

    for k in range(kmin, kmax + 1):
      a = k * L
      if d == 0:
        DefaultFigure.draw_line(
          [[a, X[1].lb()], [a, X[1].ub()]],
          Color.light_gray()
        )
      else:
        DefaultFigure.draw_line(
          [[X[0].lb(), a], [X[0].ub(), a]],
          Color.light_gray()
        )

  for v in (Vector([0, 0]), Vector([-8, 8])):
    DefaultFigure.draw_box(
      IntervalVector(L * v) + IntervalVector([[0, L], [0, L]]),
      [Color.dark_green(), Color.green(0.1)]
    )
# [E-q4-end]


# [E-q5-beg]
def f(i):
  x = VectorVar(2)
  return AnalyticFunction([x], mod(x[i]+L_eps/2,L))

c_verti = CtcInverse(f(0), [0,L_eps])
c_horiz = CtcInverse(f(1), [0,L_eps])

your_contractor = c_verti | c_horiz
# [E-q5-end]
ctc_map = your_contractor


# [E-q6-beg]
fig_ctcmap = Figure2D("Testing tile-map contractor", GraphicOutput.VIBES)
fig_ctcmap.set_axes(axis(0,[-1,1]), axis(1,[-1,1])).auto_scale()
fig_ctcmap.pave([[-1,1],[-1,1]], your_contractor, 1e-2)
# [E-q6-end]


# [E-q7-beg]
# Robot parameters
R1 = 0.033
R2 = 0.16
R3 = 8e-2
x0 = Vector([L/2, L/2, 0.018, 0.0]) # initial state

traj_spd = R1 * (u[1].derivative() + u[0].derivative()) / 2
traj_hdg = (R1 * (u[1].derivative() - u[0].derivative()) / R2).primitive() + x0[2]

# Better: use the directly measured heading to avoid odometric drift.
traj_hdg = continuous_traj(pos[2])
# [E-q7-end]


# [E-q8-beg]
tdomain = create_tdomain(u.tdomain(), 5e-2) # last argument is time discretization

tube_hdg = SlicedTube(tdomain, traj_hdg)
tube_hdg.inflate(2e-3)

tube_spd = SlicedTube(tdomain, traj_spd)
tube_spd.inflate(9e-3)
# [E-q8-end]


# [E-q9-beg]
# Computing tube of velocities
v_hdg,v_spd = ScalarVar(),ScalarVar()
f_evol = AnalyticFunction([v_hdg,v_spd], [
  v_spd*cos(v_hdg),
  v_spd*sin(v_hdg)
])

tube_v12 = f_evol.tube_eval(tube_hdg,tube_spd)
# [E-q9-end]


# [E-q10-beg]
# Computing initial tube of positions
ix0 = IntervalVector(x0.subvector(0,1))
ix0.inflate(L/3)

tube_x12 = tube_v12.primitive(ix0)
# [E-q10-end]


# [E-q11-beg]
X = IntervalVector([[-5,2],[-2,5]])
DefaultFigure.set_window_properties([50,50],[1000,1000])
DefaultFigure.set_axes(axis(0,X[0]),axis(1,X[1])).auto_scale()

draw_map(X,L)
DefaultFigure.draw_tube(tube_x12, [Color.light_gray(),Color.light_gray()])
# [E-q11-end]


# [E-q12-beg]
ctc_deriv = CtcDeriv()
# ctc_map is already defined from E.5
# [E-q12-end]


# [E-q13-beg]
def contractors_list(tube_x12):
  for ti in T:
    tj = ti - 0.01
    pi = tube_x12(tj)
    hdgi = tube_hdg(tj)

    bi = IntervalVector([
      pi[0]-R3*cos(hdgi),
      pi[1]-R3*sin(hdgi)
    ])

    bi = ctc_map.contract(bi)

    pi[0] &= bi[0]+R3*cos(hdgi)
    pi[1] &= bi[1]+R3*sin(hdgi)

    tube_x12.set(pi, tj)

  ctc_deriv.contract(tube_x12, tube_v12)
  return tube_x12

tube_x12 = fixpoint(contractors_list, tube_x12)
# [E-q13-end]


# [E-q14-beg]
DefaultFigure.draw_tube(tube_x12, ColorMap.blue_tube())
draw_map(X,L) # drawing again the map over the tube
# [E-q14-end]


# [E-q15-beg]
def draw_estim(x, T, R3, traj_hdg, traj_col, detec_col):
  DefaultFigure.draw_trajectory(x, traj_col)

  for ti in T:
    pi = cart_prod(x(ti).subvector(0,1), traj_hdg(ti))
    bi = Vector([
      pi[0] + R3*math.cos(pi[2]+PI),
      pi[1] + R3*math.sin(pi[2]+PI)
    ])

    DefaultFigure.draw_tank(pi.subvector(0,2)+[0,0,PI], 0.15, detec_col)
    DefaultFigure.draw_circle(bi, 7e-3, detec_col)

draw_estim(tube_x12.mid(), T, R3, traj_hdg, Color.white(), Color.dark_blue())
# [E-q15-end]


# [E-q16-beg]
DefaultFigure.draw_box(
  IntervalVector([[-2.4, -2.1], [2.4, 2.7]]),
  [Color.white(), Color.blue(0.1)]
)
# [E-q16-end]