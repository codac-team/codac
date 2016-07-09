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

#include "Tube.h"

Tube tubeTest1()
{
  Tube tube(ibex::Interval(0.,46.), 1.);
  tube.setY(ibex::Interval(4,8), 0);
  tube.setY(ibex::Interval(2,7), 1);
  tube.setY(ibex::Interval(1,6), 2);
  tube.setY(ibex::Interval(-4,4), 3.2); // double
  tube.setY(ibex::Interval(-7,-1), 4);
  tube.setY(ibex::Interval(-9,-5), 5.9); // double
  tube.setY(ibex::Interval(-10,-6), 6);
  tube.setY(ibex::Interval(-11,-7), 7);
  tube.setY(ibex::Interval(-10,-6), 8);
  tube.setY(ibex::Interval(-9,-4), 9);
  tube.setY(ibex::Interval(-8,-5), 10);
  tube.setY(ibex::Interval(-7,-4), 11);
  tube.setY(ibex::Interval(-6,-2), 12);
  tube.setY(ibex::Interval(-5,-1), 13);
  tube.setY(ibex::Interval(-5,3), 14); // to be updated
  tube.setY(ibex::Interval(-2,4), 15);
  tube.setY(ibex::Interval(0,6), 16);
  tube.setY(ibex::Interval(2,7), 17);
  tube.setY(ibex::Interval(4,8), 18);
  tube.setY(ibex::Interval(6,9), 19);
  tube.setY(ibex::Interval(7,10), 20);
  tube.setY(ibex::Interval(8,11), 21);
  tube.setY(ibex::Interval(9,12), 22);
  tube.setY(ibex::Interval(8,13), 23);
  tube.setY(ibex::Interval(7,12), 24);
  tube.setY(ibex::Interval(5,11), 25);
  tube.setY(ibex::Interval(3,10), 26);
  tube.setY(ibex::Interval(4,9), 27);
  tube.setY(ibex::Interval(5,8), 28);
  tube.setY(ibex::Interval(4,7), 29);
  tube.setY(ibex::Interval(3,6), 30);
  tube.setY(ibex::Interval(3,5), 31);
  tube.setY(ibex::Interval(2,5), 32);
  tube.setY(ibex::Interval(2,5), 33);
  tube.setY(ibex::Interval(1,5), 34);
  tube.setY(ibex::Interval(2,4), 35);
  tube.setY(ibex::Interval(1,4), 36);
  tube.setY(ibex::Interval(0,4), 37);
  tube.setY(ibex::Interval(-1,3), 38);
  tube.setY(ibex::Interval(-1,3), 39);
  tube.setY(ibex::Interval(-1,4), 40);
  tube.setY(ibex::Interval(0,5), 41);
  tube.setY(ibex::Interval(1,6), 42);
  tube.setY(ibex::Interval(0,5), 43);
  tube.setY(ibex::Interval(-1,4), 44);
  tube.setY(ibex::Interval(-1,3), 45);
  return tube;
}

