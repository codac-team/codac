from codac._core._sympy import *
from codac import AnalyticFunction, AnalyticFunction_Scalar, AnalyticFunction_Vector, AnalyticFunction_Matrix

def sympy_simplify(f):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_simplify_scalar(f.f))
  elif isinstance(f.f, (AnalyticFunction_Vector)):
    return AnalyticFunction(sympy_simplify_vector(f.f))
  elif isinstance(f.f, (AnalyticFunction_Matrix)):
    return AnalyticFunction(sympy_simplify_matrix(f.f))
  else:
    codac_error("sympy_simplify: invalid function input")

def sympy_horner(f):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_horner_scalar(f.f))
  elif isinstance(f.f, (AnalyticFunction_Vector)):
    return AnalyticFunction(sympy_horner_vector(f.f))
  elif isinstance(f.f, (AnalyticFunction_Matrix)):
    return AnalyticFunction(sympy_horner_matrix(f.f))
  else:
    codac_error("sympy_horner: invalid function input")

def sympy_partial_diff(f,x):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_partial_diff_(f.f,x))
  else:
    codac_error("sympy_partial_diff: invalid function input")

def sympy_diff(f,*args):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_diff_scalar(f.f,*args))
  elif isinstance(f.f, (AnalyticFunction_Vector)):
    return AnalyticFunction(sympy_diff_vector(f.f,*args))
  else:
    codac_error("sympy_diff: invalid function input")

def sympy_gradient(f):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_gradient_(f.f))
  else:
    codac_error("sympy_gradient: invalid function input")

def sympy_hessian(f):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_hessian_(f.f))
  else:
    codac_error("sympy_hessian: invalid function input")

def sympy_series(f,*args):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    return AnalyticFunction(sympy_series_(f.f,*args))
  else:
    codac_error("sympy_series: invalid function input")

def sympy_equal(f,g):
  if isinstance(f.f, (AnalyticFunction_Scalar)):
    if not isinstance(g.f, (AnalyticFunction_Scalar)):
      return False
    return sympy_equal_scalar(f.f,g.f)
  elif isinstance(f.f, (AnalyticFunction_Vector)):
    if not isinstance(g.f, (AnalyticFunction_Vector)):
      return False
    return sympy_equal_vector(f.f,g.f)
  elif isinstance(f.f, (AnalyticFunction_Matrix)):
    if not isinstance(g.f, (AnalyticFunction_Matrix)):
      return False
    return sympy_equal_matrix(f.f,g.f)
  else:
    codac_error("sympy_equal: invalid function inputs")