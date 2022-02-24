"""
Simple example of the projection of a sphere
"""

from pyibex import *
from vibes import vibes
f = Function("x1","x2","y", "x1^2+x2^2+y^2")
S1=SepFwdBwd(f,Interval(4,9))
Y=IntervalVector([[-1,1]])
#S2=SepCtcPairProj(S1,Y,0.01) 
S2=SepProj(S1,Y,0.01)
X0 =IntervalVector([[-10,10],[-10,10]]);
vibes.beginDrawing()
vibes.newFigure('Proj')
vibes.setFigureSize(500,500)
pySIVIA(X0,S2,0.1, use_patch=True)
vibes.axisEqual()
