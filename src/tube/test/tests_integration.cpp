/* ============================================================================
 *  tube-lib - Tube class tests
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

bool test_primitive()
{
  bool success = true;

  Tube test_tube = tubeTest3();
  success &= compare(test_tube[0], Interval(1,3));
  success &= compare(test_tube[1], Interval(0,2));
  success &= compare(test_tube[2], Interval(-1,1));
  success &= compare(test_tube[3], Interval(-2,0));
  success &= compare(test_tube[4], Interval(-3,-1));

  Tube test_tube_primitive = test_tube.primitive();
  success &= compare(test_tube_primitive[0], Interval(0,3));
  success &= compare(test_tube_primitive[1], Interval(1,5));
  success &= compare(test_tube_primitive[2], Interval(0,6));
  success &= compare(test_tube_primitive[3], Interval(-2,6));
  success &= compare(test_tube_primitive[4], Interval(-5,6));

  Tube test_tube4 = tubeTest4();
  Tube test_tube_primitive4 = test_tube4.primitive();
  success &= compare(test_tube_primitive4[0], Interval(0,2));
  success &= compare(test_tube_primitive4[1], Interval(1,4));
  success &= compare(test_tube_primitive4[2], Interval(2,6));
  success &= compare(test_tube_primitive4[3], Interval(3,8));
  success &= compare(test_tube_primitive4[4], Interval(4,10));
  success &= compare(test_tube_primitive4[5], Interval(5,12));
  success &= compare(test_tube_primitive4[6], Interval(6,14));
  success &= compare(test_tube_primitive4[7], Interval(7,16));
  success &= compare(test_tube_primitive4[8], Interval(8,18));
  success &= compare(test_tube_primitive4[9], Interval(9,19.5));
  success &= compare(test_tube_primitive4[10], Interval(8.5,20.5));
  success &= compare(test_tube_primitive4[11], Interval(7,20.5));
  success &= compare(test_tube_primitive4[12], Interval(6,21));
  success &= compare(test_tube_primitive4[13], Interval(6,22.5));
  success &= compare(test_tube_primitive4[14], Interval(6.5,24.5));
  success &= compare(test_tube_primitive4[15], Interval(7.5,26.5));
  success &= compare(test_tube_primitive4[16], Interval(8.5,28.5));
  success &= compare(test_tube_primitive4[17], Interval(9.5,30.5));
  success &= compare(test_tube_primitive4[18], Interval(10.5,32.5));
  success &= compare(test_tube_primitive4[19], Interval(11.5,34.5));
  success &= compare(test_tube_primitive4[20], Interval(12.5,36.5));

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  Tube test_tube_primitive1 = test_tube1.primitive();
  success &= compare(test_tube_primitive1[0], Interval(0,8));
  success &= compare(test_tube_primitive1[1], Interval(4,15));
  success &= compare(test_tube_primitive1[2], Interval(6,21));

  Tube test_tube4_05 = tubeTest4_05();
  Tube test_tube4_05_primitive = test_tube4_05.primitive();
  success &= compare(test_tube4_05_primitive[0], Interval(0.0,1));
  success &= compare(test_tube4_05_primitive[1], Interval(0.5,2));
  success &= compare(test_tube4_05_primitive[2], Interval(1.0,3));
  success &= compare(test_tube4_05_primitive[3], Interval(1.5,4));
  success &= compare(test_tube4_05_primitive[4], Interval(2.0,5));
  success &= compare(test_tube4_05_primitive[5], Interval(2.5,6));
  success &= compare(test_tube4_05_primitive[6], Interval(3.0,7));
  success &= compare(test_tube4_05_primitive[7], Interval(3.5,8));
  success &= compare(test_tube4_05_primitive[8], Interval(4.0,9));
  success &= compare(test_tube4_05_primitive[9], Interval(4.5,10));
  success &= compare(test_tube4_05_primitive[10], Interval(5.0,11));
  success &= compare(test_tube4_05_primitive[11], Interval(5.5,12));
  success &= compare(test_tube4_05_primitive[12], Interval(6.0,13));
  success &= compare(test_tube4_05_primitive[13], Interval(6.5,14));

  return success;
}

bool test_from_0_integration_double()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.timeIntegration(0), Interval(0));
  success &= compare(test_tube1.timeIntegration(0.1), Interval(0.4,0.8));
  success &= compare(test_tube1.timeIntegration(1.1), Interval(4.2,8.7));
  success &= compare(test_tube1.timeIntegration(0.5), Interval(2,4));
  success &= compare(test_tube1.timeIntegration(1.0), Interval(4,8));
  success &= compare(test_tube1.timeIntegration(1.5), Interval(5,11.5));
  success &= compare(test_tube1.timeIntegration(2.0), Interval(6,15));
  success &= compare(test_tube1.timeIntegration(2.5), Interval(6.5,18));
  success &= compare(test_tube1.timeIntegration(2.9), Interval(6.9,20.4));
  success &= compare(test_tube1.timeIntegration(3.0), Interval(7,21));
  success &= compare(test_tube1.timeIntegration(3.1), Interval(6.6,21.4));
  success &= compare(test_tube1.timeIntegration(4.0), Interval(3,25));

  Tube test_tube1_01 = tubeTest1_01();
  success &= compare(test_tube1_01.timeIntegration(0), Interval(0));
  success &= compare(test_tube1_01.timeIntegration(0.1), Interval(0.4,0.8));
  success &= compare(test_tube1_01.timeIntegration(1.1), Interval(4.2,8.7));
  success &= compare(test_tube1_01.timeIntegration(0.5), Interval(2,4));
  success &= compare(test_tube1_01.timeIntegration(1.0), Interval(4,8));
  success &= compare(test_tube1_01.timeIntegration(1.5), Interval(5,11.5));
  success &= compare(test_tube1_01.timeIntegration(2.0), Interval(6,15));
  success &= compare(test_tube1_01.timeIntegration(2.5), Interval(6.5,18));
  success &= compare(test_tube1_01.timeIntegration(2.9), Interval(6.9,20.4));
  success &= compare(test_tube1_01.timeIntegration(3.0), Interval(7,21));
  success &= compare(test_tube1_01.timeIntegration(3.1), Interval(6.6,21.4));
  success &= compare(test_tube1_01.timeIntegration(4.0), Interval(3,25));
  
  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.timeIntegration(0.0), Interval(0.0));
  success &= compare(test_tube4.timeIntegration(0.1), Interval(0.1,0.2));
  success &= compare(test_tube4.timeIntegration(0.9), Interval(0.9,1.8));
  success &= compare(test_tube4.timeIntegration(1.0), Interval(1,2));
  success &= compare(test_tube4.timeIntegration(1.2), Interval(1.2,2.4));
  success &= compare(test_tube4.timeIntegration(2.0), Interval(2.0,4.0));
  success &= compare(test_tube4.timeIntegration(3.0), Interval(3.0,6.0));
  success &= compare(test_tube4.timeIntegration(5.0), Interval(5.0,10.0));
  success &= compare(test_tube4.timeIntegration(7.0), Interval(7.0,14.0));
  success &= compare(test_tube4.timeIntegration(9.0), Interval(9.0,18.0));
  success &= compare(test_tube4.timeIntegration(10.0), Interval(9.5,19.5));
  success &= compare(test_tube4.timeIntegration(10.1), Interval(9.4,19.6));
  success &= compare(test_tube4.timeIntegration(11.0), Interval(8.5,20.5));
  success &= compare(test_tube4.timeIntegration(12.5), Interval(6.5,20.5));
  success &= compare(test_tube4.timeIntegration(12.6), Interval(6.4,20.6));
  success &= compare(test_tube4.timeIntegration(14.5), Interval(7,23.5));

  return success;
}

bool test_from_0_integration_interval()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.timeIntegration(Interval(0)), Interval(0));
  success &= compare(test_tube1.timeIntegration(Interval(0.1)), Interval(0.4,0.8));
  success &= compare(test_tube1.timeIntegration(Interval(0.5)), Interval(2,4));
  success &= compare(test_tube1.timeIntegration(Interval(1.0)), Interval(4,8));
  success &= compare(test_tube1.timeIntegration(Interval(1.5)), Interval(5,11.5));
  success &= compare(test_tube1.timeIntegration(Interval(2.0)), Interval(6,15));
  success &= compare(test_tube1.timeIntegration(Interval(2.5)), Interval(6.5,18));
  success &= compare(test_tube1.timeIntegration(Interval(3.0)), Interval(7,21));
  success &= compare(test_tube1.timeIntegration(Interval(3.0)), Interval(7,21));
  success &= compare(test_tube1.timeIntegration(Interval(0.0,0.0)), Interval(0,0));
  success &= compare(test_tube1.timeIntegration(Interval(0.0,0.5)), Interval(0,4));
  success &= compare(test_tube1.timeIntegration(Interval(0.0,1.0)), Interval(0,8));
  success &= compare(test_tube1.timeIntegration(Interval(0.0,1.5)), Interval(0,11.5));
  success &= compare(test_tube1.timeIntegration(Interval(0.0,2.0)), Interval(0,15));
  success &= compare(test_tube1.timeIntegration(Interval(1.0,1.0)), Interval(4,8));
  success &= compare(test_tube1.timeIntegration(Interval(1.0,1.5)), Interval(4,11.5));
  success &= compare(test_tube1.timeIntegration(Interval(1.0,2.0)), Interval(4,15));
  success &= compare(test_tube1.timeIntegration(Interval(1.1,1.9)), Interval(4.2,14.3));
  success &= compare(test_tube1.timeIntegration(Interval(1.1,2.1)), Interval(4.2,15.6));
  success &= compare(test_tube1.timeIntegration(Interval(6,7)), Interval(-23,19));
  success &= compare(test_tube1.timeIntegration(Interval(0,46)), Interval(-85,194));

  Tube test_tube1_01 = tubeTest1_01();
  success &= compare(test_tube1_01.timeIntegration(Interval(0)), Interval(0));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.1)), Interval(0.4,0.8));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.5)), Interval(2,4));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.0)), Interval(4,8));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.5)), Interval(5,11.5));
  success &= compare(test_tube1_01.timeIntegration(Interval(2.0)), Interval(6,15));
  success &= compare(test_tube1_01.timeIntegration(Interval(2.5)), Interval(6.5,18));
  success &= compare(test_tube1_01.timeIntegration(Interval(3.0)), Interval(7,21));
  success &= compare(test_tube1_01.timeIntegration(Interval(3.0)), Interval(7,21));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.0,0.0)), Interval(0,0));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.0,0.5)), Interval(0,4));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.0,1.0)), Interval(0,8));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.0,1.5)), Interval(0,11.5));
  success &= compare(test_tube1_01.timeIntegration(Interval(0.0,2.0)), Interval(0,15));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.0,1.0)), Interval(4,8));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.0,1.5)), Interval(4,11.5));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.0,2.0)), Interval(4,15));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.1,1.9)), Interval(4.2,14.3));
  success &= compare(test_tube1_01.timeIntegration(Interval(1.1,2.1)), Interval(4.2,15.6));
  success &= compare(test_tube1_01.timeIntegration(Interval(6,7)), Interval(-23,19));
  success &= compare(test_tube1_01.timeIntegration(Interval(0,46)), Interval(-85,194));

  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.timeIntegration(Interval(12.5)), Interval(6.5,20.5));
  success &= compare(test_tube4.timeIntegration(Interval(14.5)), Interval(7,23.5));
  success &= compare(test_tube4.timeIntegration(Interval(12.5,14.5)), Interval(6.0,23.5));
  success &= compare(test_tube4.timeIntegration(Interval(0)), Interval(0));
  success &= compare(test_tube4.timeIntegration(Interval(10.2)), Interval(9.3,19.7));
  success &= compare(test_tube4.timeIntegration(Interval(1.5,3.5)), Interval(1.5,7));
  success &= compare(test_tube4.timeIntegration(Interval(9,21)), Interval(6,36.5));

  Tube test_tube4_05 = tubeTest4_05();
  success &= compare(test_tube4_05.timeIntegration(Interval(12.5)), Interval(6.5,20.5));
  success &= compare(test_tube4_05.timeIntegration(Interval(14.5)), Interval(7,23.5));
  success &= compare(test_tube4_05.timeIntegration(Interval(12.5,14.5)), Interval(6.0,23.5));
  success &= compare(test_tube4_05.timeIntegration(Interval(0)), Interval(0));
  success &= compare(test_tube4_05.timeIntegration(Interval(10.2)), Interval(9.3,19.7));
  success &= compare(test_tube4_05.timeIntegration(Interval(1.5,3.5)), Interval(1.5,7));
  success &= compare(test_tube4_05.timeIntegration(Interval(9,21)), Interval(6,36.5));

  return success;
}

bool test_from_0_partial_integration()
{
  bool success = true;

  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.partialTimeIntegration(Interval(0)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(0.2)), make_pair(Interval(0.2), Interval(0.4)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(0.1)), make_pair(Interval(0.1), Interval(0.2)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(0.9)), make_pair(Interval(0.9), Interval(1.8)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(1.0)), make_pair(Interval(1), Interval(2)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(1.2)), make_pair(Interval(1.2), Interval(2.4)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(2.0)), make_pair(Interval(2.0), Interval(4.0)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(3.0)), make_pair(Interval(3.0), Interval(6.0)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(5.0)), make_pair(Interval(5.0), Interval(10.0)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(7.0)), make_pair(Interval(7.0), Interval(14.0)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(9.0)), make_pair(Interval(9.0), Interval(18.0)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(10.0)), make_pair(Interval(9.5), Interval(19.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(10.1)), make_pair(Interval(9.4), Interval(19.6)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(11.0)), make_pair(Interval(8.5), Interval(20.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(12.5)), make_pair(Interval(6.5), Interval(20.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(12.6)), make_pair(Interval(6.4), Interval(20.6)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(14.5)), make_pair(Interval(7), Interval(23.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(12.5,13)), make_pair(Interval(6,6.5), Interval(20.5,21)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(13,14)), make_pair(Interval(6,6.5), Interval(21,22.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(14,14.5)), make_pair(Interval(6.5,7), Interval(22.5,23.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(12.5,14.5)), make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(1.5,3.5)), make_pair(Interval(1.5,3.5), Interval(3,7)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(9,21)), make_pair(Interval(6,13.5), Interval(18,36.5)));

  Tube test_tube4_05 = tubeTest4_05();
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(0)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(0.2)), make_pair(Interval(0.2), Interval(0.4)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(0.1)), make_pair(Interval(0.1), Interval(0.2)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(0.9)), make_pair(Interval(0.9), Interval(1.8)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(1.0)), make_pair(Interval(1), Interval(2)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(1.2)), make_pair(Interval(1.2), Interval(2.4)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(2.0)), make_pair(Interval(2.0), Interval(4.0)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(3.0)), make_pair(Interval(3.0), Interval(6.0)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(5.0)), make_pair(Interval(5.0), Interval(10.0)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(7.0)), make_pair(Interval(7.0), Interval(14.0)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(9.0)), make_pair(Interval(9.0), Interval(18.0)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(10.0)), make_pair(Interval(9.5), Interval(19.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(10.1)), make_pair(Interval(9.4), Interval(19.6)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(11.0)), make_pair(Interval(8.5), Interval(20.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(12.5)), make_pair(Interval(6.5), Interval(20.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(12.6)), make_pair(Interval(6.4), Interval(20.6)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(14.5)), make_pair(Interval(7), Interval(23.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(12.5,13)), make_pair(Interval(6,6.5), Interval(20.5,21)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(13,14)), make_pair(Interval(6,6.5), Interval(21,22.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(14,14.5)), make_pair(Interval(6.5,7), Interval(22.5,23.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(12.5,14.5)), make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(1.5,3.5)), make_pair(Interval(1.5,3.5), Interval(3,7)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(9,21)), make_pair(Interval(6,13.5), Interval(18,36.5)));

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.partialTimeIntegration(Interval(0.,46.)), make_pair(Interval(-85,7), Interval(-16,194)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(7.)), make_pair(Interval(-23), Interval(13)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(0.,7.)), make_pair(Interval(-23,7.0), Interval(0,25)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(2.,6.)), make_pair(Interval(-13,7), Interval(15,25)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(6.,7.)), make_pair(Interval(-23,-13), Interval(13,19)));

  Interval integrale_lb = Interval::EMPTY_SET;
  Interval integrale_ub = Interval::EMPTY_SET;
  for(double t = 0.0 ; t <= 46.0 ; t += 0.2)
  {
    integrale_lb |= test_tube1.partialTimeIntegration(Interval(0, t)).first;
    integrale_ub |= test_tube1.partialTimeIntegration(Interval(0, t)).second;
  }
  success &= integrale_lb.is_subset(Interval(-85,7));
  success &= integrale_ub.is_subset(Interval(-16,194));

  Tube test_tube1_01 = tubeTest1_01();
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0.,46.)), make_pair(Interval(-85,7), Interval(-16,194)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(7.)), make_pair(Interval(-23), Interval(13)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0.,7.)), make_pair(Interval(-23,7.0), Interval(0,25)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(2.,6.)), make_pair(Interval(-13,7), Interval(15,25)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(6.,7.)), make_pair(Interval(-23,-13), Interval(13,19)));

  return success;
}

bool test_integration()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.timeIntegration(Interval(20), Interval(20)), Interval(0.));
  success &= compare(test_tube1.timeIntegration(Interval(2.1), Interval(2.1)), Interval(0.));
  success &= compare(test_tube1.timeIntegration(Interval(12.2), Interval(12.2)), Interval(0.));
  success &= compare(test_tube1.timeIntegration(Interval(12.2), Interval(12.2)), Interval(0.));
  success &= compare(test_tube1.timeIntegration(Interval(17.), Interval(20.)), Interval(12,24));
  success &= compare(test_tube1.timeIntegration(Interval(28.), Interval(33.)), Interval(17,31));
  success &= compare(test_tube1.timeIntegration(Interval(20.), Interval(28.)), Interval(51,88));
  success &= compare(test_tube1.timeIntegration(Interval(17.), Interval(33.)), Interval(80,143));
  success &= compare(test_tube1.timeIntegration(Interval(22.5), Interval(23)), Interval(4.5,6));
  success &= compare(test_tube1.timeIntegration(Interval(8.5), Interval(10.5)), Interval(-18,-9.5));
  success &= compare(test_tube1.timeIntegration(Interval(0), Interval(7)), Interval(-23,13));
  success &= compare(test_tube1.timeIntegration(Interval(2), Interval(6)), Interval(-19,4));
  success &= compare(test_tube1.timeIntegration(Interval(0,2), Interval(6,7)), Interval(-29,19));
  success &= compare(test_tube1.timeIntegration(Interval(0), Interval(17)), Interval(-85,-4));
  success &= compare(test_tube1.timeIntegration(Interval(0), Interval(38)), Interval(1,161));
  success &= compare(test_tube1.timeIntegration(Interval(0), Interval(17,38)), Interval(-85,161));
  success &= compare(test_tube1.timeIntegration(Interval(10), Interval(13)), Interval(-21,-11));
  success &= compare(test_tube1.timeIntegration(Interval(17), Interval(20)), Interval(12,24));
  success &= compare(test_tube1.timeIntegration(Interval(13), Interval(17)), Interval(-11,11));
  success &= compare(test_tube1.timeIntegration(Interval(10,13), Interval(17)), Interval(-32,11));
  success &= compare(test_tube1.timeIntegration(Interval(13), Interval(17,20)), Interval(-11,35));
  success &= compare(test_tube1.timeIntegration(Interval(10,13), Interval(17,20)), Interval(-32,35));
  success &= compare(test_tube1.timeIntegration(Interval(5,10), Interval(31,42)), Interval(-6,180));
  
  Tube test_tube1_01 = tubeTest1_01();
  success &= compare(test_tube1_01.timeIntegration(Interval(20), Interval(20)), Interval(0.));
  success &= compare(test_tube1_01.timeIntegration(Interval(2.1), Interval(2.1)), Interval(0.));
  success &= compare(test_tube1_01.timeIntegration(Interval(12.2), Interval(12.2)), Interval(0.));
  success &= compare(test_tube1_01.timeIntegration(Interval(12.2), Interval(12.2)), Interval(0.));
  success &= compare(test_tube1_01.timeIntegration(Interval(17.), Interval(20.)), Interval(12,24));
  success &= compare(test_tube1_01.timeIntegration(Interval(28.), Interval(33.)), Interval(17,31));
  success &= compare(test_tube1_01.timeIntegration(Interval(20.), Interval(28.)), Interval(51,88));
  success &= compare(test_tube1_01.timeIntegration(Interval(17.), Interval(33.)), Interval(80,143));
  success &= compare(test_tube1_01.timeIntegration(Interval(22.5), Interval(23)), Interval(4.5,6));
  success &= compare(test_tube1_01.timeIntegration(Interval(8.5), Interval(10.5)), Interval(-18,-9.5));
  success &= compare(test_tube1_01.timeIntegration(Interval(0), Interval(7)), Interval(-23,13));
  success &= compare(test_tube1_01.timeIntegration(Interval(2), Interval(6)), Interval(-19,4));
  success &= compare(test_tube1_01.timeIntegration(Interval(0,2), Interval(6,7)), Interval(-29,19));
  success &= compare(test_tube1_01.timeIntegration(Interval(0), Interval(17)), Interval(-85,-4));
  success &= compare(test_tube1_01.timeIntegration(Interval(0), Interval(38)), Interval(1,161));
  success &= compare(test_tube1_01.timeIntegration(Interval(0), Interval(17,38)), Interval(-85,161));
  success &= compare(test_tube1_01.timeIntegration(Interval(10), Interval(13)), Interval(-21,-11));
  success &= compare(test_tube1_01.timeIntegration(Interval(17), Interval(20)), Interval(12,24));
  success &= compare(test_tube1_01.timeIntegration(Interval(13), Interval(17)), Interval(-11,11));
  success &= compare(test_tube1_01.timeIntegration(Interval(10,13), Interval(17)), Interval(-32,11));
  success &= compare(test_tube1_01.timeIntegration(Interval(13), Interval(17,20)), Interval(-11,35));
  success &= compare(test_tube1_01.timeIntegration(Interval(10,13), Interval(17,20)), Interval(-32,35));
  success &= compare(test_tube1_01.timeIntegration(Interval(5,10), Interval(31,42)), Interval(-6,180));

  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.timeIntegration(Interval(0,8), Interval(15,21)), Interval(-0.5,36.5));
  success &= compare(test_tube4.timeIntegration(Interval(0,8), Interval(9,21)), Interval(-2,36.5));

  Tube test_tube4_05 = tubeTest4_05();
  success &= compare(test_tube4_05.timeIntegration(Interval(0,8), Interval(15,21)), Interval(-0.5,36.5));
  success &= compare(test_tube4_05.timeIntegration(Interval(0,8), Interval(9,21)), Interval(-2,36.5));

  return success;
}

bool test_partial_integration()
{
  bool success = true;

  Tube test_tube1 = tubeTest1();
  test_tube1.setY(Interval(-4,2), 14); // to test primitives pre-computation
  success &= compare(test_tube1.partialTimeIntegration(Interval(0), Interval(0)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(20), Interval(20)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(22,23), Interval(24,25)), make_pair(Interval(8,24), Interval(13,37)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(17,20), Interval(28,33)), make_pair(Interval(51,80), Interval(88,143)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(0.), Interval(46.)), make_pair(Interval(-3), Interval(194)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(0.), Interval(7.)), make_pair(Interval(-23), Interval(13)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(2.), Interval(6.)), make_pair(Interval(-19), Interval(4)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(0,2), Interval(6,7)), make_pair(Interval(-29,-13), Interval(-2,19)));
  success &= compare(test_tube1.partialTimeIntegration(Interval(10,13), Interval(17,20)), make_pair(Interval(-32,1), Interval(0,35)));

  Tube test_tube1_01 = tubeTest1_01();
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0), Interval(0)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(20), Interval(20)), make_pair(Interval(0.), Interval(0.)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(22,23), Interval(24,25)), make_pair(Interval(8,24), Interval(13,37)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(17,20), Interval(28,33)), make_pair(Interval(51,80), Interval(88,143)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0.), Interval(46.)), make_pair(Interval(-3), Interval(194)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0.), Interval(7.)), make_pair(Interval(-23), Interval(13)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(2.), Interval(6.)), make_pair(Interval(-19), Interval(4)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(0,2), Interval(6,7)), make_pair(Interval(-29,-13), Interval(-2,19)));
  success &= compare(test_tube1_01.partialTimeIntegration(Interval(10,13), Interval(17,20)), make_pair(Interval(-32,1), Interval(0,35)));

  Tube test_tube4 = tubeTest4();
  success &= compare(test_tube4.partialTimeIntegration(Interval(0.1,1.1), Interval(2.6,3.2)), make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
  success &= compare(test_tube4.partialTimeIntegration(Interval(8.6,9.9), Interval(13.2,13.6)), make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));

  Tube test_tube4_05 = tubeTest4_05();
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(0.1,1.1), Interval(2.6,3.2)), make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
  success &= compare(test_tube4_05.partialTimeIntegration(Interval(8.6,9.9), Interval(13.2,13.6)), make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));

  return success;
}