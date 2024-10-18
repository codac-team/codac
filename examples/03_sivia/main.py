from codac import *

x = VectorVar(2)
f = AnalyticFunction([x], sqr(x[0])*sin(sqr(x[0])+sqr(x[1]))-sqr(x[1]))
p = sivia([[-5,5],[-4,4]], f, [0,oo], 1e-2)
DefaultView.draw_paving(p)