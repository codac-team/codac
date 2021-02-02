#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector
from codac import *

class TestCtcEval(unittest.TestCase):

    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)

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
        ctc_eval.enable_time_propag(False);
        ctc_eval.contract(t, z, x, v);
        self.assertEqual(x.nb_slices(), 6);

        self.assertEqual(x.codomain(), Interval.ALL_REALS); # only gates should be affected
        self.assertEqual(x(-1.), Interval.ALL_REALS);
        self.assertEqual(x(0.), Interval(-2.,6.));
        self.assertEqual(x(1.), Interval(-3.,5.));
        self.assertEqual(x(3.), Interval(-4.,3.));
        self.assertEqual(x(5.), Interval(-6.,1.));
        self.assertEqual(x(6.), Interval(-5.,2.));
        self.assertEqual(x(7.), Interval.ALL_REALS);


    def test_ctceval_2(self):
        t, z =  Interval(0.,6.), Interval(-1.,1.);
        x = Tube(Interval(-1.,7.), 2.);
        v = Tube(x);
        v.set(Interval(-1.), 0);
        v.set(Interval(-1.,1.), 1);
        v.set(Interval(-1.), 2);
        v.set(Interval(1.), 3);

        x.set(Interval(0.,1.), 2.);
        v.sample(2.);

        self.assertEqual(x.nb_slices(), 5);

        ctc_eval = CtcEval();
        ctc_eval.preserve_slicing(False);
        ctc_eval.enable_time_propag(False);
        ctc_eval.contract(t, z, x, v);

        self.assertEqual(x.nb_slices(), 7);

        self.assertEqual(x.codomain(), Interval.ALL_REALS); # only gates should be affected
        self.assertEqual(x(-1.), Interval.ALL_REALS);
        self.assertEqual(x(0.), Interval(0.,3.));
        self.assertEqual(x(1.), Interval(-1.,2.));
        self.assertEqual(x(2.), Interval(0.,1.));
        self.assertEqual(x(3.), Interval(-1.,2.));
        self.assertEqual(x(5.), Interval(-3.,1.));
        self.assertEqual(x(6.), Interval(-2.,2.));
        self.assertEqual(x(7.), Interval.ALL_REALS);

        t = Interval(1.75,5.5);
        z = Interval(1.6);

        ctc_eval.enable_time_propag(True);
        ctc_eval.contract(t, z, x, v);


        box = IntervalVector([Interval(1.75,5.5), Interval(1.6)])

        # if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "red"));
        ctc_eval.contract(box[0], box[1], x, v);
        # if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "blue"));

        self.assertApproxIntv(x.interpol(3.4,v), Interval(0.8,1.6));
        self.assertApproxIntv(box[0], Interval(2.6,3.4));
        self.assertApproxIntv(box[1], Interval(1.6));


