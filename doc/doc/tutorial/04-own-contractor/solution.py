from pyibex import *
from tubex_lib import *
import math


# D.1 / D.2

class MyCtc(Ctc):

  def __init__(self, M_):
    Ctc.__init__(self, 2) # the contractor acts on 2d boxes
    self.M = M_           # attribute needed later on for the contraction

  def contract(self, a):

    # Insert contraction formula here (question D.2)

    a_ = IntervalVector(2,Interval.EMPTY_SET)
    for Mj in M:
      a_ |= Mj & a

    a = a_
    return a


# D.3

a = [ IntervalVector([[1.25,3],[1.6,2.75]]),
      IntervalVector([[2.,3.5],[0.6,1.2]]),
      IntervalVector([[1.1,3.25],[0.2,1.4]]) ]

M = [ IntervalVector([1.5,2.5]),
      IntervalVector([3.,1.]),
      IntervalVector([2.,2.]),
      IntervalVector([2.5,3.]),
      IntervalVector([3.5,2.]),
      IntervalVector([4.,1.]),
      IntervalVector([1.5,0.5]) ]

for Mi in M:
  Mi.inflate(0.05)

ctc_constell = MyCtc(M)
cn = ContractorNetwork()

cn.add(ctc_constell, [a[0]])
cn.add(ctc_constell, [a[1]])
cn.add(ctc_constell, [a[2]])

cn.contract()

print(a[0])
print(a[1])
print(a[2])


# D.4

x_truth = (2,1,math.pi/6) # actual state vector (pose)


# D.5

beginDrawing()

fig_map = VIBesFigMap("Map")
fig_map.set_properties(100,100,500,500)
fig_map.draw_vehicle(x_truth,0.5)
for Mi in M:
  fig_map.draw_box(Mi, "red[orange]")


# D.6

v_obs = DataLoader.generate_static_observations(x_truth, M, False)

# Adding uncertainties on the measurements
for obs in v_obs:
  obs[0].inflate(0.02) # range
  obs[1].inflate(0.02) # bearing

x = IntervalVector(3)
x[2] = Interval(x_truth[2])

for obs in v_obs:
  fig_map.draw_pie(x_truth[0],x_truth[1],Interval(0)|obs[0],x_truth[2]+obs[1],"lightGray")
  fig_map.draw_pie(x_truth[0],x_truth[1],obs[0],x_truth[2]+obs[1],"darkGray")

  fig_map.draw_pie(x_truth[0], x_truth[1], obs[0], x[2]+obs[1])
# D.7

# Define contractors
ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # a+b=c
ctc_minus = CtcFunction(Function("a", "b", "c", "a-b-c")) # a-b=c
# ctc_constell already created

if False:

  cn = ContractorNetwork()

  for i in range(0,len(v_obs)): # for each measurement

    # Define intermediate variables
    theta = Interval()
    d = IntervalVector(2)

    # Add contractors and related domains
    cn.add(ctc_plus, [v_obs[i][1], x[2], theta])
    cn.add(ctc_minus, [M[i][0], x[0], d[0]])
    cn.add(ctc_minus, [M[i][1], x[1], d[1]])
    cn.add(ctc.polar, [d, v_obs[i][0], theta])

    # Note: v_obs[i] is a 2d vector [y^i]
    # Note: M[i] is a 2d box of M

  cn.contract()


  # D.8

  fig_map.draw_box(x.subvector(0,1))


# D.9

# Association set (possible identities)
m = [IntervalVector(2) for _ in v_obs] # unknown associations for each observation


# D.10

cn = ContractorNetwork()

for i in range(0,len(v_obs)): # for each measurement

  # Define intermediate variables
  theta = Interval()
  d = IntervalVector(2)

  # Add contractors and related domains
  cn.add(ctc_plus, [v_obs[i][1], x[2], theta])
  cn.add(ctc_minus, [m[i][0], x[0], d[0]])
  cn.add(ctc_minus, [m[i][1], x[1], d[1]])
  cn.add(ctc.polar, [d, v_obs[i][0], theta])

  cn.add(ctc_constell, [m[i]])

cn.contract()

fig_map.draw_box(x.subvector(0,1))


# D.11

for mi in m:
  if mi.max_diam() <= 0.10001: # if identified
    fig_map.draw_circle(mi[0].mid(), mi[1].mid(), 0.02, "blue[blue]")



fig_map.axis_limits(fig_map.view_box(), True, 0.1)

endDrawing()