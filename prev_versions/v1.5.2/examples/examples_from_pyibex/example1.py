# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------
# Simple example of a separator

from codac import *

f = Function('x', 'y', 'x*cos(x-y)+y')
sep = SepFwdBwd(f, CmpOp.LEQ)
x = IntervalVector(2, [-10, 10])

beginDrawing()
fig = VIBesFig('Example')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x)
SIVIA(x, sep, 0.2)
endDrawing()