class TestCtcEval2(unittest.TestCase):

  def assertApproxIntv(self, first, second):
    if first.is_empty() is False:
    # if isinstance(second, Interval):
        self.assertAlmostEqual(first.lb(), second.lb())
        self.assertAlmostEqual(first.ub(), second.ub())
    else:
        self.assertEqual(first, second)

  def setUp(self):
    xdot = Tube(Interval(0., 10.), 1.0);
    xdot.set(Interval(-0.5,1.));

    x = Tube(xdot);
    x.set(Interval.ALL_REALS);
    x.set(Interval(-1.5,1.), 4);
    x.set(Interval(-1.,1.5), 5);
    self.assertEqual(x.nb_slices(), 10);
    
    self.ctc_deriv = CtcDeriv();
    self.ctc_deriv.contract(x, xdot);

    self.x_raw = Tube(x)
    self.xdot_raw = Tube(xdot);

    self.ctc_eval_propa = CtcEval()
    self.ctc_eval_nopropa = CtcEval()
    self.ctc_eval_propa.preserve_slicing(False);
    self.ctc_eval_nopropa.preserve_slicing(False);
    self.ctc_eval_propa.enable_time_propag(True);
    self.ctc_eval_nopropa.enable_time_propag(False);

  def test_1(self):
    # Checking the tube...
    self.assertEqual(self.x_raw(0), Interval(-5.5,3.));
    self.assertEqual(self.x_raw(1), Interval(-4.5,2.5));
    self.assertEqual(self.x_raw(2), Interval(-3.5,2.));
    self.assertEqual(self.x_raw(3), Interval(-2.5,1.5));
    self.assertEqual(self.x_raw(4), Interval(-1.5,1.));
    self.assertEqual(self.x_raw(5), Interval(-1.,1.5));
    self.assertEqual(self.x_raw(6), Interval(-1.5,2.5));
    self.assertEqual(self.x_raw(7), Interval(-2.,3.5));
    self.assertEqual(self.x_raw(8), Interval(-2.5,4.5));
    self.assertEqual(self.x_raw(9), Interval(-3.,5.5));

      # Interval intv_t, intv_y;
      

  def test_1_no_propa(self):
    # Test A_no_propa
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    self.assertEqual(x(1.), Interval(-4.5,2.5));
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(intv_t, 1.);
    self.assertEqual(intv_y, Interval(-0.5,1.));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1.), intv_y);
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(7), Interval(-2.,3.5));
    self.assertEqual(x(8), Interval(-2.5,4.5));
    self.assertEqual(x(9), Interval(-3.,5.5));

  def  test_A__propa(self): 
    # Test A_propa
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    self.assertEqual(x(1.), Interval(-4.5,2.5));
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(intv_t, 1.);
    self.assertEqual(intv_y, Interval(-0.5,1.));
    self.assertEqual(x(0), Interval(-1.5,1.5));
    self.assertEqual(x(1.), intv_y);
    self.assertEqual(x(1), Interval(-1.,2.));
    self.assertEqual(x(2), Interval(-1.5,2.));
    self.assertApproxIntv(x(3), Interval(-1.5-1./3.,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(7), Interval(-2.,3.5));
    self.assertEqual(x(8), Interval(-2.5,4.5));
    self.assertEqual(x(9), Interval(-3.,5.5));

  def test_A_ctc_deriv(self):
  # def test_A: ctc_deriv should not be effective after ctc_eval(true)(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
  def test_B_ctc_deriv(self):
    # Test B_no_propa
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(0.5,2.5);
    intv_y = Interval(-2.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(0.5,2.5));
    self.assertEqual(intv_y, Interval(-2.));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(0.5), Interval(-4.,-1.));
    self.assertEqual(x(1), Interval(-5.5,3.));
    self.assertEqual(x(1.), Interval(-3.5,-1.25));
    self.assertEqual(x(2), Interval(-4.5,2.5));
    self.assertEqual(x(2.), Interval(-2.75,-0.5));
    self.assertEqual(x(3), Interval(-3.5,2.));
    self.assertEqual(x(2.5), Interval(-3.,-0.));
    self.assertEqual(x(4), Interval(-3.5,2.));
    self.assertEqual(x(5), Interval(-2.5,1.5));
    self.assertEqual(x(6), Interval(-1.5,1.));
    self.assertEqual(x(7), Interval(-1.,1.5));
    self.assertEqual(x(8), Interval(-1.5,2.5));
    self.assertEqual(x(9), Interval(-2.,3.5));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_B_ctc_deriv_propa(self):
    # Test B_propa
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(0.5,2.5);
    intv_y = Interval(-2.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(0.5,2.5));
    self.assertEqual(intv_y, Interval(-2.));
    self.assertEqual(x(0), Interval(-4.5,-0.75));
    self.assertEqual(x(0.5), Interval(-4.,-1.));
    self.assertEqual(x(1), Interval(-4.,-1.));
    self.assertEqual(x(1.), Interval(-3.5,-1.25));
    self.assertEqual(x.slice(0.6).tdomain(), Interval(0.5,1.));
    self.assertEqual(x.slice(0.6).input_gate(), Interval(-4.,-1.));
    self.assertEqual(x.slice(0.6).output_gate(), Interval(-3.5,-1.25));
    self.assertEqual(xdot(2), Interval(-0.5,1.));
    #self.assertEqual(x(2), Interval(-3.5,-0.5)); # todo: optimal implementation?
    self.assertEqual(x(2.), Interval(-2.75,-0.5));
    self.assertEqual(x(3), Interval(-3.,0.));
    self.assertEqual(x(2.5), Interval(-3.,-0.));
    self.assertEqual(x(4), Interval(-3.,0.5));
    self.assertApproxIntv(x(5), Interval(-2.5,1.+1./6.));
    self.assertEqual(x(6), Interval(-1.5,1.));
    self.assertEqual(x(7), Interval(-1.,1.5));
    self.assertEqual(x(8), Interval(-1.5,2.5));
    self.assertEqual(x(9), Interval(-2.,3.5));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));


  def test_C_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(0.5,3.5);
    intv_y = Interval(-4.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(0.5,1.5));
    self.assertEqual(intv_y, Interval(-4.));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(0.5), Interval(-5.,-3.5));
    self.assertEqual(x(1), Interval(-5.5,3.));
    self.assertEqual(x(1.), Interval(-4.5,-3.5));
    self.assertEqual(x(2), Interval(-4.5,2.5));
    self.assertEqual(x(1.5), Interval(-4.,-3.));
    self.assertEqual(x(3), Interval(-4.5,2.5));
    self.assertEqual(x(4), Interval(-3.5,2.));
    self.assertEqual(x(5), Interval(-2.5,1.5));
    self.assertEqual(x(6), Interval(-1.5,1.));
    self.assertEqual(x(7), Interval(-1.,1.5));
    self.assertEqual(x(8), Interval(-1.5,2.5));
    self.assertEqual(x(9), Interval(-2.,3.5));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_C_propas(self):
      x = Tube(self.x_raw);
      xdot = Tube(self.xdot_raw);
      intv_t = Interval(0.5,3.5);
      intv_y = Interval(-4.);
      self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
      self.assertEqual(x.nb_slices(), 12);
      self.assertEqual(intv_t, Interval(0.5,1.5));
      self.assertEqual(intv_y, Interval(-4.));
      self.assertEqual(x(0), Interval(-5.5,-3.25));
      self.assertEqual(x(0.5), Interval(-5.,-3.5));
      # //self.assertEqual(x(1), Interval(-5.,-3.5)); // optimality
      self.assertEqual(x(1.), Interval(-4.5,-3.5));
      self.assertEqual(x(2), Interval(-4.5,-3.));
      self.assertEqual(x(1.5), Interval(-4.,-3.));
      self.assertEqual(x(3), Interval(-4.,-2.5));
      self.assertEqual(x(4), Interval(-3.5,-1.5));
      self.assertEqual(x(5), Interval(-2.5,-0.5));
      self.assertEqual(x(6), Interval(-1.5,0.5));
      self.assertEqual(x(7), Interval(-1.,1.5));
      self.assertEqual(x(8), Interval(-1.5,2.5));
      self.assertEqual(x(9), Interval(-2.,3.5));
      self.assertEqual(x(10), Interval(-2.5,4.5));
      self.assertEqual(x(11), Interval(-3.,5.5));

