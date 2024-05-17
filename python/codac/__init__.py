from codac.core import *
from .version import __version__

class AnalyticFunction:

  def __init__(self, args, e):

    if isinstance(e, (ScalarVar,ScalarExpr)):
      self.f = ScalarAnalyticFunction(args,e)
    elif isinstance(e, (VectorVar,VectorExpr)):
      self.f = VectorAnalyticFunction(args,e)
    else:
      print("Can only build functions from scalar or vector expressions")

  def eval(self,*args):

    return self.f.eval(*args)

  def __repr__(self):

    return str(self.f)