#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector
from codac import *
import codac as codac

class TestSlice(unittest.TestCase):

  def setUp(self):
    tube = Tube(Interval(0.,46.), 0.1, Interval(10.,11.));
    self.tube = tube

  def test_Init(self):
    self.assertTrue(self.tube.volume() == 46)
    #print(cos(self.tube).codomain())
    #self.assertTrue(cos(self.tube).codomain() == cos(Interval(10.,11.)))

  def test_Serialize(self):
    self.tube.serialize("/tmp/x.tube")
    
  def test_Slice_class(self):
  
    tubeslice = Slice(Interval(0.,1.), Interval(-1.,1.));
    self.assertEqual(tubeslice.tdomain(), Interval(0.,1.));
    self.assertEqual(tubeslice.codomain(), Interval(-1.,1.));

    # tubeslice_copy1 = tubeslice;
    # self.assertEqual(tubeslice_copy1.tdomain(), Interval(0.,1.));
    # self.assertEqual(tubeslice_copy1.codomain(), Interval(-1.,1.));
    # self.assertEqual(tubeslice_copy1, tubeslice);

    # Slice tubeslice_copy2(tubeslice);
    # self.assertEqual(tubeslice_copy2.tdomain(), Interval(0.,1.));
    # self.assertEqual(tubeslice_copy2.codomain(), Interval(-1.,1.));
    # self.assertEqual(tubeslice_copy2, tubeslice);

    # todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::ALL_REALS));
    # todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::EMPTY_SET));
class TestTube(unittest.TestCase):

  def assertApproxIntv(self, first, second):
    if isinstance(second, Interval):
      self.assertAlmostEqual(first.lb(), second.lb())
      self.assertAlmostEqual(first.ub(), second.ub())

  def test_Slice_class(self):
    tube = Tube (Interval(0.,1.), Interval(-1.,1.));
    self.assertEqual(tube.tdomain() , Interval(0.,1.));
    self.assertEqual(tube.codomain() , Interval(-1.,1.));

    tube_copy1 = tube;
    self.assertEqual(tube_copy1.tdomain() , Interval(0.,1.));
    self.assertEqual(tube_copy1.codomain() , Interval(-1.,1.));
    self.assertEqual(tube_copy1 , tube);

  def test_Tube_fct(self):
    tube_a = Tube(Interval(0.,12.), 1.);
    self.assertEqual(tube_a.nb_slices(), 12);
    self.assertEqual(tube_a.tdomain(), Interval(0.,12.));

    tube_b = Tube(Interval(0.,12.), 0.5, Interval(-3.,-2.));
    self.assertEqual(tube_b.nb_slices(), 24);
    self.assertEqual(tube_b.tdomain(), Interval(0.,12.));
    self.assertEqual(tube_b(3), Interval(-3.,-2.));
    self.assertEqual(tube_b.codomain(), Interval(-3.,-2.));

  def test_Tube_nb_slices(self):
    tube_a = Tube(Interval(0.,12.), 1.);
    self.assertEqual(tube_a.nb_slices(), 12);
    self.assertEqual(tube_a.tdomain(), Interval(0.,12.));

  def test_Tube_nb_slices2(self):
    tube_b = Tube(Interval(0.,12.), 0.5, Interval(-3.,-2.));
    self.assertEqual(tube_b.nb_slices(), 24);
    self.assertEqual(tube_b.tdomain(), Interval(0.,12.));
    self.assertEqual(tube_b(3), Interval(-3.,-2.));
    self.assertEqual(tube_b.codomain(), Interval(-3.,-2.));

  def test_Tube_nbslices3(self):
    tube_c = Tube(Interval(0.,4.), 1.1);
    self.assertEqual(tube_c.nb_slices(), 4);
    
    self.assertApproxIntv(tube_c.slice(0).tdomain(), Interval(0.,1.1));
    self.assertApproxIntv(tube_c.slice(1).tdomain(), Interval(1.1,2.2));
    self.assertApproxIntv(tube_c.slice(2).tdomain(), Interval(2.2,3.3));
    self.assertApproxIntv(tube_c.slice(3).tdomain(), Interval(3.3,4.));

