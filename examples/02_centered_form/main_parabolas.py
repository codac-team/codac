from codac import *

u = ScalarVar()
v = ScalarVar()
a = VectorVar(4)
a0 = VectorVar(3); a1 = VectorVar(3); a2 = VectorVar(3)
b0 = VectorVar(3); b1 = VectorVar(3); b2 = VectorVar(3)

b = AnalyticFunction([u,a0,a1,a2], sqr(1-u)*a0+2*u*(1-u)*a1+sqr(u)*a2)
f = AnalyticFunction([u,v,a0,a1,a2,b0,b1,b2], (1-v)*b(u,a0,a1,a2)+v*b(u,b0,b1,b2))

h = AnalyticFunction([a],
   f(a[0],a[1],
     IntervalVector([[0],[0],[0]]),IntervalVector([[1],[0],[1]]),IntervalVector([[2],[0],[0]]),
     IntervalVector([[0],[2],[0]]),IntervalVector([[1],[2],[1]]),IntervalVector([[2],[2],[0]]))
  -f(a[2],a[3],
     IntervalVector([[0],[0],[0.55]]),IntervalVector([[0],[1],[-0.45]]),IntervalVector([[0],[2],[0.55]]),
     IntervalVector([[2],[0],[0.55]]),IntervalVector([[2],[1],[-0.45]]),IntervalVector([[2],[2],[0.55]]))
)

ctc = CtcInverse(h, [[0],[0],[0]])
draw_while_paving([[0,1],[0,1],[0,1],[0,1]], ctc, 0.001)