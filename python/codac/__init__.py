from codac.core import *
from .version import __version__
from sys import float_info


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

  def __init__(self, f, y, with_centered_form = True):
    CtcIntervalVector.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.c = CtcInverse_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.c = CtcInverse_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      raise ValueError("Can only build CtcInverse from scalar or vector functions")

  def contract(self,x):
    self.c.contract(x)
    return x


class SepInverse(Sep):

  def __init__(self, f, y, with_centered_form = True):
    Sep.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.s = SepInverse_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.s = SepInverse_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      raise ValueError("Can only build SepInverse from scalar or vector functions")

  def separate(self,x):
    return self.s.separate(x)


class Approx:

  def __init__(self, x, eps = float_info.epsilon*10):
    if isinstance(x, (float,Interval)):
      self.a = Approx_Interval(x,eps)
    elif isinstance(x, (IntervalVector)):
      self.a = Approx_IntervalVector(x,eps)
    else:
      raise ValueError("Can only build Approx from Interval or IntervalVector")

  def __eq__(self, x):
    return self.a == x

  def __repr__(self):
    return str(self.a)


def cart_prod(*args):
  lst=[]
  for arg in args:
    if isinstance(arg, (float,Interval)):
      lst.append(IntervalVector(1,Interval(arg)))
    else:
      lst.append(IntervalVector(arg))
  return cart_prod_list(lst)