#     Tube tube_d(tube_c);
#     CHECK(tube_d == tube_c);
#     CHECK(tube_d.codomain() == tube_c.codomain());
#     CHECK(tube_d.nb_slices() == tube_c.nb_slices());
#     //CHECK(tube_d.slice(0)->tube_reference() == &tube_d);
#     //CHECK(tube_d.slice(0)->tube_reference() == tube_d.slice(1)->tube_reference());
#     //CHECK(tube_d.slice(1)->tube_reference() == &tube_d);
#     //CHECK(tube_d.slice(2)->tube_reference() == &tube_d);
#     //CHECK(tube_d.slice(3)->tube_reference() == &tube_d);

#     Tube tube_e(tube_c, Interval(1.,2.));
#     //CHECK(tube_e.slice(0)->tube_reference() == &tube_e);
#     //CHECK(tube_e.slice(1)->tube_reference() == &tube_e);
#     //CHECK(tube_e.slice(2)->tube_reference() == &tube_e);
#     //CHECK(tube_e.slice(3)->tube_reference() == &tube_e);
#     CHECK(tube_e.codomain() == Interval(1.,2.));
#     CHECK(tube_e.nb_slices() == tube_c.nb_slices());
#     CHECK(tube_e.slice(0)->codomain() == Interval(1.,2.));
#     CHECK(tube_e.slice(1)->codomain() == Interval(1.,2.));
#     CHECK(tube_e.slice(2)->codomain() == Interval(1.,2.));
#     CHECK(tube_e.slice(3)->codomain() == Interval(1.,2.));

#     // todo: find a way to catch assert abort: CHECK_THROWS(Tube tube_f(Interval(0.,12.), -1.););

#     Tube tube_f(Interval(0.,12.), 0.);
#     CHECK(tube_f.tdomain() == Interval(0.,12.));
#     CHECK(tube_f.nb_slices() == 1);
    
class TestTubeVector(unittest.TestCase):

  def test_TubeVector(self):
    # modif Interval -> IntervalVector
    tube = TubeVector(Interval(0.,1.), IntervalVector(1, [-1.,1.]));
    self.assertEqual(tube.tdomain(), Interval(0.,1.));
    self.assertEqual(tube.codomain(), IntervalVector(1, [-1.,1.]));

    tube_copy1 = tube;
    self.assertEqual(tube_copy1.tdomain(), Interval(0.,1.));
    self.assertEqual(tube_copy1.codomain(), IntervalVector(1, [-1.,1.]));
    self.assertEqual(tube_copy1, tube);


class TestTubeClassFunction(unittest.TestCase):

  # def test_1(self):
  #   f = TFunction("t^2")
  #   f2 = TFunction("t^2")
  #   tube1 = Tube(Interval(-1.,10.), 0.01, f);
  #   f3 = TFunction("t^2+2")
  #   f4 = TFunction(f2)
  #   traj1_inside = Trajectory(Interval(-1.,10.), f2);
  #   print(f4.eval(Interval(-1.,10.)))

  #   traj1_outside = Trajectory(Interval(-1.,10.), f3);

  #   self.assertEqual(tube1.contains(traj1_inside), codac.YES);
  #   self.assertEqual(tube1.contains(traj1_outside), codac.NO);

  def test_2(self):
    tube2 = Tube(Interval(-1.,10.), 0.01, TFunction("t^2+[-1,1]"));
    traj3_lb = Trajectory(Interval(-1.,10.), TFunction("(t^2)-1"));
    traj3_ub = Trajectory(Interval(-1.,10.), TFunction("(t^2)+1"));
    tube3 = Tube(traj3_lb, traj3_ub, 0.01);
    self.assertEqual(tube2.nb_slices(), tube3.nb_slices());
    self.assertEqual(tube2.tdomain(), tube3.tdomain());
    self.assertEqual(tube2.codomain(), tube3.codomain());
    self.assertEqual(tube2.volume(), tube3.volume());
    for i in range(tube2.nb_slices()):
      self.assertEqual(tube2(i), tube3(i)); 
      

  def test_TubeVector_subvector(self):
    
    tdomain = Interval(-10, 10)
    vtube = TubeVector(Interval(-10, 10), 0.1, TFunction("(t+[-1,1]; t^2+[-1,1]; t^3+[-1,1])"))
    vtube2 = TubeVector(Interval(-10, 10), 0.1, TFunction("(t+[-1,1]; t^2+[-1,1])"))
    
    self.assertEqual(vtube[:].size(), 3)
    self.assertEqual(vtube[:1].size(), 1)
    self.assertEqual(vtube[:2].size(), 2)
    self.assertTrue(vtube[:2] == vtube2)    

