/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: tubes_examples.cpp
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

Tube tubeTest1()
{
  Tube tube(Interval(0.,46.), 1.);
  tube.setY(Interval(4,8), 0);
  tube.setY(Interval(2,7), 1);
  tube.setY(Interval(1,6), 2);
  tube.setY(Interval(-4,4), 3.2); // double
  tube.setY(Interval(-7,-1), 4);
  tube.setY(Interval(-9,-5), 5.9); // double
  tube.setY(Interval(-10,-6), 6);
  tube.setY(Interval(-11,-7), 7);
  tube.setY(Interval(-10,-6), 8);
  tube.setY(Interval(-9,-4), 9);
  tube.setY(Interval(-8,-5), 10);
  tube.setY(Interval(-7,-4), 11);
  tube.setY(Interval(-6,-2), 12);
  tube.setY(Interval(-5,-1), 13);
  tube.setY(Interval(-5,3), 14); // to be updated
  tube.setY(Interval(-2,4), 15);
  tube.setY(Interval(0,6), 16);
  tube.setY(Interval(2,7), 17);
  tube.setY(Interval(4,8), 18);
  tube.setY(Interval(6,9), 19);
  tube.setY(Interval(7,10), 20);
  tube.setY(Interval(8,11), 21);
  tube.setY(Interval(9,12), 22);
  tube.setY(Interval(8,13), 23);
  tube.setY(Interval(7,12), 24);
  tube.setY(Interval(5,11), 25);
  tube.setY(Interval(3,10), 26);
  tube.setY(Interval(4,9), 27);
  tube.setY(Interval(5,8), 28);
  tube.setY(Interval(4,7), 29);
  tube.setY(Interval(3,6), 30);
  tube.setY(Interval(3,5), 31);
  tube.setY(Interval(2,5), 32);
  tube.setY(Interval(2,5), 33);
  tube.setY(Interval(1,5), 34);
  tube.setY(Interval(2,4), 35);
  tube.setY(Interval(1,4), 36);
  tube.setY(Interval(0,4), 37);
  tube.setY(Interval(-1,3), 38);
  tube.setY(Interval(-1,3), 39);
  tube.setY(Interval(-1,4), 40);
  tube.setY(Interval(0,5), 41);
  tube.setY(Interval(1,6), 42);
  tube.setY(Interval(0,5), 43);
  tube.setY(Interval(-1,4), 44);
  tube.setY(Interval(-1,3), 45);
  return tube;
}

Tube tubeTest1_01()
{
  Tube tube(Interval(0.,46.), 0.5);
  tube.setY(Interval(4,8), Interval(0,1));
  tube.setY(Interval(2,7), Interval(1,2));
  tube.setY(Interval(1,6), Interval(2,3));
  tube.setY(Interval(-4,4), Interval(3,4));
  tube.setY(Interval(-7,-1), Interval(4,5));
  tube.setY(Interval(-9,-5), Interval(5,6));
  tube.setY(Interval(-10,-6), Interval(6,7));
  tube.setY(Interval(-11,-7), Interval(7,8));
  tube.setY(Interval(-10,-6), Interval(8,9));
  tube.setY(Interval(-9,-4), Interval(9,10));
  tube.setY(Interval(-8,-5), Interval(10,11));
  tube.setY(Interval(-7,-4), Interval(11,12));
  tube.setY(Interval(-6,-2), Interval(12,13));
  tube.setY(Interval(-5,-1), Interval(13,14));
  tube.setY(Interval(-4,2), Interval(14,15));
  tube.setY(Interval(-2,4), Interval(15,16));
  tube.setY(Interval(0,6), Interval(16,17));
  tube.setY(Interval(2,7), Interval(17,18));
  tube.setY(Interval(4,8), Interval(18,19));
  tube.setY(Interval(6,9), Interval(19,20));
  tube.setY(Interval(7,10), Interval(20,21));
  tube.setY(Interval(8,11), Interval(21,22));
  tube.setY(Interval(9,12), Interval(22,23));
  tube.setY(Interval(8,13), Interval(23,24));
  tube.setY(Interval(7,12), Interval(24,25));
  tube.setY(Interval(5,11), Interval(25,26));
  tube.setY(Interval(3,10), Interval(26,27));
  tube.setY(Interval(4,9), Interval(27,28));
  tube.setY(Interval(5,8), Interval(28,29));
  tube.setY(Interval(4,7), Interval(29,30));
  tube.setY(Interval(3,6), Interval(30,31));
  tube.setY(Interval(3,5), Interval(31,32));
  tube.setY(Interval(2,5), Interval(32,33));
  tube.setY(Interval(2,5), Interval(33,34));
  tube.setY(Interval(1,5), Interval(34,35));
  tube.setY(Interval(2,4), Interval(35,36));
  tube.setY(Interval(1,4), Interval(36,37));
  tube.setY(Interval(0,4), Interval(37,38));
  tube.setY(Interval(-1,3), Interval(38,39));
  tube.setY(Interval(-1,3), Interval(39,40));
  tube.setY(Interval(-1,4), Interval(40,41));
  tube.setY(Interval(0,5), Interval(41,42));
  tube.setY(Interval(1,6), Interval(42,43));
  tube.setY(Interval(0,5), Interval(43,44));
  tube.setY(Interval(-1,4), Interval(44,45));
  tube.setY(Interval(-1,3), Interval(45,46));
  return tube;
}

