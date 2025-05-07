# Example from the publication:
# https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

from codac import *

x = VectorVar(3)
f = AnalyticFunction([x], [
  -(x[2]^2)+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
  2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
])

ctc = CtcInverse(f, [0,0])
draw_while_paving([[0,2],[2,4],[0,10]], ctc, 0.004)