# class TestTubeClassFunction_thick_slices(unittest.TestCase):
#   SECTION("Tube class - Function (thick slices)")
    # def test_3(self):
    #   tube = Tube(Interval(-1.,1.), 0.667, TFunction("t^2"));
    #   self.assertEqual(tube.nb_slices(), 3);
    #   self.assertEqual(ApproxIntv(tube(0)), Interval(pow(-1. + 0.667, 2), 1.));
    #   self.assertEqual(tube(-1.), Interval(1.));
    #   self.assertEqual(tube(1.), Interval(1.));
    #   self.assertEqual(tube(-0.9), tube(0));
    #   self.assertEqual(ApproxIntv(tube(1)), Interval(0., pow(tube.slice(1).tdomain().ub(), 2)));

#   SECTION("Tube class - 2 Trajectory")
#   {
#     Trajectory traj_lb(Interval(-1.,10.), TFunction("t^2"));
#     Trajectory traj_ub(Interval(-1.,10.), TFunction("t^2-2"));
    
#     Tube tube_1slice(traj_lb, traj_ub, traj_lb.tdomain().diam());
#     CHECK(tube_1slice.nb_slices() == 1);
#     CHECK(tube_1slice.tdomain() == Interval(-1.,10.));
#     CHECK(tube_1slice.codomain() == Interval(-2.,100.));
#     CHECK(Interval(28.25,30.25).is_subset(tube_1slice(5.5)));
#     CHECK(tube_1slice.max_diam() > 2.);
    
