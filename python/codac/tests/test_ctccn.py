#!/usr/bin/env python

# Codac tests
# ---------------------------------------------------------------------------
# \date      2022
# \author    Julien DAMERS
# \copyright Copyright 2022 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
from codac import *
import codac as codac


class TestCtcCN(unittest.TestCase):
    def test(self):

        domain = Interval(0.,10.)
        a = Tube(domain, 0.01, TFunction("t"))
        t = 1.0
        x = IntervalVector(2)
        x[0] = Interval(t,t)
        x[1] = Interval.ALL_REALS

        ctc_f = CtcFunction(Function("x[2]","a","(x[1]-2-a)"))
        ctc_eval = CtcEval()

        cn = ContractorNetwork()
        box = IntervalVectorVar(2)
        at = cn.create_interm_var(Interval())
        cn.add(ctc_eval,[box[0],at,a])
        cn.add(ctc_f,[box,at])
        ctc_cn = CtcCN(cn,box)

        ctc_cn.contract(x)

        self.assertEqual(x[1], a(t) + 2)

if __name__ == '__main__':
    unittest.main()