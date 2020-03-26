#!/usr/bin/env python

import unittest
import pyibex
from pyibex_tube import *
import pyibex_tube as tubex
import numpy as np

class TestCtcEval(unittest.TestCase):
    def test_ctceval_1(self):

        t = Interval(0.,6.)
        z = Interval(-1.,1.)
        x = Tube(Interval(-1.,7.), 2.);
        v = Tube(x);
        v.set(Interval(-1.), 0);
        v.set(Interval(-1.,1.), 1);
        v.set(Interval(-1.), 2);
        v.set(Interval(1.), 3);

        self.assertEqual(x.nb_slices(), 4);
        ctc_eval = CtcEval();
        ctc_eval.preserve_slicing(False);
        ctc_eval.enable_temporal_propagation(False);
        print("ctc_eval")
        ctc_eval.contract(t, z, x, v);
        print("ctc_eval...DONE")
        self.assertEqual(x.nb_slices(), 6);

        self.assertEqual(x.codomain(), Interval.ALL_REALS); # only gates should be affected
        self.assertEqual(x(-1.), Interval.ALL_REALS);
        self.assertEqual(x(0.), Interval(-2.,6.));
        self.assertEqual(x(1.), Interval(-3.,5.));
        self.assertEqual(x(3.), Interval(-4.,3.));
        self.assertEqual(x(5.), Interval(-6.,1.));
        self.assertEqual(x(6.), Interval(-5.,2.));
        self.assertEqual(x(7.), Interval.ALL_REALS);


#   SECTION("Test CtcEval, 2")
#   {
#     Interval t(0.,6.), z(-1.,1.);
#     Tube x(Interval(-1.,7.), 2.);
#     Tube v(x);
#     v.set(Interval(-1.), 0);
#     v.set(Interval(-1.,1.), 1);
#     v.set(Interval(-1.), 2);
#     v.set(Interval(1.), 3);

#     x.set(Interval(0.,1.), 2.);
#     v.sample(2.);

#     CHECK(x.nb_slices() == 5);

#     CtcEval ctc_eval;
#     ctc_eval.preserve_slicing(false);
#     ctc_eval.enable_temporal_propagation(false);
#     ctc_eval.contract(t, z, x, v);

#     CHECK(x.nb_slices() == 7);

#     CHECK(x.codomain() == Interval::ALL_REALS); // only gates should be affected
#     CHECK(x(-1.) == Interval::ALL_REALS);
#     CHECK(x(0.) == Interval(0.,3.));
#     CHECK(x(1.) == Interval(-1.,2.));
#     CHECK(x(2.) == Interval(0.,1.));
#     CHECK(x(3.) == Interval(-1.,2.));
#     CHECK(x(5.) == Interval(-3.,1.));
#     CHECK(x(6.) == Interval(-2.,2.));
#     CHECK(x(7.) == Interval::ALL_REALS);

#     t = Interval(1.75,5.5);
#     z = Interval(1.6);

#     ctc_eval.enable_temporal_propagation(true);
#     ctc_eval.contract(t, z, x, v);

#     VIBesFigTube *fig_tube;
#     if(VIBES_DRAWING) // drawing results
#     {
#       vibes::beginDrawing();
#       x &= Interval(-10.,10.);
#       fig_tube = new VIBesFigTube("ctceval", &x);
#       fig_tube->set_properties(1500, 100, 500, 500);
#       fig_tube->set_tube_derivative(&x, &v);
#       fig_tube->show(true);
#     }

#     IntervalVector box(2);
#     box[0] = Interval(1.75,5.5);
#     box[1] = Interval(1.6);

#     if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "red"));
#     ctc_eval.contract(box[0], box[1], x, v);
#     if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "blue"));

#     CHECK(ApproxIntv(x.interpol(3.4,v)) == Interval(0.8,1.6));
#     CHECK(box[0] == Interval(2.6,3.4));
#     CHECK(box[1] == Interval(1.6));

#     if(VIBES_DRAWING)
#     {
#       delete fig_tube;
#       vibes::endDrawing();
#     }
#   }
if __name__ == '__main__':

  unittest.main()