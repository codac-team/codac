from codac4matlab._core import *
import sys

def codac_error(message):
  print(f'''
=============================================================================
The following Codac assertion failed:

{message}

You need help? Submit an issue on: https://github.com/codac-team/codac/issues
=============================================================================
  ''')
  raise ValueError("")


class AnalyticFunction:

  def __init__(self, args, e=None):
    if e:
      if isinstance(e, (int,float,Interval,ScalarVar,ScalarExpr)):
        self.f = AnalyticFunction_Scalar(args,ScalarExpr(e))
      elif isinstance(e, (Vector,IntervalVector,VectorVar,VectorExpr)):
        self.f = AnalyticFunction_Vector(args,VectorExpr(e))
      elif isinstance(e, (Matrix,IntervalMatrix,MatrixVar,MatrixExpr)):
        self.f = AnalyticFunction_Matrix(args,MatrixExpr(e))
      elif isinstance(e, list):
        lst=[]
        for e_i in e:
          if isinstance(e_i, (int,float,Interval,ScalarVar,ScalarExpr)):
            lst.append(ScalarExpr(e_i))
          else:
            codac_error("AnalyticFunction: invalid vectorial expression")
        self.f = AnalyticFunction_Vector(args,lst)
      else:
        codac_error("AnalyticFunction: can only build functions from scalar or vector expressions")
    else:
      if isinstance(args, (AnalyticFunction_Scalar,AnalyticFunction_Vector,AnalyticFunction_Matrix)):
        self.f = args
      else:
        codac_error("AnalyticFunction: invalid function argument")

  def input_size(self):
    return self.f.input_size()

  def output_size(self):
    return self.f.output_size()

  def real_eval(self,*args):
    return self.f.real_eval(*args)

  def eval(self,m,*args):
    return self.f.eval(m,*args)

  def eval(self,*args):
    return self.f.eval(*args)

  def diff(self,*args):
    return self.f.diff(*args)

  def __call__(self,*args):
    lst=[]
    for arg in args:
      if isinstance(arg, (int,float,Interval,ScalarVar,ScalarExpr)):
        lst.append(ScalarExpr(arg))
      elif isinstance(arg, (Vector,IntervalVector,VectorVar,VectorExpr)):
        lst.append(VectorExpr(arg))
      elif isinstance(arg, (Matrix,IntervalMatrix,MatrixVar,MatrixExpr)):
        lst.append(MatrixExpr(arg))
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


class Sep(SepBase):

  def copy(self):
    return super().copy()


class CtcInverse(Ctc):

  def __init__(self, f, y, with_centered_form = True):
    Ctc.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      if not (isinstance(y, (int,float,Interval))
              or (isinstance(y, list) and len(y) > 0 and len(y) <= 2 and isinstance(y[0], (int,float)))):
        codac_error("CtcInverse: inverse argument 'y' should be a scalar type (float,Interval)")
      self.c = CtcInverse_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      if not isinstance(y, (Vector,IntervalVector,list,CtcIntervalVector)):
        codac_error("CtcInverse: inverse argument 'y' should be a vector type (Vector,IntervalVector,CtcIntervalVector)")
      if isinstance(y, CtcIntervalVector):
        self.c = CtcInverse_IntervalVector(f,y,with_centered_form)
      else:
        self.c = CtcInverse_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      codac_error("CtcInverse: can only build CtcInverse from scalar or vector functions")

  def contract(self,x):
    return self.c.contract(x)

  def copy(self):
    return self.c.copy()

  def function(self):
    return self.c.function()


class CtcInverseNotIn(Ctc):

  def __init__(self, f, y, with_centered_form = True):
    Ctc.__init__(self, f.input_size())
    if isinstance(f.f, AnalyticFunction_Scalar):
      if not (isinstance(y, (int,float,Interval))
              or (isinstance(y, list) and len(y) > 0 and len(y) <= 2 and isinstance(y[0], (int,float)))):
        codac_error("CtcInverseNotIn: inverse argument 'y' should be a scalar type (float,Interval)")
      self.c = CtcInverseNotIn_Interval(f.f,Interval(y),with_centered_form)
    elif isinstance(f.f, AnalyticFunction_Vector):
      if not isinstance(y, (Vector,IntervalVector,list)):
        codac_error("CtcInverseNotIn: inverse argument 'y' should be a vector type (Vector,IntervalVector)")
      self.c = CtcInverseNotIn_IntervalVector(f.f,IntervalVector(y),with_centered_form)
    else:
      codac_error("CtcInverseNotIn: can only build CtcInverseNotIn from scalar or vector functions")

  def contract(self,x):
    return self.c.contract(x)

  def copy(self):
    return self.c.copy()


