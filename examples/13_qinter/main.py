# Example from:
#   Robust Localisation Using Separators
#   Luc Jaulin and Benoı̂t Desrochers

from codac import *

def create_sep(b, d):

  DefaultFigure.draw_circle(b, 0.1, StyleProperties([Color.white(),Color.white()], "z:5"))
  DefaultFigure.draw_ring(b, d, StyleProperties(Color.dark_gray(), "z:4"))

  v = VectorVar(2)
  f = AnalyticFunction([v], sqr(v[0]-b[0]) + sqr(v[1]-b[1]))
  return SepInverse(f, sqr(d))


s1 = create_sep(Vector([1,3]), Interval(1,2))
s2 = create_sep(Vector([3,1]), Interval(2,3))
s3 = create_sep(Vector([-1,-1]), Interval(3,4))

DefaultFigure.set_axes(axis(0, Interval(-6,6)), axis(1, Interval(-6,6)))

q = 2
DefaultFigure.pave(
  [[-6,6],[-6,6]],
  SepQInter(q, s1, s2, s3),
  5e-2
)