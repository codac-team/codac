from codac import *

# As an example, defining a separator as the complementary of another one
class MySep(Sep):

  def __init__(self, S_):
    Sep.__init__(self, S_.size())
    self.S = S_

  def separate(self, x):
    xs = self.S.separate(x) # separates x into [xs.innter,xs.outer]
    return BoxPair(xs.outer,xs.inner) # inner/outer boxes are permuted

# Trying this custom separator
x = VectorVar(2)
f = AnalyticFunction([x], vec(sqrt(sqr(x[0])+sqr(x[1]))))
s = SepInverse(f, [[2,4]])
pave([[-5,5],[-5,5]], MySep(s), 0.1)