class Approx:

  def __init__(self, x, eps = sys.float_info.epsilon*10):
    if isinstance(x, (int,float)):
      self.a = Approx_double(x,eps)
    elif isinstance(x, (Interval)):
      self.a = Approx_Interval(x,eps)
    elif isinstance(x, (Vector)):
      self.a = Approx_Vector(x,eps)
    elif isinstance(x, (IntervalVector)):
      self.a = Approx_IntervalVector(x,eps)
    elif isinstance(x, (Matrix)):
      self.a = Approx_Matrix(x,eps)
    elif isinstance(x, (IntervalMatrix)):
      self.a = Approx_IntervalMatrix(x,eps)
    elif isinstance(x, (Segment)):
      self.a = Approx_Segment(x,eps)
    elif isinstance(x, (Polygon,ConvexPolygon)):
      self.a = Approx_Polygon(x,eps)
    else:
      codac_error("Approx: can only build Approx for: \
        double, Interval, [Interval]Vector, Matrix, [Interval]Matrix, Segment, [Convex]Polygon")

  def __eq__(self, x):
    return self.a == x

  def __repr__(self):
    return str(self.a)


def hull(*args):
  # -1: to be defined, 0: IntervalVector, 1: IntervalMatrix
  lst = []
  mode = -1

  for arg in args:
    if isinstance(arg, (Vector)):
      mode = 0
      lst.append(IntervalVector(arg))
    elif isinstance(arg, (IntervalVector)):
      mode = 0
      lst.append(arg)
    elif isinstance(arg, (Matrix)):
      mode = 1
      lst.append(IntervalMatrix(arg))
    elif isinstance(arg, (IntervalMatrix)):
      mode = 1
      lst.append(arg)

  if mode == 0:
    return hull_intervalvector(lst)
  else:
    return hull_intervalmatrix(lst)


def cart_prod(*args):
  # -1: to be defined, 0: vector, 1: intervalvector, 2: ctc, 3: sep
  lst = []
  mode = -1

  for arg in args:

    if isinstance(arg, (int,float,Vector)):
      if mode == -1:
        mode = 0

    elif isinstance(arg, (list,Interval,IntervalVector)):
      if mode != 2 and mode != 3:
        mode = 1

    elif isinstance(arg, Ctc):
      mode = 2

    elif isinstance(arg, (Sep,SepBase)):
      mode = 3

    else:
      codac_error("cart_prod: invalid input arguments (a/" + str(mode) + ")")

  for arg in args:

    if isinstance(arg, (int,float)):
      if mode == 0:
        lst.append(Vector([arg]))
      elif mode == 1:
        lst.append(IntervalVector(Vector([arg])))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector([arg])))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector([arg])))
      else:
        codac_error("cart_prod: invalid input arguments (b/" + str(mode) + ")")

    elif isinstance(arg, (Vector)):
      if mode == 0:
        lst.append(arg)
      elif mode == 1:
        lst.append(IntervalVector(arg))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector(arg)))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector(arg)))
      else:
        codac_error("cart_prod: invalid input arguments (c/" + str(mode) + ")")

    elif isinstance(arg, (Interval)):
      if mode == 1:
        lst.append(IntervalVector([arg]))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector([arg])))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector([arg])))
      else:
        codac_error("cart_prod: invalid input arguments (d/" + str(mode) + ")")

    elif isinstance(arg, (list,IntervalVector)):
      if mode == 1:
        lst.append(IntervalVector(arg))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector(arg)))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector(arg)))
      else:
        codac_error("cart_prod: invalid input arguments (e/" + str(mode) + ")")

    elif isinstance(arg, Ctc):
      if mode != 2:
        codac_error("cart_prod: invalid input arguments (f/" + str(mode) + ")")
      lst.append(arg)

    elif isinstance(arg, (Sep,SepBase)):
      if mode != 3:
        codac_error("cart_prod: invalid input arguments (g/" + str(mode) + ")")
      lst.append(arg)

    else:
      mode = -2 # will generate an error

  if mode == 0:
    return cart_prod_vector(lst)
  elif mode == 1:
    return cart_prod_intervalvector(lst)
  elif mode == 2:
    return cart_prod_ctc(lst)
  elif mode == 3:
    return cart_prod_sep(lst)
  else:
    codac_error("cart_prod: invalid input arguments (h/" + str(mode) + ")")


class AnalyticTraj:

  def __init__(self, f, t):
    if isinstance(f, AnalyticFunction):
      self.__init__(f.f,t)
    elif isinstance(f, AnalyticFunction_Scalar):
      self.traj = AnalyticTraj_Scalar(f,t)
    elif isinstance(f, AnalyticFunction_Vector):
      self.traj = AnalyticTraj_Vector(f,t)
    else:
      codac_error("AnalyticTraj: can only build this trajectory from an AnalyticFunction_[Scalar/Vector]")

  # Methods from TrajBase:

  def size(self):
    return self.traj.size()

  def is_empty(self):
    return self.traj.is_empty()

  def tdomain(self):
    return self.traj.tdomain()

  def truncate_tdomain(self, new_tdomain):
    return self.traj.truncate_tdomain(new_tdomain)
    
  def codomain(self):
    return self.traj.codomain()
    
  def __call__(self, t):
    return self.traj(t)
    
  def nan_value(self):
    return self.traj.nan_value()
    
  def sampled(self, dt):
    return self.traj.sampled(dt)
    
  def primitive(self, y0, t):
    return self.traj.primitive(y0, t)
    
  def as_function(self):
    return AnalyticFunction(self.traj.as_function())
    
  # Methods from AnalyticTraj:
  #   none