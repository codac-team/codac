# Codac - Examples
# Contractors on tubes: temporal contractors
# Comparison between the Lohner & Picard temporal contractors
# ----------------------------------------------------------------------------
# Auguste Bourgois, 2020


from codac import *
import sys # only for checking if this example still works


# =========== INITIALIZATION ===========

dt = 0.1 # time step of the tubes
tdomain = Interval(0, 5) # time domain of the tubes

x = TubeVector(tdomain, dt, 1) # 1d tube vector
x.set(IntervalVector(1, Interval(0.9, 1.1)), 0.) # initial condition
y = TubeVector(x) # copy of x

beginDrawing()


# =========== COMPARISON OF CONTRACTORS ===========

f = Function("x", "-sin(x)") # xdot = -sin(x)

# Picard contractor

ctc_picard = CtcPicard(f)
ctc_picard.contract(x)

fig1 = VIBesFigTube("Picard integration")
fig1.set_properties(100, 100, 800, 400)
fig1.add_tube(x[0], "x")

# Lohner contractor

ctc_lohner = CtcLohner(f)
ctc_lohner.contract(y)

fig2 = VIBesFigTube("Lohner integration")
fig2.set_properties(100, 550, 800, 400)
fig2.add_tube(y[0], "y")


# =========== COMPUTING TRAJECTORIES AS ILLUSTRATION ===========

v_traj = [Trajectory for _ in range(11)]

for i in range(0,len(v_traj)):
  x0 = 0.9 + 0.199 * (i / 10.) # initial conditions in [x0]=[0.9,1.1]
  v_traj[i] = Trajectory(tdomain, TFunction(str(x0) + "*2.*atan(exp(-t)*tan(0.5))"))
  fig1.add_trajectory(v_traj[i], "truth" + str(i))
  fig2.add_trajectory(v_traj[i], "truth" + str(i))

fig1.show(True)
fig2.show(True)
fig1.axis_limits(fig2.view_box())

endDrawing()


# =========== ENDING ===========

# Checking if this example still works:

for traj_i in v_traj:
  if x[0].contains(traj_i) != BoolInterval.YES:
    sys.exit(0)
  if y[0].contains(traj_i) != BoolInterval.YES:
    sys.exit(0)

# Checking if this example still works:
sys.exit(0 if fabs(x[0].volume() - 13.9148) < 1e-2 and fabs(y[0].volume() - 0.43186) < 1e-2 else 1)