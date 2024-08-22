from codac._core import *
from sys import float_info


def codac_error(message):
  print(f'''
============================================================================
The following Codac assertion failed:

{message}

If you need help, submit an issue on https://codac.io/issues
=============================================================================
  ''')
  raise ValueError("")


class AnalyticFunction:

  def __init__(self, args, e):
    if isinstance(e, (int,float,Interval,ScalarVar,ScalarExpr)):
      self.f = AnalyticFunction_Scalar(args,e)
    elif isinstance(e, (Vector,IntervalVector,VectorVar,VectorExpr)):
      self.f = AnalyticFunction_Vector(args,e)
    else:
      codac_error("AnalyticFunction: can only build functions from scalar or vector expressions")

  def input_size(self):
    return self.f.input_size()

  def eval(self,*args):
    return self.f.eval(*args)

  def natural_eval(self,*args):
    return self.f.natural_eval(*args)

  def centered_eval(self,*args):
    return self.f.centered_eval(*args)

  def diff(self,*args):
    return self.f.diff(*args)

  def __call__(self,*args):
    lst=[]
    for arg in args:
      if isinstance(arg, (int,float,Interval,ScalarVar,ScalarExpr)):
        lst.append(ScalarExpr(arg).raw_copy())
      elif isinstance(arg, (Vector,IntervalVector,VectorVar,VectorExpr)):
        lst.append(VectorExpr(arg).raw_copy())
      else:
        codac_error("AnalyticFunction: invalid input arguments")
    return self.f(lst)

  def __repr__(self):
    return str(self.f)


class Ctc(CtcIntervalVector):

  def __init__(self, n):
    CtcIntervalVector.__init__(self,n)

  def copy(self):
    return super().copy()


class Sep(Sep_):

  def copy(self):
    return super().copy()


class CtcInverse(Ctc):

  def __init__(self, f, y, with_centered_form = True):
    Ctc.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.c = CtcInverse_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.c = CtcInverse_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      codac_error("CtcInverse: can only build CtcInverse from scalar or vector functions")

  def contract(self,x):
    self.c.contract(x)

  def copy(self):
    return self.c.copy()


class CtcInverseNotIn(Ctc):

  def __init__(self, f, y, with_centered_form = True):
    Ctc.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.c = CtcInverseNotIn_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.c = CtcInverseNotIn_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      codac_error("CtcInverseNotIn: can only build CtcInverseNotIn from scalar or vector functions")

  def contract(self,x):
    self.c.contract(x)

  def copy(self):
    return self.c.copy()


class SepInverse(Sep):

  def __init__(self, f, y, with_centered_form = True):
    Sep.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      self.s = SepInverse_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      self.s = SepInverse_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      codac_error("SepInverse: can only build SepInverse from scalar or vector functions")

  def separate(self,x):
    return self.s.separate(x)

  def copy(self):
    return super().copy()


class Approx:

  def __init__(self, x, eps = float_info.epsilon*10):
    if isinstance(x, (float,Interval)):
      self.a = Approx_Interval(x,eps)
    elif isinstance(x, (IntervalVector)):
      self.a = Approx_IntervalVector(x,eps)
    else:
      codac_error("Approx: can only build Approx from Interval or IntervalVector")

  def __eq__(self, x):
    return self.a == x

  def __repr__(self):
    return str(self.a)


def cart_prod(*args):
  # -1: to be defined, 0: domains, 1: ctc, 2: sep
  lst=[]
  mode = -1

  mode_str = {}
  mode_str[-2] = "error"
  mode_str[-1] = "undefined"
  mode_str[0] = "domain"
  mode_str[1] = "contractor"
  mode_str[2] = "separator"

  for arg in args:

    if isinstance(arg, (float,Interval)):
      if mode != -1 and mode != 0:
        codac_error("cart_prod: invalid input arguments, was expecting a " + mode_str[mode] + ", got a scalar domain")
      mode = 0
      lst.append(IntervalVector(1,Interval(arg)))

    elif isinstance(arg, (list,Vector,IntervalVector)):
      if mode != -1 and mode != 0:
        codac_error("cart_prod: invalid input arguments, was expecting a " + mode_str[mode] + ", got a vector domain")
      mode = 0
      lst.append(IntervalVector(arg))

    elif isinstance(arg, Ctc):
      if mode != -1 and mode != 1:
        codac_error("cart_prod: invalid input arguments, was expecting a " + mode_str[mode] + ", got a contractor")
      mode = 1
      lst.append(arg)

    elif isinstance(arg, Sep):
      if mode != -1 and mode != 2:
        codac_error("cart_prod: invalid input arguments, was expecting a " + mode_str[mode] + ", got a separator")
      mode = 2
      lst.append(arg)

    else:
      mode = -2 # will generate an error

  if mode == 0:
    return cart_prod_boxes(lst)
  elif mode == 1:
    return cart_prod_ctc(lst)
  elif mode == 2:
    return cart_prod_sep(lst)
  else:
    codac_error("cart_prod: invalid input arguments")