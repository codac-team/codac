/** 
 *  Codac tests
 *  
 *  Most of these tests come from the IBEX library (Gilles Chabert)
 *  See more: https://ibex-lib.readthedocs.io
 *  They have been revised to fit with Codac (v2)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, (Simon Rohou)
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_Approx.h>
#include <limits>

using namespace std;
using namespace codac2;

TEST_CASE("Interval - tests from IBEX")
{
  CHECK(Interval() == Interval(-oo,oo));
  CHECK(Interval(1.0) == Interval(1.0,1.0));
  CHECK(Interval(-oo) == Interval::empty());
  CHECK(Interval(oo) == Interval::empty());
  CHECK(Interval(1,0) == Interval::empty());
  CHECK(Interval{1.0} == Interval(1.0,1.0));
  CHECK(Interval{1.0, 2.0} == Interval(1.0,2.0));

  CHECK(Interval(-oo,oo) == Interval(-oo,oo));
  CHECK(Interval(-oo,oo) != Interval(0,oo));
  CHECK(Interval(0,1) == Interval(0,1));
  CHECK(Interval(0,2) != Interval(0,1));
  CHECK(Interval(0,2) != Interval(1,3));
  CHECK(Interval(0,oo) == Interval(0,oo));
  CHECK(Interval::empty() == Interval::empty());
  CHECK(Interval(0,0) == Interval(0,0));

  CHECK(Interval({1.}) == Interval(1.));
  CHECK(Interval({2,3}) == Interval(2,3));
  CHECK(Interval({3,2}) == Interval::empty());
  CHECK(Interval({-oo}) == Interval::empty());
  CHECK(Interval({oo}) == Interval::empty());
  CHECK(Interval({oo,0}) == Interval::empty());
  CHECK(Interval({-oo,oo}) == Interval(-oo,oo));
  CHECK(Interval({oo,-oo}) == Interval::empty());

  CHECK(5._i == Interval(5.));
  CHECK(-3._i == Interval(-3.));

  Interval x, y, z;

  x = Interval(0,1); x.set_empty();
  CHECK(x == Interval::empty());
  CHECK(x.is_empty());

  x = Interval(Interval::empty()); x.set_empty();
  CHECK(x == Interval::empty());

  x = Interval(0,1); y = Interval(2,3); z = Interval(0,3);
  CHECK((x|y) == z); CHECK((y|x) == z);
  CHECK((Interval(x)|=y) == z); CHECK((Interval(y)|=x) == z);

  x = Interval(0,1); y = Interval::empty(); z = Interval(0,1);
  CHECK((x|y) == z); CHECK((y|x) == z);
  CHECK((Interval(x)|=y) == z); CHECK((Interval(y)|=x) == z);

  x = Interval(-1); y = Interval(0,oo); z = Interval(-1,oo);
  CHECK((x|y) == z); CHECK((y|x) == z);
  CHECK((Interval(x)|=y) == z); CHECK((Interval(y)|=x) == z);

  x = Interval(0,1); y = Interval(2,3); z = Interval::empty();
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,1); y = Interval::empty(); z = Interval::empty();
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,2); y = Interval(-1,1); z = Interval(0,1);
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,1); y = Interval(0,oo); z = Interval(0,1);
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,1); y = Interval(-oo,0); z = Interval(0);
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,1); y = Interval(1,2); z = Interval(1,1);
  CHECK((x&y) == z); CHECK((y&x) == z);
  CHECK((Interval(x)&=y) == z); CHECK((Interval(y)&=x) == z);

  x = Interval(0,2);

  CHECK(x.lb() == 0);
  CHECK(x.ub() == 2);
  CHECK(x.mid() == 1);
  CHECK(x.rad() == 1);
  CHECK(x.diam() == 2);

  x = Interval(-3,-1);

  CHECK(x.lb() == -3);
  CHECK(x.ub() == -1);
  CHECK(x.mid() == -2);
  CHECK(x.rad() == 1);
  CHECK(x.diam() == 2);

  x = Interval(-3,1);

  CHECK(x.lb() == -3);
  CHECK(x.ub() == 1);
  CHECK(x.mid() == -1);
  CHECK(x.rad() == 2);
  CHECK(x.diam() == 4);

  x = Interval(-oo,0);

  CHECK(x.lb() == -oo);
  CHECK(x.ub() == 0);
  CHECK(x.mid() == -std::numeric_limits<double>::max());
  CHECK(x.rad() == oo);
  CHECK(x.diam() == oo);

  x = Interval(-oo,oo);

  CHECK(x.lb() == -oo);
  CHECK(x.ub() == oo);
  CHECK(x.mid() == 0);
  CHECK(x.rad() == oo);
  CHECK(x.diam() == oo);

  x = Interval(std::numeric_limits<double>::max(),oo);

  CHECK(x.lb() == std::numeric_limits<double>::max());
  CHECK(x.ub() == oo);
  CHECK(x.mid() == std::numeric_limits<double>::max());
  CHECK(x.rad() == oo);
  CHECK(x.diam() == oo);

  x = Interval(-1,1);
  for(size_t i = 0 ; i < 10 ; i++)
    CHECK(x.contains(x.rand()));
  x = Interval(-oo,0);
  for(size_t i = 0 ; i < 10 ; i++)
    CHECK(x.contains(x.rand()));
  x = Interval(0,oo);
  for(size_t i = 0 ; i < 10 ; i++)
    CHECK(x.contains(x.rand()));
  x = Interval(-oo,oo);
  for(size_t i = 0 ; i < 10 ; i++)
    CHECK(x.contains(x.rand()));
  x = Interval::empty();
  CHECK(std::isnan(x.rand()));

  CHECK(Interval(0,1).is_subset(Interval(0,2)));
  CHECK(!Interval(0,1).is_subset(Interval(1,2)));
  CHECK(!Interval(0,1).is_strict_interior_subset(Interval(0,2)));
  CHECK(Interval(-oo,0).is_strict_interior_subset(Interval(-oo,oo)));
  CHECK(Interval::empty().is_strict_interior_subset(Interval(0,1)));
  CHECK(Interval(0,2).is_superset(Interval(0,1)));
  CHECK(!Interval(1,2).is_superset(Interval(0,1)));
  CHECK(!Interval(0,2).is_strict_superset(Interval(0,2)));
  CHECK(Interval(-oo,oo).is_strict_superset(Interval(-oo,0)));
  CHECK(!Interval::empty().is_strict_superset(Interval::empty()));
  CHECK(Interval(0,2).contains(0));
  CHECK(Interval(-oo,oo).contains(oo));
  CHECK(!Interval(0,2).interior_contains(0));
  CHECK(Interval(0,2).interior_contains(1));
  CHECK(!Interval(-oo,0).is_disjoint(Interval(0,oo)));
  CHECK(Interval(0,1).is_disjoint(Interval(2,3)));
  CHECK(Interval(oo).is_empty());
  CHECK(!Interval(0,1).is_empty());
  CHECK(Interval(0).is_degenerated());
  CHECK(Interval::empty().is_degenerated());
  CHECK(!Interval::empty().is_unbounded());
  CHECK(!Interval(0,1).is_unbounded());
  CHECK(Interval(-oo,0).is_unbounded());
  CHECK(Interval(0,oo).is_unbounded());
  CHECK(!Interval::empty().is_bisectable());
  CHECK(!Interval(std::numeric_limits<double>::max(),oo).is_bisectable());
  CHECK(!Interval(-oo,-std::numeric_limits<double>::max()).is_bisectable());
  CHECK(!Interval(0,codac2::next_float(0)).is_bisectable());
  CHECK(Interval(0,codac2::next_float(codac2::next_float(0))).is_bisectable());
  CHECK(Interval(10,codac2::next_float(codac2::next_float(10))).is_bisectable());
  CHECK(Interval(codac2::previous_float(codac2::previous_float(0)),0).is_bisectable());
  CHECK(Interval(codac2::previous_float(codac2::previous_float(10)),10).is_bisectable());
  CHECK(!Interval(10,codac2::next_float(10)).is_bisectable());
  CHECK(!Interval(codac2::previous_float(0),0).is_bisectable());

  CHECK(Interval(0,2).mid() == 1.0);
  CHECK(Interval(-oo,oo).mid() == 0);
  CHECK(Interval(0,oo).mid() == std::numeric_limits<double>::max());
  CHECK(Interval(-oo,0).mid() == -std::numeric_limits<double>::max());
  double d = Interval(std::numeric_limits<double>::max(),oo).mid();
  CHECK((d == std::numeric_limits<double>::max() || d == -std::numeric_limits<double>::max()));
  CHECK(Interval(0).mid() == 0.0);

  x = Interval(2.7629416257693772963,2.7629416257693835135);
  y = x;
  static double m = x.mid();
  Interval x2 = x+m;
  Interval x3 = x2-m;
  CHECK(x3.is_superset(y));

  CHECK(Interval(0).diff(Interval(0)).empty());
  CHECK(Interval(0,0).diff(Interval(1,2)) == std::vector({Interval(0)}));
  CHECK(Interval(0,0).diff(Interval(0,1)).empty());
  CHECK(Interval(0,0).diff(Interval::empty()) == std::vector({Interval(0)}));
  CHECK(Interval::empty().diff(Interval(0.)).empty());
  CHECK(Interval(0,1).diff(Interval(0,1)).empty());
  CHECK(Interval(0,1).diff(Interval::empty()) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,1).diff(Interval(1,2)) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,1).diff(Interval(-1,1)).empty());
  CHECK(Interval(0,1).diff(Interval(0,0)) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,1).diff(Interval(1,1)) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,1).diff(Interval(2,3)) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,3).diff(Interval(1,2)) == std::vector({Interval(0,1),Interval(2,3)})); // order may not be respected
  CHECK(Interval(0,2).diff(Interval(1,4)) == std::vector({Interval(0,1)}));
  CHECK(Interval(0,2).diff(Interval(-1,1)) == std::vector({Interval(1,2)}));
  CHECK(Interval(0,2).diff(Interval(1,1), false) == std::vector({Interval(0,1),Interval(1,2)})); // order may not be respected
  CHECK(Interval(1,1).diff(Interval(0,2), false).empty());

  Interval a = Interval(3,8);
  Interval b = Interval(1,3) & Interval(6,7); // [b] is empty
  Interval c = a+b;
  CHECK(c.is_empty());
}

TEST_CASE("Interval - other tests")
{
  Interval x;

  auto a = x.bisect(0.5);
  CHECK(a.first == Interval(-oo,0));
  CHECK(a.second == Interval(0,oo));

  x = Interval(0,oo);
  a = x.bisect(0.5);
  CHECK(a.first == Interval(0,codac2::previous_float(oo)));
  CHECK(a.second == Interval(codac2::previous_float(oo),oo));

  x = Interval(-oo,0);
  a = x.bisect(0.5);
  CHECK(a.first == Interval(-oo,codac2::next_float(-oo)));
  CHECK(a.second == Interval(codac2::next_float(-oo),0));

  x = Interval(0,10);
  a = x.bisect(0.2);
  CHECK(Approx(a.first,1e-7) == Interval(0,2));
  CHECK(Approx(a.second,1e-7) == Interval(2,10));
}

#if 0
// Tests from the IBEX lib that are not considered in this file:

void TestInterval::distance01() { CHECK(ibex::distance(Interval(0,10), Interval(-5,5)), 5); }
void TestInterval::distance02() { CHECK(ibex::distance(Interval(0,10), Interval(5,10)), 5); }
void TestInterval::distance03() { CHECK(ibex::distance(Interval(0,10), Interval(5,15)), 5); }
void TestInterval::distance04() { CHECK(ibex::distance(Interval(0,10), Interval(1,2)), 8); /* 9 */}
void TestInterval::distance05() { CHECK(ibex::distance(Interval(0,10), Interval(0,10)), 0); }
void TestInterval::distance06() { CHECK(ibex::distance(Interval(0,10), Interval(-10,20)), 10); /* 0 */}
// with infinite bounds
void TestInterval::distance07() { CHECK(ibex::distance(Interval(0,oo), Interval(0,10)), oo); }
void TestInterval::distance08() { CHECK(ibex::distance(Interval(-oo,oo), Interval(0,10)), oo); }
void TestInterval::distance09() { CHECK(ibex::distance(Interval(-oo,oo), Interval(-oo,oo)), 0); }
void TestInterval::distance10() { CHECK(ibex::distance(Interval(-oo,oo), Interval(-oo,0)), oo); }
void TestInterval::distance11() { CHECK(ibex::distance(Interval(-oo,oo), Interval(0,oo)), oo); }
void TestInterval::distance12() { CHECK(ibex::distance(Interval(0,oo), Interval(-oo,1)), oo); }
void TestInterval::distance13() { CHECK(ibex::distance(Interval(0,oo), Interval(0,1)), oo); }
void TestInterval::distance14() { CHECK(ibex::distance(Interval(-oo,0), Interval(-1,oo)), oo); }
void TestInterval::distance15() { CHECK(ibex::distance(Interval(-oo,0), Interval(-oo,-1)), 1); }
void TestInterval::distance15b(){ CHECK(ibex::distance(Interval(0,oo), Interval(1,oo)), 1); }
// with degenerated intervals
void TestInterval::distance16() { CHECK(ibex::distance(Interval(0,10), Interval(5,5)), 5); /* 10 */}
void TestInterval::distance17() { CHECK(ibex::distance(Interval(0,10), Interval(0,0)), 10); }
void TestInterval::distance18() { CHECK(ibex::distance(Interval(0,10), Interval(10,10)), 10); }
void TestInterval::distance19() { CHECK(ibex::distance(Interval(0,0), Interval(0,0)), 0); }
// with empty intervals
void TestInterval::distance20() { CHECK(ibex::distance(Interval(0,10), Interval::empty()), 5); /* 10 */}
void TestInterval::distance21() { CHECK(ibex::distance(Interval(0,0), Interval::empty()), 0); }
void TestInterval::distance22() { CHECK(ibex::distance(Interval(0,10), Interval(11,20)), 11); }
void TestInterval::distance23() { CHECK(ibex::distance(Interval(0,10), Interval(0,5)), 5); }
// rel_distance01
void TestInterval::rel_distance01() { CHECK(Interval(0,10).rel_distance(Interval(-5,5)), 0.5); }
void TestInterval::rel_distance02() { CHECK(Interval(0,10).rel_distance(Interval(5,10)), 0.5); }
void TestInterval::rel_distance03() { CHECK(Interval(0,10).rel_distance(Interval(5,15)), 0.5); }
void TestInterval::rel_distance04() { CHECK(Interval(0,10).rel_distance(Interval(1,2)), 0.8); }
void TestInterval::rel_distance05() { CHECK(Interval(0,10).rel_distance(Interval(0,10)), 0); }
void TestInterval::rel_distance06() { CHECK(Interval(0,10).rel_distance(Interval(-10,20)), 1);/*0*/ }
// with infinite bounds
void TestInterval::rel_distance07() { CHECK(Interval(0,oo).rel_distance(Interval(0,10)), 1); }
void TestInterval::rel_distance08() { CHECK(Interval(-oo,oo).rel_distance(Interval(0,10)), 1); }
void TestInterval::rel_distance09() { CHECK(Interval(-oo,oo).rel_distance(Interval(-oo,oo)), 0); }
void TestInterval::rel_distance10() { CHECK(Interval(-oo,oo).rel_distance(Interval(-oo,0)), 1); }
void TestInterval::rel_distance11() { CHECK(Interval(-oo,oo).rel_distance(Interval(0,oo)), 1); }
void TestInterval::rel_distance12() { CHECK(Interval(0,oo).rel_distance(Interval(-oo,1)), 1); }
void TestInterval::rel_distance13() { CHECK(Interval(0,oo).rel_distance(Interval(0,1)), 1); }
void TestInterval::rel_distance14() { CHECK(Interval(-oo,0).rel_distance(Interval(-1,oo)), 1); }
void TestInterval::rel_distance15() { CHECK(Interval(-oo,0).rel_distance(Interval(-oo,-1)), 0); }
// with degenerated intervals
void TestInterval::rel_distance16() { CHECK(Interval(0,10).rel_distance(Interval(5,5)), 0.5); /*1*/}
void TestInterval::rel_distance17() { CHECK(Interval(0,10).rel_distance(Interval(0,0)), 1); }
void TestInterval::rel_distance18() { CHECK(Interval(0,10).rel_distance(Interval(10,10)), 1); }
void TestInterval::rel_distance19() { CHECK(Interval(0,0).rel_distance(Interval(0,0)), 0); }
void TestInterval::rel_distance20() { CHECK(Interval(1,4).rel_distance(Interval(1.5,3)), 1.0/3.0); }
void TestInterval::rel_distance21() { CHECK(Interval(0,10).rel_distance(Interval(0,5)), 0.5); }

Interval x(0,2);
CHECK_DOUBLES_EQUAL_MESSAGE("mig",0,x.mig(), ERROR);
CHECK_DOUBLES_EQUAL_MESSAGE("mag",2,x.mag(), ERROR);
#endif