#     if(false & VIBES_DRAWING) // drawing results
#     {
#       //x = x_raw;
#       //xdot = xdot_raw;
#       //IntervalVector box(2);
#       //box[0] = Interval(0.5,3.5);
#       //box[1] = Interval(-4.);
#       //vibes::beginDrawing();
#       //VIBesFigTube fig_tube("ctceval", &x);
#       //fig_tube.set_properties(100, 100, 500, 500);
#       //fig_tube.set_tube_derivative(&x, &xdot);
#       //fig_tube.show(true);
#       //contraction = ctc_eval_propa.contract(box[0], box[1], x, xdot);
#       //fig_tube.show(true);
#       //vibes::drawBox(box, vibesParams("figure", "ctceval", "blue"));
#       //vibes::endDrawing();
#     }

  def test_D_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    self.assertEqual(x.nb_slices(), 10);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(intv_t, 3.5);
    self.assertEqual(intv_y, Interval(-2.,-0.5));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(3.5), Interval(-2.,-0.5));
    self.assertEqual(x(4), Interval(-2.5,1.5));
    self.assertEqual(x(5), Interval(-1.5,1.));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_D_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    self.assertEqual(x.nb_slices(), 10);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(intv_t, 3.5);
    self.assertEqual(intv_y, Interval(-2.,-0.5));
    self.assertEqual(x(0), Interval(-5.5,1.25));
    self.assertEqual(x(1), Interval(-4.5,0.75));
    self.assertEqual(x(2), Interval(-3.5,0.25));
    self.assertEqual(x(3), Interval(-2.5,-0.25));
    self.assertEqual(x(3.5), Interval(-2.,-0.5));
    self.assertEqual(x(4), Interval(-2.,0.));
    self.assertEqual(x(5), Interval(-1.5,1.));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_E_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(7.5, 8.5));
    self.assertEqual(intv_y, Interval(-2.25, -1.75));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(7), Interval(-2.,3.5));
    self.assertEqual(x(7.5), Interval(-1.75,-1.25));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(8.), Interval(-2.,-1.25));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_E_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(7.5, 8.5));
    self.assertEqual(intv_y, Interval(-2.25, -1.75));
    self.assertEqual(x(0), Interval(-5.5,2.5));
    self.assertEqual(x(1), Interval(-4.5,2.));
    self.assertEqual(x(2), Interval(-3.5,1.5));
    self.assertEqual(x(3), Interval(-2.5,1.));
    self.assertEqual(x(4), Interval(-1.5,0.5));
    self.assertEqual(x(5), Interval(-1.,0.));
    self.assertEqual(x(6), Interval(-1.5,-0.5));
    self.assertEqual(x(7), Interval(-1.75,-1.));
    self.assertEqual(x(7.5), Interval(-1.75,-1.25));
#     //CHECK(x(8), Interval(-2.,-1.25)); // optimality
    self.assertEqual(x(8.), Interval(-2.,-1.25));
    self.assertEqual(x(9), Interval(-2.25,-0.75));
    self.assertEqual(x(10), Interval(-2.5,-0.25));
    self.assertEqual(x(11), Interval(-3.,0.75));

  def test_F_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(8.5,9.5));
    self.assertEqual(intv_y, Interval(-0.5,2.5));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(7), Interval(-2.,3.5));
    self.assertEqual(x(8), Interval(-2.5,4.5));
    self.assertEqual(x(8.5), Interval(-1.5,3.));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));
    self.assertEqual(x(9.5), Interval(-1.,3.5));
    self.assertEqual(x(11), Interval(-3,5.5));

  def test_F_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(intv_t, Interval(8.5,9.5));
    self.assertEqual(intv_y, Interval(-0.5,2.5));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertApproxIntv(x(7), Interval(-2.,3.+1./3.));
    self.assertEqual(x(8), Interval(-2.,3.25));
    self.assertEqual(x(8.5), Interval(-1.5,3.));
