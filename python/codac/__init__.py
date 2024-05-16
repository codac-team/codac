from codac.core import *
from .version import __version__


class AnalyticFunction:

  def __init__(self, args, e):

    if e.is_scalar():
      self.f = ScalarAnalyticFunction(args,e)
    else:
      self.f = VectorAnalyticFunction(args,e)

  def eval(self,x):

    return self.f.eval(x)
  
  def __repr__(self):

    return str(self.f)