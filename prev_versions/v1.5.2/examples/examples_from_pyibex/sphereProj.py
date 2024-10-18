# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------
# Simple example of the projection of a sphere

from codac import *
from codac.unsupported import *

f = Function("x[2]","y","x[0]^2+x[1]^2+y^2")
S1=SepFwdBwd(f,Interval(4,9))
Y=IntervalVector([[-1,1]])
#S2=SepCtcPairProj(S1,Y,0.01) 
S2=SepProj(S1,Y,0.01)
X0 =IntervalVector([[-10,10],[-10,10]]);

beginDrawing()
fig = VIBesFig('Proj')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(X0)
SIVIA(X0,S2,0.1)
endDrawing()