#     //CHECK(x(9), Interval(-1.5,3.)); // optimality
#     //CHECK(x(10), Interval(-1.,3.5)); // optimality
    self.assertEqual(x(9.5), Interval(-1.,3.5));
    self.assertEqual(x(11), Interval(-1.25,4.));

  def test_G_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(7.5));
    self.assertEqual(intv_y, Interval(3.));
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(7), Interval(-2.,3.5));
    self.assertEqual(x(7.5), Interval(3.));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_G_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(7.5));
    self.assertEqual(intv_y, Interval(3.));
    self.assertEqual(x(0), Interval(-4.5,3.));
    self.assertEqual(x(1), Interval(-3.5,2.5));
    self.assertEqual(x(2), Interval(-2.5,2.));
    self.assertEqual(x(3), Interval(-1.5,1.5));
    self.assertEqual(x(4), Interval(-0.5,1.));
    self.assertEqual(x(5), Interval(0.5,1.5));
    self.assertEqual(x(6), Interval(1.5,2.5));
    self.assertEqual(x(7), Interval(2.5,3.));
    self.assertEqual(x(7.5), Interval(3.));
    self.assertEqual(x(8), Interval(2.75,3.5));
    self.assertEqual(x(9), Interval(2.25,4.5));
    self.assertEqual(x(10), Interval(1.75,5.5));

  def test_H_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval.EMPTY_SET);
    self.assertEqual(intv_y, Interval.EMPTY_SET);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(x(0), Interval.EMPTY_SET);
    self.assertEqual(x(1), Interval.EMPTY_SET);
    self.assertEqual(x(2), Interval.EMPTY_SET);
    self.assertEqual(x(3), Interval.EMPTY_SET);
    self.assertEqual(x(4), Interval.EMPTY_SET);
    self.assertEqual(x(5), Interval.EMPTY_SET);
    self.assertEqual(x(6), Interval.EMPTY_SET);
    self.assertEqual(x(7), Interval.EMPTY_SET);
    self.assertEqual(x(8), Interval.EMPTY_SET);
    self.assertEqual(x(9), Interval.EMPTY_SET);
    self.assertTrue(x.is_empty());

  def test_H_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval.EMPTY_SET);
    self.assertEqual(intv_y, Interval.EMPTY_SET);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(x(0), Interval.EMPTY_SET);
    self.assertEqual(x(1), Interval.EMPTY_SET);
    self.assertEqual(x(2), Interval.EMPTY_SET);
    self.assertEqual(x(3), Interval.EMPTY_SET);
    self.assertEqual(x(4), Interval.EMPTY_SET);
    self.assertEqual(x(5), Interval.EMPTY_SET);
    self.assertEqual(x(6), Interval.EMPTY_SET);
    self.assertEqual(x(7), Interval.EMPTY_SET);
    self.assertEqual(x(8), Interval.EMPTY_SET);
    self.assertEqual(x(9), Interval.EMPTY_SET);
    self.assertTrue(x.is_empty());

  def test_I_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(2.5);
    intv_y = Interval(0.5);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(2.5));
    self.assertEqual(intv_y, Interval(0.5));
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(2.5), Interval(0.5));
    self.assertEqual(x(3), Interval(-3.5,2.));
    self.assertEqual(x(4), Interval(-2.5,1.5));
    self.assertEqual(x(5), Interval(-1.5,1.));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_I_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(2.5);
    intv_y = Interval(0.5);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(2.5));
    self.assertEqual(intv_y, Interval(0.5));
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(x(0), Interval(-2.,1.75));
    self.assertEqual(x(1), Interval(-1.,1.25));
    self.assertEqual(x(2), Interval(-0.,0.75));
    self.assertEqual(x(2.5), Interval(0.5));
    self.assertEqual(x(3), Interval(0.25,1.));
    self.assertApproxIntv(x(4), Interval(-0.25,1.+1./3.));
    self.assertEqual(x(5), Interval(-0.75,1.));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_J_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, 6.25);
    self.assertEqual(intv_y, Interval(0.5,1.));
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(6.25), Interval(0.5,1.));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));

  def test_J_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, 6.25);
    self.assertEqual(intv_y, Interval(0.5,1.));
    self.assertEqual(x.nb_slices(), 11);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5.), Interval(-0.75,1.));
    self.assertApproxIntv(x(5), Interval(-0.75,1.4+1./60.));
    self.assertEqual(x(6), Interval(0.25,1.125));
    self.assertEqual(x(7), Interval(0.125,1.75));
    self.assertEqual(x(8), Interval(0.125-0.5,2.75));
    self.assertEqual(x(9), Interval(0.125-1.,3.75));
    self.assertEqual(x(10), Interval(0.125-1.5,4.75));

  def test_K_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(5.25, 8.25));
    self.assertEqual(intv_y, Interval(-0.5, 0.));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(5.25), Interval(-1.,1.25));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(8.25), Interval(-2.,3.));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_K_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(5.25, 8.25));
    self.assertEqual(intv_y, Interval(-0.5, 0.));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.25));
    self.assertEqual(x(5.25), Interval(-1.,1.25));
    self.assertApproxIntv(x(6), Interval(-1.,1.4+1./60.));
    self.assertEqual(x(7), Interval(-1.5,1.875));
    self.assertApproxIntv(x(8), Interval(-1.625-1./3.,2.75));
    self.assertEqual(x(9), Interval(-2.,3.));
    self.assertEqual(x(8.25), Interval(-2.,3.));
    self.assertEqual(x(10), Interval(-2.375,3.75));
    self.assertEqual(x(11), Interval(-2.875,4.75));

  def test_L_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(1.5,4.5);
    intv_y = Interval(1.75,2.75);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(1.5,2.5));
    self.assertEqual(intv_y, Interval(1.75,2.25));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(1.5), Interval(0.75,2.25));
    self.assertEqual(x(2), Interval(-4.5,2.5));
    self.assertEqual(x(3), Interval(-3.5,2.));
    self.assertEqual(x(2.5), Interval(1.25,1.75));
    self.assertEqual(x(4), Interval(-3.5,2.));
    self.assertEqual(x(5), Interval(-2.5,1.5));
    self.assertEqual(x(6), Interval(-1.5,1.));
    self.assertEqual(x(7), Interval(-1.,1.5));
    self.assertEqual(x(8), Interval(-1.5,2.5));
    self.assertEqual(x(9), Interval(-2.,3.5));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_L_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(1.5,2.5);
    intv_y = Interval(1.75,2.75);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(1.5,2.5));
    self.assertEqual(intv_y, Interval(1.75,2.25));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-0.75,3.));
    self.assertEqual(x(1), Interval(0.25,2.5));
    self.assertEqual(x(1.5), Interval(0.75,2.25));
    self.assertEqual(x(2), Interval(0.75,2.25));
