from pyibex.thickset import *
from pyibex import Interval, IntervalVector
from vibes import vibes


def test_inBox(X):
  B = IntervalVector(2, [-5, 5])
  if X.is_disjoint(B): return OUT
  elif X.is_subset(B): return IN
  return UNK

def test_inBox2(X):
  B = IntervalVector(2, [-3, 3])
  if X.is_disjoint(B): return OUT
  elif X.is_subset(B): return IN
  return UNK





X0 = IntervalVector(2, [-10,10])
test = ThickDisk(Interval(0), Interval(0), Interval(0,4), Interval(0,4))
P = ThickPaving(X0, lambda x: opSupSub(test_inBox2(x), test_inBox(x)), 0.05, opInter)
# test2 = ThickDisk(Interval(1), Interval(1), Interval(0,2), Interval(0,10))
# P.Sivia(test2, 0.1, opAnd)
# T  = ThickTranslateInPaving(P, IntervalVector(2, [1,4]))
# P2 = ThickPaving(X0, T, 0.1, opInter)

vibes.beginDrawing()

P.visit(ToVibes("test_paving"))
vibes.endDrawing()