Tube tubeTest1_01()
{
  Tube tube(ibex::Interval(0.,46.), 0.5);
  tube.setY(ibex::Interval(4,8), ibex::Interval(0,1));
  tube.setY(ibex::Interval(2,7), ibex::Interval(1,2));
  tube.setY(ibex::Interval(1,6), ibex::Interval(2,3));
  tube.setY(ibex::Interval(-4,4), ibex::Interval(3,4));
  tube.setY(ibex::Interval(-7,-1), ibex::Interval(4,5));
  tube.setY(ibex::Interval(-9,-5), ibex::Interval(5,6));
  tube.setY(ibex::Interval(-10,-6), ibex::Interval(6,7));
  tube.setY(ibex::Interval(-11,-7), ibex::Interval(7,8));
  tube.setY(ibex::Interval(-10,-6), ibex::Interval(8,9));
  tube.setY(ibex::Interval(-9,-4), ibex::Interval(9,10));
  tube.setY(ibex::Interval(-8,-5), ibex::Interval(10,11));
  tube.setY(ibex::Interval(-7,-4), ibex::Interval(11,12));
  tube.setY(ibex::Interval(-6,-2), ibex::Interval(12,13));
  tube.setY(ibex::Interval(-5,-1), ibex::Interval(13,14));
  tube.setY(ibex::Interval(-4,2), ibex::Interval(14,15));
  tube.setY(ibex::Interval(-2,4), ibex::Interval(15,16));
  tube.setY(ibex::Interval(0,6), ibex::Interval(16,17));
  tube.setY(ibex::Interval(2,7), ibex::Interval(17,18));
  tube.setY(ibex::Interval(4,8), ibex::Interval(18,19));
  tube.setY(ibex::Interval(6,9), ibex::Interval(19,20));
  tube.setY(ibex::Interval(7,10), ibex::Interval(20,21));
  tube.setY(ibex::Interval(8,11), ibex::Interval(21,22));
  tube.setY(ibex::Interval(9,12), ibex::Interval(22,23));
  tube.setY(ibex::Interval(8,13), ibex::Interval(23,24));
  tube.setY(ibex::Interval(7,12), ibex::Interval(24,25));
  tube.setY(ibex::Interval(5,11), ibex::Interval(25,26));
  tube.setY(ibex::Interval(3,10), ibex::Interval(26,27));
  tube.setY(ibex::Interval(4,9), ibex::Interval(27,28));
  tube.setY(ibex::Interval(5,8), ibex::Interval(28,29));
  tube.setY(ibex::Interval(4,7), ibex::Interval(29,30));
  tube.setY(ibex::Interval(3,6), ibex::Interval(30,31));
  tube.setY(ibex::Interval(3,5), ibex::Interval(31,32));
  tube.setY(ibex::Interval(2,5), ibex::Interval(32,33));
  tube.setY(ibex::Interval(2,5), ibex::Interval(33,34));
  tube.setY(ibex::Interval(1,5), ibex::Interval(34,35));
  tube.setY(ibex::Interval(2,4), ibex::Interval(35,36));
  tube.setY(ibex::Interval(1,4), ibex::Interval(36,37));
  tube.setY(ibex::Interval(0,4), ibex::Interval(37,38));
  tube.setY(ibex::Interval(-1,3), ibex::Interval(38,39));
  tube.setY(ibex::Interval(-1,3), ibex::Interval(39,40));
  tube.setY(ibex::Interval(-1,4), ibex::Interval(40,41));
  tube.setY(ibex::Interval(0,5), ibex::Interval(41,42));
  tube.setY(ibex::Interval(1,6), ibex::Interval(42,43));
  tube.setY(ibex::Interval(0,5), ibex::Interval(43,44));
  tube.setY(ibex::Interval(-1,4), ibex::Interval(44,45));
  tube.setY(ibex::Interval(-1,3), ibex::Interval(45,46));
  return tube;
}

