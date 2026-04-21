#  Codac Python binding - main file
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

from codac._core import *
import sys
import warnings

try:
  import numpy as _np
except ImportError:
  _np = None


def codac_error(message):
  print(f'''
=============================================================================
The following Codac assertion failed:

{message}

You need help? Submit an issue on: https://github.com/codac-team/codac/issues
=============================================================================
  ''')
  raise ValueError("")


_ANALYTIC_FUNCTION_TYPES = (
  AnalyticFunction_Scalar,
  AnalyticFunction_Vector,
  AnalyticFunction_Matrix,
)

_SLICEDTUBE_TYPES = (
  SlicedTube_Interval,
  SlicedTube_IntervalVector,
  SlicedTube_IntervalMatrix,
)

_ANALYTIC_TRAJ_TYPES = (
  AnalyticTraj_Scalar,
  AnalyticTraj_Vector,
  AnalyticTraj_Matrix,
)

_SAMPLED_TRAJ_TYPES = (
  SampledTraj_Scalar,
  SampledTraj_Vector,
  SampledTraj_Matrix,
)

def _is_scalar_expr_like(x):
  return isinstance(x, (int, float, Interval, ScalarVar, ScalarExpr))

def _is_vector_expr_like(x):
  return isinstance(x, (Vector, IntervalVector, VectorVar, VectorExpr))

def _is_matrix_expr_like(x):
  return isinstance(x, (Matrix, IntervalMatrix, MatrixVar, MatrixExpr))


def _to_function_argument_expr(x):
  if _is_scalar_expr_like(x):
    return ScalarExpr(x)
  if _is_vector_expr_like(x):
    return VectorExpr(x)
  if _is_matrix_expr_like(x):
    return MatrixExpr(x)
  codac_error("AnalyticFunction: invalid input arguments")


def AnalyticFunction(args, e=None):
  if e is None:
    if isinstance(args, _ANALYTIC_FUNCTION_TYPES):
      return args
    codac_error("AnalyticFunction: invalid function argument")

  if _is_scalar_expr_like(e):
    return AnalyticFunction_Scalar(args, ScalarExpr(e))

  if _is_vector_expr_like(e):
    return AnalyticFunction_Vector(args, VectorExpr(e))

  if _is_matrix_expr_like(e):
    return AnalyticFunction_Matrix(args, MatrixExpr(e))

  if isinstance(e, list):
    lst = []
    for e_i in e:
      if _is_scalar_expr_like(e_i):
        lst.append(ScalarExpr(e_i))
      else:
        codac_error("AnalyticFunction: invalid vectorial expression")
    return AnalyticFunction_Vector(args, lst)

  codac_error("AnalyticFunction: can only build functions from scalar or vector expressions")


def _make_variadic_analytic_function_call(cpp_call):
  def _call(self, *args):
    if len(args) == 1 and isinstance(args[0], list):
      args = tuple(args[0])

    lst = []
    for arg in args:
      lst.append(_to_function_argument_expr(arg))
    return cpp_call(self, lst)

  return _call


for _cls in _ANALYTIC_FUNCTION_TYPES:
  _cls._codac_python_cpp_call = _cls.__call__
  _cls.__call__ = _make_variadic_analytic_function_call(_cls._codac_python_cpp_call)


class Ctc_IntervalVector(Ctc_IntervalVector_):

  def __init__(self, n):
    Ctc_IntervalVector_.__init__(self, n)

  def copy(self):
    return super().copy()


class Sep(SepBase):

  def copy(self):
    return super().copy()


def CtcInverse(f, y, with_centered_form=True):
  f = AnalyticFunction(f)

  if isinstance(f, AnalyticFunction_Scalar):
    return CtcInverse_Interval(f, y, with_centered_form)

  if isinstance(f, AnalyticFunction_Vector):
    return CtcInverse_IntervalVector(f, y, with_centered_form)

  codac_error("CtcInverse: can only build CtcInverse from scalar or vector functions")


def CtcInverseNotIn(f, y, with_centered_form=True):
  f = AnalyticFunction(f)

  if isinstance(f, AnalyticFunction_Scalar):
    return CtcInverseNotIn_Interval(f, y, with_centered_form)

  if isinstance(f, AnalyticFunction_Vector):
    return CtcInverseNotIn_IntervalVector(f, y, with_centered_form)

  codac_error("CtcInverseNotIn: can only build CtcInverseNotIn from scalar or vector functions")


