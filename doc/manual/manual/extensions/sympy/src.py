#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
# [sympy-1-beg]
from codac import *
# sympy extension is already embedded in Codac
# [sympy-1-end]


class TestSympyManual(unittest.TestCase):

  def test_sympy_manual(self):
    # [sympy-2-beg]
    x = ScalarVar()
    f = AnalyticFunction([x], sqr(sin(x)) + sqr(cos(x)))

    fs = sympy_simplify(f)
    assert sympy_equal(fs, AnalyticFunction([x], 1.))
    # [sympy-2-end]

    # [sympy-3-beg]
    x = ScalarVar()
    f = AnalyticFunction([x], x*(x + 1.) - sqr(x))

    fs = sympy_simplify(f)
    assert sympy_equal(fs, AnalyticFunction([x], x))
    # [sympy-3-end]

    # [sympy-4-beg]
    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], x*y + sin(x))

    dfdx = sympy_partial_diff(f, x)
    dfdy = sympy_partial_diff(f, y)

    assert sympy_equal(dfdx, AnalyticFunction([x,y], y + cos(x)))
    assert sympy_equal(dfdy, AnalyticFunction([x,y], x))
    # [sympy-4-end]

    # [sympy-5-beg]
    v = VectorVar(2)
    g = AnalyticFunction([v], v[0]*v[1] + sin(v[0]))

    dg_dv0 = sympy_partial_diff(g, v[0])
    dg_dv1 = sympy_partial_diff(g, v[1])

    assert sympy_equal(dg_dv0, AnalyticFunction([v], v[1] + cos(v[0])))
    assert sympy_equal(dg_dv1, AnalyticFunction([v], v[0]))
    # [sympy-5-end]

    # [sympy-6-beg]
    x = ScalarVar()
    f = AnalyticFunction([x], cos(x)*x + sin(x))

    df = sympy_diff(f)
    d3f = sympy_diff(f, x, 3)

    assert sympy_equal(df, AnalyticFunction([x], 2.*cos(x) - x*sin(x)))
    assert sympy_equal(d3f, AnalyticFunction([x], x*sin(x) - 4.*cos(x)))
    # [sympy-6-end]

    # [sympy-7-beg]
    v = VectorVar(2)
    g = AnalyticFunction([v], v[0]*v[1] + sin(v[0]))

    grad_g = sympy_gradient(g)

    assert sympy_equal(
      grad_g,
      AnalyticFunction([v], vec(v[1] + cos(v[0]), v[0])))
    # [sympy-7-end]

    # [sympy-8-beg]
    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], x*y + sin(x) + sqr(y))

    H = sympy_hessian(f)

    assert sympy_equal(
      H,
      AnalyticFunction(
        [x,y],
        mat(
          vec(-sin(x), 1.),
          vec(1., 2.)
      )))
    # [sympy-8-end]

    # [sympy-9-beg]
    v = VectorVar(2)

    f = AnalyticFunction([v], [
      v[0]*v[1] + sin(v[0]),
      sqr(v[0]) + cos(v[1])
    ])

    J = sympy_diff(f)

    assert sympy_equal(
      J,
      AnalyticFunction(
        [v],
        mat(
          vec(v[1] + cos(v[0]), 2.*v[0]),
          vec(v[0], -sin(v[1]))
        )))
    # [sympy-9-end]

    # [sympy-10-beg]
    x = ScalarVar()
    y = ScalarVar()
    v = VectorVar(2)

    f = AnalyticFunction([x,y], x + 2.*y)
    g = AnalyticFunction([v], v[0] + 2.*v[1])

    assert sympy_equal(f, g)
    # [sympy-10-end]

    # [sympy-11-beg]
    x = ScalarVar()
    f = AnalyticFunction([x], 1. / (1. - x))

    p = sympy_series(f, x, 0.0, 3)

    assert sympy_equal(p, AnalyticFunction([x], 1. + x + sqr(x) + x*sqr(x)))
    # [sympy-11-end]

    # [sympy-12-beg]
    x = ScalarVar()
    y = ScalarVar()
    f = AnalyticFunction([x,y], y + 1. / (1. - x))

    p = sympy_series(f, x, 0.0, 2)

    assert sympy_equal(p, AnalyticFunction([x,y], y + 1. + x + sqr(x)))
    # [sympy-12-end]

    # [sympy-13-beg]
    x = ScalarVar()
    p = AnalyticFunction([x], 2.*pow(x,5) + pow(x,3) - 3.*sqr(x))

    h = sympy_horner(p)

    assert sympy_equal(h, AnalyticFunction([x], sqr(x)*(-3 + x*(1 + 2*sqr(x)))))
    # [sympy-13-end]


if __name__ == '__main__':
  unittest.main()