Tube tubeTest2()
{
  Tube tube(ibex::Interval(0.,46.), 1.);
  tube.setY(ibex::Interval(-2,0), 0);
  tube.setY(ibex::Interval(-3,1), 1);
  tube.setY(ibex::Interval(-1,3), 2);
  tube.setY(ibex::Interval(2,4), 3);
  tube.setY(ibex::Interval(3,5), 4);
  tube.setY(ibex::Interval(2,6), 5);
  tube.setY(ibex::Interval(2,5), 6);
  tube.setY(ibex::Interval(1,4), 7);
  tube.setY(ibex::Interval(1,3), 8);
  tube.setY(ibex::Interval(2,3), 9);
  tube.setY(ibex::Interval(2,4), 10);
  tube.setY(ibex::Interval(1,3), 11);
  tube.setY(ibex::Interval(0,2), 12);
  tube.setY(ibex::Interval(-1,2), 13);
  tube.setY(ibex::Interval(-2,1), 14);
  tube.setY(ibex::Interval(-3,0), 15);
  tube.setY(ibex::Interval(-2,0), 16);
  tube.setY(ibex::Interval(-1,1), 17);
  tube.setY(ibex::Interval(0,2), 18);
  tube.setY(ibex::Interval(1,3), 19);
  tube.setY(ibex::Interval(1,4), 20);
  tube.setY(ibex::Interval(2,5), 21);
  tube.setY(ibex::Interval(2,4), 22);
  tube.setY(ibex::Interval(1,3), 23);
  tube.setY(ibex::Interval(0,2), 24);
  tube.setY(ibex::Interval(0,2), 25);
  tube.setY(ibex::Interval(0,2), 26);
  tube.setY(ibex::Interval(1,2), 27);
  tube.setY(ibex::Interval(1,3), 28);
  tube.setY(ibex::Interval(0,3), 29);
  tube.setY(ibex::Interval(-1,2), 30);
  tube.setY(ibex::Interval(-2,1), 31);
  tube.setY(ibex::Interval(-3,0), 32);
  tube.setY(ibex::Interval(-4,-1), 33);
  tube.setY(ibex::Interval(-4,-2), 34);
  tube.setY(ibex::Interval(-3,-1), 35);
  tube.setY(ibex::Interval(-4,-2), 36);
  tube.setY(ibex::Interval(-4,-2), 37);
  tube.setY(ibex::Interval(-4,-2), 38);
  tube.setY(ibex::Interval(-3,-1), 39);
  tube.setY(ibex::Interval(-3,-1), 40);
  tube.setY(ibex::Interval(-2,-1), 41);
  tube.setY(ibex::Interval(-1,1), 42);
  tube.setY(ibex::Interval(-1,1), 43);
  tube.setY(ibex::Interval(0,2), 44);
  tube.setY(ibex::Interval(1,3), 45);
  return tube;
}

Tube tubeTest3()
{
  Tube tube(ibex::Interval(0.,5.), 1.);
  tube.setY(ibex::Interval(1,3), 0);
  tube.setY(ibex::Interval(0,2), 1);
  tube.setY(ibex::Interval(-1,1), 2);
  tube.setY(ibex::Interval(-2,0), 3);
  tube.setY(ibex::Interval(-3,-1), 4);
  return tube;
}

Tube tubeTest4()
{
  Tube tube(ibex::Interval(0.,21.), 1.);
  tube.setY(ibex::Interval(1,2), ibex::Interval(0,9));
  tube.setY(ibex::Interval(0.5,1.5), ibex::Interval(9,10));
  tube.setY(ibex::Interval(-1,1), ibex::Interval(10.2));
  tube.setY(ibex::Interval(-1.5,-0.5), ibex::Interval(11,12));
  tube.setY(ibex::Interval(-1,1), ibex::Interval(12,13));
  tube.setY(ibex::Interval(0.5,1.5), ibex::Interval(13,14));
  tube.setY(ibex::Interval(1,2), ibex::Interval(14,21));  
  return tube;
}

Tube tubeTest4_05()
{
  Tube tube(ibex::Interval(0.,21.), 0.5);
  tube.setY(ibex::Interval(1,2), ibex::Interval(0,9));
  tube.setY(ibex::Interval(0.5,1.5), ibex::Interval(9,10));
  tube.setY(ibex::Interval(-1,1), ibex::Interval(10,11));
  tube.setY(ibex::Interval(-1.5,-0.5), ibex::Interval(11,12));
  tube.setY(ibex::Interval(-1,1), ibex::Interval(12,13));
  tube.setY(ibex::Interval(0.5,1.5), ibex::Interval(13,14));
  tube.setY(ibex::Interval(1,2), ibex::Interval(14,21));  
  return tube;
}