#     //CHECK(x(3), Interval(1.25,2.)); // optimality
    self.assertEqual(x(2.5), Interval(1.25,1.75));
    self.assertEqual(x(4), Interval(1.,1.75));
    self.assertEqual(x(5), Interval(0.5,1.5));
    self.assertEqual(x(6), Interval(0.,1.));
    self.assertEqual(x(7), Interval(-0.5,1.5));
    self.assertEqual(x(8), Interval(-1.,2.5));
    self.assertEqual(x(9), Interval(-1.5,3.5));
    self.assertEqual(x(10), Interval(-2.,4.5));
    self.assertEqual(x(11), Interval(-2.5,5.5));

  def test_M_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval.EMPTY_SET);
    self.assertEqual(intv_y, Interval.EMPTY_SET);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(x(0), Interval.EMPTY_SET);
    self.assertEqual(x(1), Interval.EMPTY_SET);
    self.assertEqual(x(2), Interval.EMPTY_SET);
    self.assertEqual(x(3), Interval.EMPTY_SET);
    self.assertEqual(x(4), Interval.EMPTY_SET);
    self.assertEqual(x(5), Interval.EMPTY_SET);
    self.assertEqual(x(6), Interval.EMPTY_SET);
    self.assertEqual(x(7), Interval.EMPTY_SET);
    self.assertEqual(x(8), Interval.EMPTY_SET);
    self.assertEqual(x(9), Interval.EMPTY_SET);
    self.assertTrue(x.is_empty());

  def test_M_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval.EMPTY_SET);
    self.assertEqual(intv_y, Interval.EMPTY_SET);
    self.assertEqual(x.nb_slices(), 10);
    self.assertEqual(x(0), Interval.EMPTY_SET);
    self.assertEqual(x(1), Interval.EMPTY_SET);
    self.assertEqual(x(2), Interval.EMPTY_SET);
    self.assertEqual(x(3), Interval.EMPTY_SET);
    self.assertEqual(x(4), Interval.EMPTY_SET);
    self.assertEqual(x(5), Interval.EMPTY_SET);
    self.assertEqual(x(6), Interval.EMPTY_SET);
    self.assertEqual(x(7), Interval.EMPTY_SET);
    self.assertEqual(x(8), Interval.EMPTY_SET);
    self.assertEqual(x(9), Interval.EMPTY_SET);
    self.assertTrue(x.is_empty());

  def test_N_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(6.5,8.5));
    self.assertEqual(intv_y, Interval(2.,4.));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-1.5,2.5));
    self.assertEqual(x(6.5), Interval(0.,2.));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(8.5), Interval(1.,4.));
    self.assertEqual(x(10), Interval(-2.5,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_N_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(6.5,8.5));
    self.assertEqual(intv_y, Interval(2.,4.));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(3), Interval(-2.5,1.5));
    self.assertEqual(x(4), Interval(-1.5,1.));
    self.assertEqual(x(5), Interval(-1.,1.5));
    self.assertEqual(x(6), Interval(-0.5,2.));
    self.assertEqual(x(6.5), Interval(0.,2.));
    self.assertEqual(x(7), Interval(0.,2.5));
