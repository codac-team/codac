from pyibex import *
from pyibex.paving import *
from pyibex.sepvisitor import *
from vibes import vibes

X0 = IntervalVector(2, [-5, 5])
sep = SepFwdBwd(Function("x[2]", "x[0]^2 + x[1]^2"), Interval(-oo, 4))
P = SepPaving(X0, sep, 0.1)

vibes.beginDrawing()
vibes.newFigure("test")
P.visit(SepToVibes("test"))

vibes.newFigure("test2")
f = Function("x", 'y', '(0.5*x, 2*y)')
sep2 = SepInverse(P, f)
P2 = SepPaving(X0, sep2, 0.1)
P2.visit(SepToVibes("test2"))

