#include "catch_interval.hpp"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

// todo: test TrajectoryVector

TEST_CASE("Trajectory base")
{
  SECTION("Evaluation")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj1(map_values);
    // todo: find a way to catch assert abort: CHECK_THROWS(traj1(-0.1););
    CHECK(Approx(traj1(0.0)) == 0.0);
    CHECK(Approx(traj1(0.2)) == 0.2);
    CHECK(Approx(traj1(8.2)) == 8.2);
    CHECK(Approx(traj1(9.0)) == 9.0);
    // todo: find a way to catch assert abort: CHECK_THROWS(traj1(9.1););

    Trajectory traj2;
    traj2.set(1., 0.);
    traj2.set(-1., 2.);
    traj2.set(1., 6.);
    traj2.set(2., 7.);
    traj2.set(4., 8.);
    traj2.set(4., 10.);
    traj2.set(-2., 11.);
    traj2.set(1., 14.);
    CHECK(traj2.tdomain() == Interval(0.,14.));
    CHECK(traj2.codomain() == Interval(-2.,4.));
    // todo: find a way to catch assert abort: CHECK_THROWS(traj2(-0.1););
    CHECK(traj2(0.) == 1.);
    CHECK(traj2(1.) == 0.);
    CHECK(traj2(4.) == 0.);
    CHECK(traj2(9.) == 4.);
    CHECK(traj2(13.) == 0.);
    CHECK(traj2(14.) == 1.);
    CHECK(traj2(Interval(2.,9.)) == Interval(-1.,4.));
    CHECK(traj2(Interval(3.,12.)) == Interval(-2.,4.));
    CHECK(traj2(traj2.tdomain()) == Interval(-2.,4.));
  }

  SECTION("Update")
  {
    Trajectory traj;
    traj.set(3.,-4.);
    CHECK(ApproxIntv(traj.tdomain()) == Interval(-4.));
    traj.set(4.,-2.);
    CHECK(ApproxIntv(traj.tdomain()) == Interval(-4.,-2.));
    // todo: find a way to catch assert abort: CHECK_THROWS(traj(-5.););
    CHECK(Approx(traj(-3.)) == 3.5);
  }

  SECTION("Domain")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj1(map_values);

    CHECK(ApproxIntv(traj1.tdomain()) == Interval(0.,9.));
    CHECK(ApproxIntv(traj1.codomain()) == Interval(0.,9.));
    traj1.truncate_tdomain(Interval(1.,5.));
    CHECK(ApproxIntv(traj1.tdomain()) == Interval(1.,5.));
    CHECK(ApproxIntv(traj1.codomain()) == Interval(1.,5.));

    Trajectory traj2(Interval(-1.,10.), TFunction("t^2"));
    CHECK(traj2.tdomain() == Interval(-1.,10.));
    CHECK(traj2.codomain() == Interval(0.,100.));
    CHECK(Approx(traj2(5.3)) == 28.09);
  }

  SECTION("Domain shift")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);

    CHECK(ApproxIntv(traj.tdomain()) == Interval(0.,9.));
    CHECK(Approx(traj(5.2)) == 5.2);
    traj.shift_tdomain(-2.);
    CHECK(ApproxIntv(traj.tdomain()) == Interval(-2.,7.));
    CHECK(Approx(traj(5.2)) == 7.2);
    CHECK(Approx(traj(3.2)) == 5.2);
  }

  SECTION("Tests")
  {
    // Defined by maps of values
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj1(map_values);
    Trajectory traj2(map_values);
    CHECK(traj1 == traj2);
    CHECK(traj1.tdomain() == Interval(0.,9.));
    CHECK_FALSE(traj1 != traj2);

    traj1.set(4.5, 3.2);
    CHECK(traj1.tdomain() == Interval(0.,9.));
    CHECK_FALSE(traj1 == traj2);
    CHECK(traj1 != traj2);

    traj1 = traj2;
    CHECK(traj1.tdomain() == Interval(0.,9.));
    CHECK(traj1 == traj2);
    CHECK_FALSE(traj1 != traj2);

    traj1.set(4.4, 0.);
    CHECK(traj1.tdomain() == Interval(0.,9.));
    CHECK_FALSE(traj1 == traj2);
    CHECK(traj1 != traj2);

    // Not defined yet
    Trajectory traj3, traj4;
    CHECK(traj3 == traj4);
    CHECK_FALSE(traj3 != traj4);

    // Defined by a TFunction object
    Trajectory traj5(Interval(0.,10.), TFunction("t^2"));
    Trajectory traj6(Interval(0.,10.), TFunction("t^2+1"));
    // todo: find a way to catch assert abort: CHECK_THROWS(traj5 == traj6); // not implemented yet
    // todo: find a way to catch assert abort: CHECK_THROWS(traj5 != traj6); // not implemented yet
  }

  SECTION("First and last values")
  {
    // Defined by maps of values
    map<double,double> map_values;
    for(double t = 0. ; t <= 10. ; t++)
      map_values[t] = t;
    Trajectory test(map_values);

    CHECK(test.codomain() == Interval(0.,10.));
    CHECK(test.first_value() == 0.);
    CHECK(test.last_value() == 10.);
  }

  SECTION("Trajectory vector")
  {
    // Defined by maps of values
    map<double,Vector> map_values;
    for(double t = 0. ; t <= 10. ; t++)
      map_values.insert(make_pair(t, Vector(4,t)));
    TrajectoryVector test(map_values);

    CHECK(test.codomain() == IntervalVector(4,Interval(0.,10.)));
    CHECK(test.first_value() == Vector(4,0.));
    CHECK(test.last_value() == Vector(4,10.));
    CHECK(test.size() == 4);
  }

  SECTION("Constructor with list")
  {
    // Defined by maps of values

    map<double,Vector> vector_map_values;
    for(double t = 0. ; t <= 10. ; t++)
      vector_map_values.insert(make_pair(t, Vector(4,t)));
    TrajectoryVector test1(vector_map_values);
    
    map<double,double> scalar_map_values;
    for(double t = 0. ; t <= 10. ; t++)
      scalar_map_values.insert(make_pair(t, t));

    TrajectoryVector test2({
      Trajectory(scalar_map_values),
      Trajectory(scalar_map_values),
      Trajectory(scalar_map_values),
      Trajectory(scalar_map_values)
    });

    CHECK(test1 == test2);
    CHECK(test1[0] == test2[0]);
  }
}