#     //CHECK(x(8), Interval(xxx)); // optimality
    self.assertEqual(x(9), Interval(1.,4.));
    self.assertEqual(x(8.5), Interval(1.,4.));
    self.assertEqual(x(10), Interval(0.75,4.5));
    self.assertEqual(x(11), Interval(0.25,5.5));

  def test_0_no_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    self.ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(2.5,9.5));
    self.assertEqual(intv_y, Interval(-3.,-2.5));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,3.));
    self.assertEqual(x(1), Interval(-4.5,2.5));
    self.assertEqual(x(2), Interval(-3.5,2.));
    self.assertEqual(x(2.5), Interval(-3.,1.));
    self.assertEqual(x(3), Interval(-3.5,2.));
    self.assertEqual(x(4), Interval(-2.5,1.5));
    self.assertEqual(x(5), Interval(-1.5,1.));
    self.assertEqual(x(6), Interval(-1.,1.5));
    self.assertEqual(x(7), Interval(-1.5,2.5));
    self.assertEqual(x(8), Interval(-2.,3.5));
    self.assertEqual(x(9), Interval(-2.5,4.5));
    self.assertEqual(x(10), Interval(-3.,5.5));
    self.assertEqual(x(9.5), Interval(-2.75,4.5));
    self.assertEqual(x(11), Interval(-3.,5.5));

  def test_0_propa(self):
    x = Tube(self.x_raw);
    xdot = Tube(self.xdot_raw);
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    self.ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    self.assertEqual(intv_t, Interval(2.5,9.5));
    self.assertEqual(intv_y, Interval(-3.,-2.5));
    self.assertEqual(x.nb_slices(), 12);
    self.assertEqual(x(0), Interval(-5.5,2.25));
    self.assertEqual(x(1), Interval(-4.5,1.75));
    self.assertEqual(x(2), Interval(-3.5,1.25));
    self.assertEqual(x(2.5), Interval(-3.,1.));
    self.assertEqual(x(3), Interval(-3.,1.));
    self.assertEqual(x(4), Interval(-2.5,0.75));
#     //CHECK(x(5), Interval(xxx)); // optimality
    self.assertEqual(x(6), Interval(-1.,1.));
    self.assertEqual(x(7), Interval(-1.5,2.));
    self.assertEqual(x(8), Interval(-2.,3.));
    self.assertEqual(x(9), Interval(-2.5,4.));
    self.assertEqual(x(10), Interval(-2.75,4.5));
    self.assertEqual(x(9.5), Interval(-2.75,4.5));
    self.assertEqual(x(11), Interval(-3.,5.));
#   }

  def Test_CtcEval_multi_eval(self):

    x = Tube(Interval(0.,20.), 0.1, TFunction("cos(t)+t*[-0.1,0.2]"));
    v = Tube(Interval(0.,20.), 0.1, TFunction("-sin(t)+[-0.1,0.2]"));
    
    ctc_deriv = CtcDeriv();
    ctc_deriv.contract(x, v);

    box = IntervalVector(2);
    ctc_eval = CtcEval();
    ctc_eval.enable_time_propag(True);
    x_c = Tube(x); 
    v_c = Tube(v);

    box[0] = Interval(11.98);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

    box[0] = Interval(6.5);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

#     if(VIBES_DRAWING) // drawing results
#     {
#       //vibes::beginDrawing();
#       //VIBesFigTube fig_tube("ctceval", &x);
#       //fig_tube.set_properties(100, 100, 800, 400);
#       //fig_tube.set_tube_derivative(&x, &v);
#       //fig_tube.show(true);
#       //x = x_c;
#       //v = v_c;
#       //fig_tube.show(true);
#       //vibes::drawBox(box, vibesParams("figure", "ctceval", "red"));
#     }
#   }
  def  Test_CtcEval_non_zero_derivative_negative_case(self):