#     Tube tube_100slices(traj_lb, traj_ub, traj_ub.tdomain().diam() / 100.);
#     CHECK(tube_100slices.nb_slices() == 100);
#     CHECK(tube_100slices.tdomain() == Interval(-1.,10.));
#     CHECK(tube_100slices.codomain() == Interval(-2.,100.));
#     CHECK(Interval(28.25,30.25).is_subset(tube_100slices(5.5)));
#     CHECK(tube_100slices(5.5).is_subset(Interval(28.,32.)));
#     CHECK(tube_100slices.max_diam() > 2.);
#   }


  def test_TubeClasss(self):

    box1 = IntervalVector(2) 
    box2 = IntervalVector(2) 
    box3 = IntervalVector(2)
    box4 = IntervalVector(2)

    box1[0] = Interval(-1.,10.);
    box1[1] = Interval(3.,4.);

    box2[0] = Interval(10.,10.5);
    box2[1] = Interval(2.,7.);

    box3[0] = Interval(10.5,12.);
    box3[1] = Interval(5.,6.);

    box4[0] = Interval(12.,14.);
    box4[1] = Interval(5.5);

    v_tdomains = [box1[0], box2[0], box3[0], box4[0]]
    v_codomains = [box1[1], box2[1], box3[1], box4[1]]
    tube_from_boxes = Tube(v_tdomains, v_codomains);

    self.assertEqual(tube_from_boxes.tdomain(), Interval(-1.,14.));
    self.assertEqual(tube_from_boxes.codomain(), Interval(2.,7.));
    self.assertEqual(tube_from_boxes.nb_slices(), 4);

    self.assertEqual(tube_from_boxes(0), Interval(3.,4.));
    self.assertEqual(tube_from_boxes(1), Interval(2.,7.));
    self.assertEqual(tube_from_boxes(2), Interval(5.,6.));
    self.assertEqual(tube_from_boxes(3), Interval(5.5));

    self.assertEqual(tube_from_boxes.slice(0).tdomain(), Interval(-1.,10.));
    self.assertEqual(tube_from_boxes.slice(1).tdomain(), Interval(10.,10.5));
    self.assertEqual(tube_from_boxes.slice(2).tdomain(), Interval(10.5,12.));
    self.assertEqual(tube_from_boxes.slice(3).tdomain(), Interval(12.,14.));


  def test_Tube_class_vector_of_boxes_n_dim_case(self):
    box1 = IntervalVector([ 
        Interval(-1.,10.), 
        Interval(3.,4.), 
        Interval(0.,2.) 
    ])
    box2 = IntervalVector([
      Interval(10.,10.5),
      Interval(2.,7.),
      Interval(1.,3.)
    ])

    box3 = IntervalVector([
      Interval(10.5,12.),
      Interval(5.,6.),
      Interval(2.,4.)
    ])

    box4 = IntervalVector([
        Interval(12.,14.),
        Interval(5.5),
        Interval(3.,5.)
    ])

    v_tdomains = [ box1[0], box2[0], box3[0], box4[0]]
    v_codomains = [
      box1.subvector(1,2), 
      box2.subvector(1,2), 
      box3.subvector(1,2), 
      box4.subvector(1,2)
    ]

    tube_from_boxes  = TubeVector(v_tdomains, v_codomains)

    self.assertEqual(tube_from_boxes.size(), 2);
    self.assertEqual(tube_from_boxes.tdomain(), Interval(-1.,14.));
    self.assertEqual(tube_from_boxes.codomain()[0], Interval(2.,7.));
    self.assertEqual(tube_from_boxes.codomain()[1], Interval(0.,5.));
    self.assertEqual(tube_from_boxes.nb_slices(), 4);

    self.assertEqual(tube_from_boxes[0](0), Interval(3.,4.));
    self.assertEqual(tube_from_boxes[0](1), Interval(2.,7.));
    self.assertEqual(tube_from_boxes[0](2), Interval(5.,6.));
    self.assertEqual(tube_from_boxes[0](3), Interval(5.5));

    self.assertEqual(tube_from_boxes[0].slice(0).tdomain(), Interval(-1.,10.));
    self.assertEqual(tube_from_boxes[0].slice(1).tdomain(), Interval(10.,10.5));
    self.assertEqual(tube_from_boxes[0].slice(2).tdomain(), Interval(10.5,12.));
    self.assertEqual(tube_from_boxes[0].slice(3).tdomain(), Interval(12.,14.));

    self.assertEqual(tube_from_boxes[1](0), Interval(0.,2.));
    self.assertEqual(tube_from_boxes[1](1), Interval(1.,3.));
    self.assertEqual(tube_from_boxes[1](2), Interval(2.,4.));
    self.assertEqual(tube_from_boxes[1](3), Interval(3.,5.));

    self.assertEqual(tube_from_boxes[1].slice(0).tdomain(), Interval(-1.,10.));
    self.assertEqual(tube_from_boxes[1].slice(1).tdomain(), Interval(10.,10.5));
    self.assertEqual(tube_from_boxes[1].slice(2).tdomain(), Interval(10.5,12.));
    self.assertEqual(tube_from_boxes[1].slice(3).tdomain(), Interval(12.,14.));


if __name__ == '__main__':

  unittest.main()