Tube tubeTest2()
{
  Tube tube(Interval(0.,46.), 1.);
  tube.setY(Interval(-2,0), 0);
  tube.setY(Interval(-3,1), 1);
  tube.setY(Interval(-1,3), 2);
  tube.setY(Interval(2,4), 3);
  tube.setY(Interval(3,5), 4);
  tube.setY(Interval(2,6), 5);
  tube.setY(Interval(2,5), 6);
  tube.setY(Interval(1,4), 7);
  tube.setY(Interval(1,3), 8);
  tube.setY(Interval(2,3), 9);
  tube.setY(Interval(2,4), 10);
  tube.setY(Interval(1,3), 11);
  tube.setY(Interval(0,2), 12);
  tube.setY(Interval(-1,2), 13);
  tube.setY(Interval(-2,1), 14);
  tube.setY(Interval(-3,0), 15);
  tube.setY(Interval(-2,0), 16);
  tube.setY(Interval(-1,1), 17);
  tube.setY(Interval(0,2), 18);
  tube.setY(Interval(1,3), 19);
  tube.setY(Interval(1,4), 20);
  tube.setY(Interval(2,5), 21);
  tube.setY(Interval(2,4), 22);
  tube.setY(Interval(1,3), 23);
  tube.setY(Interval(0,2), 24);
  tube.setY(Interval(0,2), 25);
  tube.setY(Interval(0,2), 26);
  tube.setY(Interval(1,2), 27);
  tube.setY(Interval(1,3), 28);
  tube.setY(Interval(0,3), 29);
  tube.setY(Interval(-1,2), 30);
  tube.setY(Interval(-2,1), 31);
  tube.setY(Interval(-3,0), 32);
  tube.setY(Interval(-4,-1), 33);
  tube.setY(Interval(-4,-2), 34);
  tube.setY(Interval(-3,-1), 35);
  tube.setY(Interval(-4,-2), 36);
  tube.setY(Interval(-4,-2), 37);
  tube.setY(Interval(-4,-2), 38);
  tube.setY(Interval(-3,-1), 39);
  tube.setY(Interval(-3,-1), 40);
  tube.setY(Interval(-2,-1), 41);
  tube.setY(Interval(-1,1), 42);
  tube.setY(Interval(-1,1), 43);
  tube.setY(Interval(0,2), 44);
  tube.setY(Interval(1,3), 45);
  return tube;
}

Tube tubeTest3()
{
  Tube tube(Interval(0.,5.), 1.);
  tube.setY(Interval(1,3), 0);
  tube.setY(Interval(0,2), 1);
  tube.setY(Interval(-1,1), 2);
  tube.setY(Interval(-2,0), 3);
  tube.setY(Interval(-3,-1), 4);
  return tube;
}

Tube tubeTest4()
{
  Tube tube(Interval(0.,21.), 1.);
  tube.setY(Interval(1,2), Interval(0,9));
  tube.setY(Interval(0.5,1.5), Interval(9,10));
  tube.setY(Interval(-1,1), Interval(10.2));
  tube.setY(Interval(-1.5,-0.5), Interval(11,12));
  tube.setY(Interval(-1,1), Interval(12,13));
  tube.setY(Interval(0.5,1.5), Interval(13,14));
  tube.setY(Interval(1,2), Interval(14,21));  
  return tube;
}

Tube tubeTest4_05()
{
  Tube tube(Interval(0.,21.), 0.5);
  tube.setY(Interval(1,2), Interval(0,9));
  tube.setY(Interval(0.5,1.5), Interval(9,10));
  tube.setY(Interval(-1,1), Interval(10,11));
  tube.setY(Interval(-1.5,-0.5), Interval(11,12));
  tube.setY(Interval(-1,1), Interval(12,13));
  tube.setY(Interval(0.5,1.5), Interval(13,14));
  tube.setY(Interval(1,2), Interval(14,21));  
  return tube;
}