#   SECTION("Test CtcEval, non-zero derivative (negative case)")

    x = Tube(Interval(0.,11.), 1.);
    v = Tube(x);
    v.set(Interval(-1.5,-1.));
    x.set(Interval(6.,8.), 0);

    ctc_eval = CtcEval;
    ctc_deriv = CtcDeriv;
    ctc_deriv.contract(x, v);

    self.assertEqual(x(0), Interval(6.,8.)); 
    self.assertEqual(x(1), Interval(4.5,7.));
    self.assertEqual(x(2), Interval(3.,6.));
    self.assertEqual(x(3), Interval(1.5,5.));
    self.assertEqual(x(4), Interval(-0.,4.));
    self.assertEqual(x(5), Interval(-1.5,3.));
    self.assertEqual(x(6), Interval(-3.,2.));
    self.assertEqual(x(7), Interval(-4.5,1.));
    self.assertEqual(x(8), Interval(-6.,0.));
    self.assertEqual(x(9), Interval(-7.5,-1.));
    self.assertEqual(x(10), Interval(-9.,-2.));

    # Interval t, y;
    # x_raw = Tube(x);
    # v_raw = Tube(v);

    # x = Tube(x_raw);
    # v = Tube(v_raw);
    # t = Interval(3.5,6.);
    # y = Interval(3.5,6.);
    # ctc_eval.contract(t, y, x, v);
  #   /*CHECK(t, Interval(3.5,5.));
  #     self.assertEqual(y, Interval(3.5,4.5));
  #     self.assertEqual(x(0), Interval(6.,8.));
  #     self.assertEqual(x(1), Interval(4.5,7.));
  #     self.assertEqual(x(2), Interval(4.,6.));
  #     self.assertEqual(x(3), Interval(2.75,5.));
  #     self.assertEqual(x(4), Interval(1.25,4.));
  #     self.assertEqual(x(5), Interval(-0.25,3.));
  #     self.assertEqual(x(6), Interval(-3.,2.));
  #     self.assertEqual(x(7), Interval(-4.5,1.));
  #     self.assertEqual(x(8), Interval(-6.,0.));
  #     self.assertEqual(x(9), Interval(-7.5,-1.));
  #     self.assertEqual(x(10), Interval(-9.,-2.));

  #   x = x_raw;
  #   v = v_raw;
  #   t = Interval(3.,8.5);
  #   y = Interval(-7.,-2.5);
  #   ctc_eval.contract(t, y, x, v);
  #     self.assertEqual(t, Interval(6.,8.5));
  #     self.assertEqual(y, Interval(-5.25,-2.5));
  #     self.assertEqual(x(0), Interval(6.,8.)); 
  #     self.assertEqual(x(1), Interval(4.5,7.));
  #     self.assertEqual(x(2), Interval(3.,6.));
  #     self.assertEqual(x(3), Interval(1.5,5.));
  #     self.assertEqual(x(4), Interval(-0.,4.));
  #     self.assertEqual(x(5), Interval(-1.5,2.75));
  #     self.assertEqual(x(6), Interval(-3.,1.25));
  #     self.assertEqual(x(7), Interval(-4.5,-0.25));
  #     self.assertEqual(x(8), Interval(-6.,-1.75));
  #     self.assertEqual(x(9), Interval(-7.5,-3.));
  #     self.assertEqual(x(10), Interval(-9.,-2.));

  #   x = x_raw;
  #   v = v_raw;
  #   t = Interval(7.,8.);
  #   y = Interval(-2.,-1.);
  #   ctc_eval.contract(t, y, x, v);
  #     self.assertEqual(t, Interval(7.,8.));
  #     self.assertEqual(y, Interval(-2.,-1.));
  #     self.assertEqual(x(0), Interval(6.,8.)); 
  #     self.assertEqual(x(1), Interval(4.5,7.));
  #     self.assertEqual(x(2), Interval(3.,6.));
  #     self.assertEqual(x(3), Interval(1.5,5.));
  #     self.assertEqual(x(4), Interval(-0.,4.));
  #     self.assertEqual(x(5), Interval(-1.,3.));
  #     self.assertEqual(x(6), Interval(-2.,2.));
  #     self.assertEqual(x(7), Interval(-3.5,0.5));
  #     self.assertEqual(x(8), Interval(-5.,-1.));
  #     self.assertEqual(x(9), Interval(-6.5,-2.));
  #     self.assertEqual(x(10), Interval(-9.,-2.));

  #   x = x_raw;
  #   v = v_raw;
  #   t = Interval(0.);
  #   y = Interval(6.,8.);
  #   ctc_eval.contract(t, y, x, v);
  #     self.assertEqual(t, Interval(0.));
  #     self.assertEqual(y, Interval(7.,8.));
  #     self.assertEqual(x(0), Interval(6.,8.)); 
  #     self.assertEqual(x(1), Interval(4.5,7.));
  #     self.assertEqual(x(2), Interval(3.,6.));
  #     self.assertEqual(x(3), Interval(1.5,5.));
  #     self.assertEqual(x(4), Interval(-0.,4.));
  #     self.assertEqual(x(5), Interval(-1.5,3.));
  #     self.assertEqual(x(6), Interval(-3.,2.));
  #     self.assertEqual(x(7), Interval(-4.5,1.));
  #     self.assertEqual(x(8), Interval(-6.,0.));
  #     self.assertEqual(x(9), Interval(-7.5,-1.));
  #     self.assertEqual(x(10), Interval(-9.,-2.));/*
  # }

