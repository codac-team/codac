from codac import *

x = VectorVar(2)
f = AnalyticFunction([x], vec(sqrt(sqr(x[0])+sqr(x[1]))))
c = CtcInverse(f, [[2.2,2.4]])

# As an example, defining a custom contractor (does nothing special here)
class MyCtc(Ctc):

  def __init__(self, C_):
    Ctc.__init__(self, C_.size())
    self.C = C_

  def contract(self, x):
    self.C.contract(x)
    
# Trying this custom contractor
a = MyCtc(c) | CtcWrapper([[-1.5,1.5],[-0.25,0.25]])
pave([[-5,5],[-5,5]], a, 0.1)