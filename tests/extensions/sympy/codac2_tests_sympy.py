#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *


class TestSympy(unittest.TestCase):

  def test_sympy(self):
    x = ScalarVar()
    f = AnalyticFunction([x], cos(x)*x + sin(x))
    df = sympy_diff(f)
    dfx = sympy_diff(f, x)

    self.assertTrue(sympy_equal(df, AnalyticFunction([x], 2.*cos(x) - x*sin(x))))
    self.assertTrue(sympy_equal(dfx, AnalyticFunction([x], 2.*cos(x) - x*sin(x))))

    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], x*y + sin(x))

    dfdx = sympy_partial_diff(f, x)
    dfdy = sympy_partial_diff(f, y)

    self.assertTrue(sympy_equal(dfdx, AnalyticFunction([x,y], y + cos(x))))
    self.assertTrue(sympy_equal(dfdy, AnalyticFunction([x,y], x)))

    v = VectorVar(2)
    g = AnalyticFunction([v], v[0]*v[1] + sin(v[0]))

    dg_dv0 = sympy_partial_diff(g, v[0])
    dg_dv1 = sympy_partial_diff(g, v[1])
    grad_g = sympy_gradient(g)
    
    self.assertTrue(sympy_equal(dg_dv0, AnalyticFunction([v], v[1] + cos(v[0]))))
    self.assertTrue(sympy_equal(dg_dv1, AnalyticFunction([v], v[0])))
    self.assertTrue(sympy_equal(grad_g, AnalyticFunction([v], vec(v[1] + cos(v[0]), v[0]))))

    v = VectorVar(2)
    f = AnalyticFunction([v], [
      v[0]*v[1] + sin(v[0]),
      sqr(v[0]) + cos(v[1])
    ])

    J = sympy_diff(f)

    self.assertTrue(sympy_equal(
      J,
      AnalyticFunction(
        [v],
        mat(
          vec(v[1] + cos(v[0]), 2.*v[0]),
          vec(v[0], -sin(v[1]))
        ))))

    x = ScalarVar()
    f = AnalyticFunction([x], sqr(sin(x)) + sqr(cos(x)))
    fs = sympy_simplify(f)
    dfs = sympy_diff(fs)

    self.assertTrue(sympy_equal(fs, AnalyticFunction([x], 1.)))
    self.assertTrue(sympy_equal(dfs, AnalyticFunction([x], 0.*x)))

    v = VectorVar(2)
    f = AnalyticFunction([v], [
      v[0] + v[0],
      sqr(sin(v[1])) + sqr(cos(v[1]))
    ])

    fs = sympy_simplify(f)

    self.assertTrue(sympy_equal(fs, AnalyticFunction([v], [
      2.*v[0],
      1.
    ])))

    x = ScalarVar()
    f = AnalyticFunction([x], sin(x))

    d3f = sympy_diff(f, x, 3)
    d0f = sympy_diff(f, x, 0)

    self.assertTrue(sympy_equal(d3f, AnalyticFunction([x], -cos(x))))
    self.assertTrue(sympy_equal(d0f, f))

    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], x*y + sin(x) + sqr(y))

    H = sympy_hessian(f)

    self.assertTrue(sympy_equal(
      H,
      AnalyticFunction(
        [x,y],
        mat(
          vec(-sin(x), 1.),
          vec(1., 2.)
        ))))

    x = ScalarVar()
    f = AnalyticFunction([x], 1. / (1. - x))
    p = sympy_series(f, x, 0.0, 3)

    self.assertTrue(sympy_equal(p, AnalyticFunction([x], 1. + x + sqr(x) + x*sqr(x))))

    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], y + 1. / (1. - x))
    p = sympy_series(f, x, 0.0, 2)

    self.assertTrue(sympy_equal(p, AnalyticFunction([x,y], y + 1. + x + sqr(x))))

    x = ScalarVar()
    f = AnalyticFunction([x], 9.*pow(x,4) + 8.*pow(x,3) + 7.*sqr(x) + 6.*x + 5.)
    h = sympy_horner(f)

    self.assertTrue(sympy_equal(h, f))

    v = VectorVar(2)
    f = AnalyticFunction([v], [
      3.*pow(v[0],3) + 2.*sqr(v[0]) + v[0] + 1.,
      v[1]*(v[1] + 1.) + 2.
    ])

    h = sympy_horner(f)

    self.assertTrue(sympy_equal(h, f))

    A = MatrixVar(2,2)
    f = AnalyticFunction([A], mat(
      vec(A(0,0)*A(0,0) + 2.*A(0,0)*A(0,1) + 1., A(1,0) + 3.*A(1,1)),
      vec(A(0,0) - A(0,1), A(1,1)*A(1,1) + A(1,1) + 2.)
    ))
    h = sympy_horner(f)

    self.assertTrue(sympy_equal(h, f))

    x = ScalarVar()

    dtan = sympy_diff(AnalyticFunction([x], tan(x)))
    dasin = sympy_diff(AnalyticFunction([x], asin(x)))
    dacos = sympy_diff(AnalyticFunction([x], acos(x)))
    datan = sympy_diff(AnalyticFunction([x], atan(x)))
    dsinh = sympy_diff(AnalyticFunction([x], sinh(x)))
    dcosh = sympy_diff(AnalyticFunction([x], cosh(x)))
    dtanh = sympy_diff(AnalyticFunction([x], tanh(x)))

    self.assertTrue(sympy_equal(dtan,  AnalyticFunction([x], 1. / sqr(cos(x)))))
    self.assertTrue(sympy_equal(dasin, AnalyticFunction([x], 1. / sqrt(1. - sqr(x)))))
    self.assertTrue(sympy_equal(dacos, AnalyticFunction([x], -1. / sqrt(1. - sqr(x)))))
    self.assertTrue(sympy_equal(datan, AnalyticFunction([x], 1. / (1. + sqr(x)))))
    self.assertTrue(sympy_equal(dsinh, AnalyticFunction([x], cosh(x))))
    self.assertTrue(sympy_equal(dcosh, AnalyticFunction([x], sinh(x))))
    self.assertTrue(sympy_equal(dtanh, AnalyticFunction([x], 1. - sqr(tanh(x)))))

    x = ScalarVar()
    f = AnalyticFunction([x], abs(x))
    df = sympy_diff(f)

    self.assertTrue(sympy_equal(df, AnalyticFunction([x], sign(x))))

    y = ScalarVar()
    x = ScalarVar()
    f = AnalyticFunction([y,x], atan2(y, x))

    dfdy = sympy_partial_diff(f, y)
    dfdx = sympy_partial_diff(f, x)

    self.assertTrue(sympy_equal(dfdy, AnalyticFunction([y,x], x / (sqr(x) + sqr(y)))))
    self.assertTrue(sympy_equal(dfdx, AnalyticFunction([y,x], -y / (sqr(x) + sqr(y)))))

    A = MatrixVar(2,2)
    f = AnalyticFunction([A], A(0,0)*A(1,1) + sin(A(0,1)) + A(1,0))

    dfdA00 = sympy_partial_diff(f, A(0,0))
    dfdA01 = sympy_partial_diff(f, A(0,1))
    dfdA10 = sympy_partial_diff(f, A(1,0))
    dfdA11 = sympy_partial_diff(f, A(1,1))
    grad_f = sympy_gradient(f)
    
    self.assertTrue(sympy_equal(dfdA00, AnalyticFunction([A], A(1,1))))
    self.assertTrue(sympy_equal(dfdA01, AnalyticFunction([A], cos(A(0,1)))))
    self.assertTrue(sympy_equal(dfdA10, AnalyticFunction([A], 1.)))
    self.assertTrue(sympy_equal(dfdA11, AnalyticFunction([A], A(0,0))))
    self.assertTrue(sympy_equal(
      grad_f,
      AnalyticFunction([A], vec(A(1,1), cos(A(0,1)), 1., A(0,0)))))

    A = MatrixVar(2,2)
    f = AnalyticFunction([A], [
      A(0,0) + A(1,1),
      A(0,1)*A(1,0)
    ])

    J = sympy_diff(f)

    self.assertTrue(sympy_equal(
      J,
      AnalyticFunction(
        [A],
        mat(
          vec(1., 0.),
          vec(0., A(1,0)),
          vec(0., A(0,1)),
          vec(1., 0.)
        ))))

    x = ScalarVar()
    y = ScalarVar()
    v = VectorVar(2)
    A = MatrixVar(2,1)

    self.assertTrue(sympy_equal(
      AnalyticFunction([x,y], x + 2.*y),
      AnalyticFunction([v], v[0] + 2.*v[1])))

    self.assertTrue(sympy_equal(
      AnalyticFunction([x,y], vec(x+y, x-y)),
      AnalyticFunction([v], vec(v[0] + v[1], v[0] - v[1]))))

    self.assertTrue(sympy_equal(
      AnalyticFunction([x,y], mat(vec(x+y, x-y))),
      AnalyticFunction([A], mat(vec(A(0,0) + A(1,0), A(0,0) - A(1,0))))))

if __name__ == '__main__':
  unittest.main()