#   SECTION("Test CtcEval, non-zero derivative (positive case)")
#   {
#     Tube tube(Interval(0.,20.), 1.);
#     Tube derivative(tube);
#     derivative.set(Interval(1.,1.5));
#     tube.set(Interval(-8.,-6.), 0);
#     tube.ctc_deriv(derivative);

      # self.assertEqual(tube(0), Interval(-8.,-6.)); 
      # self.assertEqual(tube(1), Interval(-7.,-4.5)); 
      # self.assertEqual(tube(2), Interval(-6.,-3.)); 
      # self.assertEqual(tube(3), Interval(-5.,-1.5)); 
      # self.assertEqual(tube(4), Interval(-4.,0.)); 
      # self.assertEqual(tube(5), Interval(-3.,1.5)); 
      # self.assertEqual(tube(6), Interval(-2.,3.)); 
      # self.assertEqual(tube(7), Interval(-1.,4.5)); 
      # self.assertEqual(tube(8), Interval(-0.,6.)); 
      # self.assertEqual(tube(9), Interval(1.,7.5)); 
      # self.assertEqual(tube(10), Interval(2.,9.)); 

#     Interval intv_t, intv_y;
#     Tube tube_raw(tube);

      # intv_t = Interval(5.,10.);
      # intv_y = Interval(-3.,-1.);
#     tube.ctc_eval(intv_t, intv_y, derivative, false);
      # self.assertEqual(intv_t, Interval(5.,8.));
      # self.assertEqual(intv_y, Interval(-3.,-1.));
      # self.assertEqual(tube(0), Interval(-8.,-6.)); 
      # self.assertEqual(tube(1), Interval(-7.,-4.5)); 
      # self.assertEqual(tube(2), Interval(-6.,-3.)); 
      # self.assertEqual(tube(3), Interval(-5.,-2.)); 
      # self.assertEqual(tube(4), Interval(-4.,-1.)); 
      # self.assertEqual(tube(5), Interval(-3.,0.5)); 
      # self.assertEqual(tube(6), Interval(-2.,2.)); 
      # self.assertEqual(tube(7), Interval(-1.,3.5)); 
      # self.assertEqual(tube(8), Interval(0.,5.)); 
      # self.assertEqual(tube(9), Interval(1.,7.5)); 
      # self.assertEqual(tube(10), Interval(2.,9.)); 

#     tube = tube_raw;
      # intv_t = Interval(2.,5.5);
      # intv_y = Interval(-0.5,3.);
#     tube.ctc_eval(intv_t, intv_y, derivative, false);
      # self.assertEqual(intv_t, Interval(4.,5.5));
      # self.assertEqual(intv_y, Interval(-0.5,0.75));
      # self.assertEqual(tube(0), Interval(-8.,-6.)); 
      # self.assertEqual(tube(1), Interval(-7.,-4.5)); 
      # self.assertEqual(tube(2), Interval(-6.,-3.)); 
      # self.assertEqual(tube(3), Interval(-4.25,-1.5)); 
      # self.assertEqual(tube(4), Interval(-2.75,0.)); 
      # self.assertEqual(tube(5), Interval(-1.25,1.5)); 
      # self.assertEqual(tube(6), Interval(0.,3.)); 
      # self.assertEqual(tube(7), Interval(-1.,4.5)); 
      # self.assertEqual(tube(8), Interval(0.,6.));
      # self.assertEqual(tube(9), Interval(1.,7.5)); 
      # self.assertEqual(tube(10), Interval(2.,9.)); 

#     tube = tube_raw;
      # intv_t = Interval(3.5,7.);
      # intv_y = Interval(-5.2,-4.8);
#     tube.ctc_eval(intv_t, intv_y, derivative, false);
#       self.assertEqual(intv_t, Interval.EMPTY_SET);
#       self.assertEqual(intv_y, Interval.EMPTY_SET);
#       self.assertEqual(tube(0), Interval(-8.,-6.)); 
#       self.assertEqual(tube(1), Interval(-7.,-4.5)); 
#       self.assertEqual(tube(2), Interval.EMPTY_SET); 
#       self.assertEqual(tube(3), Interval.EMPTY_SET); 
#       self.assertEqual(tube(4), Interval.EMPTY_SET); 
#       self.assertEqual(tube(5), Interval(-3.,1.5)); 
#       self.assertEqual(tube(6), Interval(-2.,3.)); 
#       self.assertEqual(tube(7), Interval(-1.,4.5)); 
#       self.assertEqual(tube(8), Interval(-0.,6.)); 
#       self.assertEqual(tube(9), Interval(1.,7.5)); 
#       self.assertEqual(tube(10, Interval(2.,9.));*/
# #   }
# }
if __name__ ==  '__main__':
  unittest.main()