from codac.core import *
from .version import __version__

class AnalyticFunction:

  def __init__(self, args, e):
    if isinstance(e, (ScalarVar,ScalarExpr)):
      self.f = AnalyticFunction_Scalar(args,e)
    elif isinstance(e, (VectorVar,VectorExpr)):
      self.f = AnalyticFunction_Vector(args,e)
    else:
      raise ValueError("Can only build functions from scalar or vector expressions")

  def input_size(self):
    return self.f.input_size()

  def eval(self,*args):
    return self.f.eval(*args)

  def __call__(self,*args):
    lst=[]
    for arg in args:
      if isinstance(arg, (ScalarVar,ScalarExpr,Interval)):
        lst.append(ScalarExpr(arg).raw_copy())
      elif isinstance(arg, (VectorVar,VectorExpr,IntervalVector)):
        lst.append(VectorExpr(arg).raw_copy())
      else:
        raise ValueError("Invalid input argument: ", arg)
    return self.f(lst)

  def __repr__(self):
    return str(self.f)

class CtcInverse(CtcIntervalVector):

  def __init__(self, f, y):
    CtcIntervalVector.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.c = CtcInverse_Interval(f.f,Interval(y))
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.c = CtcInverse_IntervalVector(f.f,IntervalVector(y))
    else:
      raise ValueError("Can only build CtcInverse from scalar or vector functions")

  def contract(self,x):
    self.c.contract(x)
    return x