def Approx(x, eps=sys.float_info.epsilon*10):
  if isinstance(x, (int, float)):
    return Approx_double(x, eps)
  elif isinstance(x, Interval):
    return Approx_Interval(x, eps)
  elif isinstance(x, Vector):
    return Approx_Vector(x, eps)
  elif isinstance(x, IntervalVector):
    return Approx_IntervalVector(x, eps)
  elif isinstance(x, Matrix):
    return Approx_Matrix(x, eps)
  elif isinstance(x, IntervalMatrix):
    return Approx_IntervalMatrix(x, eps)
  elif isinstance(x, Segment):
    return Approx_Segment(x, eps)
  elif isinstance(x, (Polygon, ConvexPolygon)):
    return Approx_Polygon(x, eps)
  elif isinstance(x, tuple) and len(x) == 2 and isinstance(x[0], Interval) and isinstance(x[1], Interval):
    return Approx_pair_Interval(x, eps)
  else:
    codac_error("Approx: invalid input")


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
    elif isinstance(arg, (list)):
      if isinstance(arg[0], (IntervalVector)):
        return hull_intervalvector(arg)
      elif isinstance(arg[0], (IntervalMatrix)):
        return hull_intervalmatrix(arg)
      else:
        codac_error("hull: invalid input arguments")
    else:
      codac_error("hull: invalid input arguments")

  if mode == 0:
    return hull_intervalvector(lst)
  return hull_intervalmatrix(lst)


def cart_prod(*args):
  # -1: to be defined, 0: vector, 1: intervalvector, 2: ctc, 3: sep
  lst = []
  mode = -1

  for arg in args:

    if isinstance(arg, (int, float, Vector)):
      if mode == -1:
        mode = 0

    elif isinstance(arg, (list, Interval, IntervalVector)):
      if mode != 2 and mode != 3:
        mode = 1

    elif isinstance(arg, (Ctc_IntervalVector, Ctc_IntervalVector_)):
      mode = 2

    elif isinstance(arg, (Sep, SepBase)):
      mode = 3

    else:
      codac_error("cart_prod: invalid input arguments (a/" + str(mode) + ", " + str(arg) + ")")

  for arg in args:

    if isinstance(arg, (int, float)):
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

    elif isinstance(arg, (Interval)) or (isinstance(arg, list) and not isinstance(arg[0], list)):
      if mode == 1:
        lst.append(IntervalVector([arg]))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector([arg])))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector([arg])))
      else:
        codac_error("cart_prod: invalid input arguments (d/" + str(mode) + ")")

    elif isinstance(arg, (list, IntervalVector)) or (isinstance(arg, list) and isinstance(arg[0], list)):
      if mode == 1:
        lst.append(IntervalVector(arg))
      elif mode == 2:
        lst.append(CtcWrapper(IntervalVector(arg)))
      elif mode == 3:
        lst.append(SepWrapper(IntervalVector(arg)))
      else:
        codac_error("cart_prod: invalid input arguments (e/" + str(mode) + ")")

    elif isinstance(arg, (Ctc_IntervalVector, Ctc_IntervalVector_)):
      if mode != 2:
        codac_error("cart_prod: invalid input arguments (f/" + str(mode) + ")")
      lst.append(arg)

    elif isinstance(arg, (Sep, SepBase)):
      if mode != 3:
        codac_error("cart_prod: invalid input arguments (g/" + str(mode) + ")")
      lst.append(arg)

    else:
      mode = -2  # will generate an error

  if mode == 0:
    return cart_prod_vector(lst)
  if mode == 1:
    return cart_prod_intervalvector(lst)
  if mode == 2:
    return cart_prod_ctc(lst)
  if mode == 3:
    return cart_prod_sep(lst)
  codac_error("cart_prod: invalid input arguments (h/" + str(mode) + ")")


def tube_cart_prod(*x):
  if not isinstance(x, tuple):
    return tube_cart_prod_list([x])
  return tube_cart_prod_list([*x])


def traj_cart_prod(*x):
  if not isinstance(x, tuple):
    return traj_cart_prod_list([x])
  return traj_cart_prod_list([*x])


def _sampled_traj_type_and_value(x):
  if isinstance(x, (int, float)):
    return SampledTraj_Scalar, float

  if isinstance(x, Vector) \
      or (_np is not None and isinstance(x, _np.ndarray) and x.ndim == 1) \
      or (isinstance(x, (list, tuple)) and (len(x) == 0 or not isinstance(x[0], (list, tuple)))):
    return SampledTraj_Vector, lambda y: y if isinstance(y, Vector) else Vector(y)

  if isinstance(x, Matrix) \
      or (_np is not None and isinstance(x, _np.ndarray) and x.ndim == 2) \
      or (isinstance(x, (list, tuple)) and len(x) > 0 and isinstance(x[0], (list, tuple))):
    return SampledTraj_Matrix, lambda y: y if isinstance(y, Matrix) else Matrix(y)

  return None, None


