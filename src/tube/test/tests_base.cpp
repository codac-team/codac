/* ============================================================================
 *  tube-lib - Tube class tests
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

bool test_getY()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.getY(), Interval(-11,13));
  success &= compare(test_tube1[7], Interval(-11,-7));
  success &= compare(test_tube1[3.5], Interval(-4,4));
  success &= compare(test_tube1[17.5], Interval(2,7));
  success &= compare(test_tube1[32.5], Interval(2,5));
  success &= compare(test_tube1[17.0], Interval(2,6)); // t belongs to two slices
  success &= compare(test_tube1[16.9999999], Interval(0,6));
  success &= compare(test_tube1[17.0000001], Interval(2,7));
  success &= compare(test_tube1[Interval(9.5,17.5)], Interval(-9,7));
  success &= compare(test_tube1[Interval(16.1,37.9)], Interval(0,13));
  success &= compare(test_tube1[Interval(37.4,45.6)], Interval(-1,6));
  success &= compare(test_tube1[Interval(14.0,27.0)], Interval(-4,13));
  success &= compare(test_tube1[Interval(19.2,35.7)], Interval(1,13));
  success &= compare(test_tube1[Interval(15.0,16.0)], Interval(-2,4));
  success &= compare(test_tube1[Interval(0.)], Interval(4,8));
  success &= compare(test_tube1[Interval(5.)], Interval(-7,-5));
  success &= compare(test_tube1[Interval(24.)], Interval(8,12));
  success &= compare(test_tube1[Interval(46.)], Interval(-1,3));

  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.getY(), Interval(-1.5,2));
  success &= compare(test_tube4[0], Interval(1,2));
  success &= compare(test_tube4[11], Interval(-1.5,-0.5));
  success &= compare(test_tube4[11.0], Interval(-1,-0.5));
  success &= compare(test_tube4[1.5], Interval(1,2));

  return success;
}

bool test_volume()
{
  bool success = true;
  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.volume(), 197.);
  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.volume(), 23.);
  return success;
}

bool test_input2index()
{
  bool success = true;
  Tube test_tube = tubeTest1();
  success &= test_tube.input2index(0.0) == 0;
  success &= test_tube.input2index(0.1) == 0;
  success &= test_tube.input2index(0.5) == 0;
  success &= test_tube.input2index(0.6) == 0;
  success &= test_tube.input2index(0.9) == 0;
  success &= test_tube.input2index(previous_float(1.0)) == 0;
  success &= test_tube.input2index(1.0) == 1;
  success &= test_tube.input2index(46.0) == 45;
  return success;
}

bool test_index2input()
{
  bool success = true;
  Tube test_tube1 = tubeTest1();
  success &= test_tube1.index2input(0) == 0.0;
  success &= test_tube1.input2index(1) == 1.0;
  success &= test_tube1.input2index(2) == 2.0;
  for(int i = 0 ; i < 46 ; i++)
    success &= (i == test_tube1.input2index(test_tube1.index2input(i)));
  return success;
}

bool test_union()
{
  bool success = true;
  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  Tube test_tube2 = tubeTest2();
  test_tube1 |= test_tube2;
  success &= compare(test_tube1.getY(), Interval(-11,13));
  success &= compare(test_tube1[0], Interval(-2,8));
  success &= compare(test_tube1[5.5], Interval(-9,6));
  success &= compare(test_tube1[11], Interval(-7,3));
  success &= compare(test_tube1[21.3], Interval(2,11));
  success &= compare(test_tube1[45], Interval(-1,3));
  success &= compare(test_tube1[Interval(5,10)], Interval(-11,6));
  success &= compare(test_tube1[Interval(15,20)], Interval(-3,9));
  success &= compare(test_tube1[Interval(8.2,39.9)], Interval(-10,13));
  return success;
}

bool test_intersection()
{
  bool success = true;
  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  Tube test_tube2 = tubeTest2();
  test_tube1 &= test_tube2;
  success &= compare(test_tube1.getY(), Interval(-2,4));
  success &= compare(test_tube1[0], Interval::EMPTY_SET);
  success &= compare(test_tube1[2.5], Interval(1,3));
  success &= compare(test_tube1[15.0], Interval(-2,0));
  success &= compare(test_tube1[44.5], Interval(0,2));
  success &= compare(test_tube1[39], Interval(-1));
  success &= compare(test_tube1[Interval(0,6)], Interval(1,4));
  success &= compare(test_tube1[Interval(11,19)], Interval(-2,1));
  success &= compare(test_tube1[Interval(39.5,40.3)], Interval(-1));
  return success;
}

bool test_enclosed_bounds()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.getEnclosedBounds(), make_pair(Interval(-11,9), Interval(-7,13)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(0.5,25.5)), make_pair(Interval(-11,9), Interval(-7,13)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(7.1,19.8)), make_pair(Interval(-11,6), Interval(-7,9)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(6.0,9.0)), make_pair(Interval(-11,-10), Interval(-7,-6)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(0.)), make_pair(Interval(4), Interval(8)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(5.)), make_pair(Interval(-7), Interval(-5)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(5.2)), make_pair(Interval(-9), Interval(-5)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(23.,24.)), make_pair(Interval(8), Interval(13)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(22.,25.)), make_pair(Interval(7,9), Interval(12,13)));
  success &= compare(test_tube1.getEnclosedBounds(Interval(21.,23.)), make_pair(Interval(8,9), Interval(11,12)));

  Tube test_tube4 = tubeTest4();
  Tube test_tube4_primitive = test_tube4.primitive();
  success &= compare(test_tube4_primitive.getEnclosedBounds(Interval(12.5,14.5)), make_pair(Interval(6,6.5), Interval(21,24.5)));
  
  return success;
}

bool test_set_inversion()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.setInversion(Interval(0.)), Interval(3.0,46.0));
  success &= compare(test_tube1.setInversion(Interval::ALL_REALS), Interval(0.0,46.0));
  success &= compare(test_tube1.setInversion(Interval(-12.0,14.0)), Interval(0.0,46.0));
  success &= compare(test_tube1.setInversion(Interval(-20,-18)), Interval::EMPTY_SET);
  success &= compare(test_tube1.setInversion(Interval(-1.0,1.0)), Interval(2.0,46.0));
  success &= compare(test_tube1.setInversion(Interval(-10.5)), Interval(7.0,8.0));
  success &= compare(test_tube1.setInversion(Interval(-12.0,-7.0)), Interval(4.0,12.0));
  success &= compare(test_tube1.setInversion(Interval(10.0,11.0)), Interval(20.0,27.0));
  success &= compare(test_tube1.setInversion(Interval(6.01,7.0)), Interval(0.0,30.0));
  success &= compare(test_tube1.setInversion(Interval(6.0,7.0)), Interval(0.0,43.0));
  success &= compare(test_tube1.setInversion(Interval(5.9,7.0)), Interval(0.0,43.0));
  return success;
}

bool test_set_inversion_vector()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation

  vector<Interval> v;

  test_tube1.setInversion(Interval(0.), v);
  success &= v.size() == 4;
  if(v.size() == 4)
  {
    success &= compare(v[0], Interval(3.0,4.0));
    success &= compare(v[1], Interval(14.0,17.0));
    success &= compare(v[2], Interval(37.0,42.0));
    success &= compare(v[3], Interval(43.0,46.0));
  }

  test_tube1.setInversion(Interval(-1.0,1.0), v);
  success &= v.size() == 4;
  if(v.size() == 4)
  {
    success &= compare(v[0], Interval(2.0,5.0));
    success &= compare(v[1], Interval(13.0,17.0));
    success &= compare(v[2], Interval(34.0,35.0));
    success &= compare(v[3], Interval(36.0,46.0));
  }

  test_tube1.setInversion(Interval::ALL_REALS, v);
  success &= v.size() == 1;
  if(v.size() == 1)
    success &= compare(v[0], Interval(0.0,46.0));

  test_tube1.setInversion(Interval(-30.0,-29.0), v);
  success &= v.size() == 0;

  test_tube1.setInversion(Interval(3.5), v);
  success &= v.size() == 5;
  if(v.size() == 5)
  {
    success &= compare(v[0], Interval(1.0,4.0));
    success &= compare(v[1], Interval(15.0,18.0));
    success &= compare(v[2], Interval(26.0,27.0));
    success &= compare(v[3], Interval(30.0,38.0));
    success &= compare(v[4], Interval(40.0,45.0));
  }

  test_tube1.setInversion(Interval(9.5,30.0), v);
  success &= v.size() == 1;
  if(v.size() == 1)
    success &= compare(v[0], Interval(20.0,27.0));

  test_tube1.setInversion(Interval(12.0,13.0), v);
  success &= v.size() == 1;
  if(v.size() == 1)
    success &= compare(v[0], Interval(22.0,25.0));

  test_tube1.setInversion(Interval(-4.0,-3.0), v);
  success &= v.size() == 3;
  if(v.size() == 3)
  {
    success &= compare(v[0], Interval(3.0,5.0));
    success &= compare(v[1], Interval(9.0,10.0));
    success &= compare(v[2], Interval(11.0,15.0));
  }

  return success;
}