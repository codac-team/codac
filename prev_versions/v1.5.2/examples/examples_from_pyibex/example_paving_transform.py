# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------

from codac import *
from codac.unsupported import *

x0 = IntervalVector(2, [-5, 5])
sep = SepFwdBwd(Function("x[2]", "x[0]^2 + x[1]^2"), Interval(-oo, 4))
#P = SepPaving(x0, sep, 0.1)

f = Function("x", 'y', '(0.5*x, 2*y)')
sep2 = SepInverse(sep, f)

beginDrawing()

fig = VIBesFig('test 1')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x0)
SIVIA(x0,sep,0.1)

fig = VIBesFig('test 2')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x0)
SIVIA(x0,sep2,0.1)

endDrawing()