def SampledTraj(x=None, y=None):
  if y is None:
    if x is None:
      codac_error("SampledTraj: unable to deduce the trajectory type from an empty constructor")

    for cls in _SAMPLED_TRAJ_TYPES:
      if isinstance(x, cls):
        return cls(dict(x))

    if isinstance(x, dict):
      if not x:
        codac_error("SampledTraj: unable to deduce the trajectory type from an empty map")
      cls, cast = _sampled_traj_type_and_value(next(iter(x.values())))
      if cls is None:
        codac_error("SampledTraj: wrong constructor argument")
      return cls({float(t): cast(v) for t, v in x.items()})

    codac_error("SampledTraj: wrong constructor argument")

  if len(y) == 0:
    codac_error("SampledTraj: unable to deduce the trajectory type from empty samples")

  cls, cast = _sampled_traj_type_and_value(y[0])
  if cls is None:
    codac_error("SampledTraj: unable to deduce the trajectory type from the provided samples")

  if cls is SampledTraj_Scalar:
    return cls(list(x), [float(v) for v in y])

  if cls is SampledTraj_Vector and _np is not None and isinstance(x, _np.ndarray) and isinstance(y, _np.ndarray):
    return cls(x, y)

  return cls({float(t): cast(v) for t, v in zip(x, y)})


def AnalyticTraj(t,f):

  if isinstance(t, _ANALYTIC_FUNCTION_TYPES) and not isinstance(f, _ANALYTIC_FUNCTION_TYPES):
    warnings.warn(
      "AnalyticTraj(f,t) is deprecated; use AnalyticTraj(t,f) instead.",
      FutureWarning,
      stacklevel=2
    )
    t,f = f,t

  f = AnalyticFunction(f)

  if isinstance(f, AnalyticFunction_Scalar):
    return AnalyticTraj_Scalar(t,f)

  if isinstance(f, AnalyticFunction_Vector):
    return AnalyticTraj_Vector(t,f)

  if isinstance(f, AnalyticFunction_Matrix):
    return AnalyticTraj_Matrix(t,f)

  codac_error("AnalyticTraj: can only build this trajectory from an AnalyticFunction_[Scalar/Vector/Matrix]")


def SlicedTube(x, y=None):

  if y is None:
    if isinstance(x, SlicedTube_Interval):
      return SlicedTube_Interval(x)
    if isinstance(x, SlicedTube_IntervalVector):
      return SlicedTube_IntervalVector(x)
    if isinstance(x, SlicedTube_IntervalMatrix):
      return SlicedTube_IntervalMatrix(x)
    codac_error("SlicedTube: unable to copy this tube from another one")

  y = AnalyticFunction(y) if isinstance(y, _ANALYTIC_FUNCTION_TYPES) else y

  if isinstance(y, (Interval, AnalyticFunction_Scalar, AnalyticTraj_Scalar, SampledTraj_Scalar)):
    return SlicedTube_Interval(x, y)

  if isinstance(y, (IntervalVector, AnalyticFunction_Vector, AnalyticTraj_Vector, SampledTraj_Vector)):
    return SlicedTube_IntervalVector(x, y)

  if isinstance(y, (IntervalMatrix, AnalyticFunction_Matrix, AnalyticTraj_Matrix, SampledTraj_Matrix)):
    return SlicedTube_IntervalMatrix(x, y)

  codac_error("SlicedTube: wrong constructor argument")


def fixpoint(contract, *x):
  vol = -1.0
  prev_vol = None

  while vol != prev_vol:

    prev_vol = vol
    if type(x) is tuple:
      x = contract(*x)
    else:  # prevent from unpacking
      x = contract(x)

    # For computing the volume:
    # only a real Python tuple is considered as a collection of objects.
    # Otherwise, any iterable object (tube, boxes, etc.) will be divided in the for loop.
    items = x if type(x) is tuple else (x,)

    vol = 0.0
    for xi in items:
      if xi.is_empty():
        return x
      w = xi.volume()
      # As infinity is absorbent, this would not
      # allow us to identify a contraction, so we
      # exclude these cases:
      if w != oo:
        vol += w

  return x


# Deprecated function draw_while_paving(..)
draw_while_paving = lambda *args, **kwargs: (_ for _ in ()).throw(
  NotImplementedError(
    "draw_while_paving(..) is deprecated,\n "
    "      please replace by DefaultFigure.pave(..) (or any Figure2D object)"
  )
)
