# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------
# Custom contractor
# Writes a custom contractor for the equation
# (x-a)^2 + (y-b)^2 \in r^2

from codac import *

class MyCtc(Ctc):

  def __init__(self, a, b, r):
    Ctc.__init__(self, 2)
    self.a = Interval(a)
    self.b = Interval(b)
    self.r = r

  def contract(self, x):
    i1 = x[0] - Interval(self.a)
    i2 = x[1] - Interval(self.b)

    i3 = sqr(i1)
    i4 = sqr(i2)

    i5 = i3 + i4
    i5 &= sqr(self.r)

    bwd_add(i5, i3, i4)
    bwd_sqr(i4, i2)
    bwd_sqr(i3, i1)

    bwd_sub(i1, x[0], Interval(self.a))
    bwd_sub(i2, x[1], Interval(self.b))

    return x


ctc = MyCtc(1, 2, Interval(1, 2))
x = IntervalVector(2, [-8, 8])

beginDrawing()
fig = VIBesFig('Result')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x)
SIVIA(x, ctc, 0.3)
endDrawing()