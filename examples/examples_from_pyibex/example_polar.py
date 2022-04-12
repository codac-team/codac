# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------

from codac import *
from codac.unsupported import *
import numpy as np

sep1 = SepPolarXY(Interval(4,5), Interval(np.deg2rad(-570), np.deg2rad(-450)))

f1 = Function("x", 'y', '(x + 2; y - 2)')
f2 = Function("x", 'y', '(x - 2; y + 2)')
sep = SepTransform(sep1, f1, f2)

x = IntervalVector([[-10,10],[-10,10]])


beginDrawing()
fig = VIBesFig('SIVIA')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(x)
SIVIA(x, sep1, 0.1)