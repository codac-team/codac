#include "catch_interval.hpp"
#include "codac_tube_arithmetic.h"
#include "tests_predefined_tubes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("Operators")
{
  SECTION("Test tube1|tube2")
  {
    Tube tube1 = tube_test_1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tube_test2();
    tube1 |= tube2;

    // todo: find a way to catch assert abort: CHECK_THROWS(tube1 |= tube_test4_05(););
    CHECK(tube1.codomain() == Interval(-11,13));
    CHECK(tube1(0) == Interval(-2,8));
    CHECK(tube1(5.5) == Interval(-9,6));
    CHECK(tube1(11) == Interval(-7,3));
    CHECK(tube1(21.3) == Interval(2,11));
    CHECK(tube1(45) == Interval(-1,3));
    CHECK(tube1(Interval(5,10)) == Interval(-11,6));
    CHECK(tube1(Interval(15,20)) == Interval(-3,9));
    CHECK(tube1(Interval(8.2,39.9)) == Interval(-10,13));

    // Gates
    tube1 = tube_test_1();
    tube2 = tube_test2();

    tube1.set(Interval(4.), 0.);
    tube2.set(Interval(-1.,0.), 0.);
    tube1 |= tube2;
    CHECK(tube1(0.) == Interval(-1.,4.));
    CHECK(tube1(0) == Interval(-2.,8.));

    tube1.set(Interval(0.,2.), 40.);
    tube2.set(Interval(-3.), 40.);
    tube1 |= tube2;
    CHECK(tube1(40.) == Interval(-3.,2.));
    CHECK(tube1(40) == Interval(-3.,4.));
  }

  SECTION("Test assignment operators for TubeVector")
  {
    Interval domain(0.,10.);
    TubeVector tube1(domain, 2);
    tube1.set(IntervalVector(2, Interval(0.,1.)));
    
    // Assignments with vector tubes...

    tube1 += TubeVector(domain, IntervalVector(2, Interval(0.,4.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(0.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(0.,5.)));
    
    tube1 -= TubeVector(domain, IntervalVector(2, Interval(0.,4.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-4.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-4.,5.)));
    
    tube1 |= TubeVector(domain, IntervalVector(2, Interval(-10.,11.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-10.,11.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-10.,11.)));
    
    tube1 &= TubeVector(domain, IntervalVector(2, Interval(-1.,1.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-1.,1.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-1.,1.)));
    
    // Now with vector trajectories...

    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    tube1 += TrajectoryVector(domain, TFunction("(2.;2.)"));
    TrajectoryVector traj(domain, TFunction("(2.;2.)"));
    CHECK(traj(0.) == Vector(2, 2.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(1.,3.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(1.,3.)));
    
    tube1 -= TrajectoryVector(domain, TFunction("(4.;4.)"));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-3.,-1.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-3.,-1.)));
    
    tube1 |= TrajectoryVector(domain, TFunction("(11.,11.)"));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-3.,11.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-3.,11.)));
    
    tube1 &= TrajectoryVector(domain, TFunction("(1.;1.)"));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(1.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(1.)));
    
    // Now with boxes...
    
    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    tube1 += IntervalVector(2, Interval(2.,3.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(1.,4.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(1.,4.)));
    
    tube1 -= IntervalVector(2, Interval(2.,3.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-2.,2.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-2.,2.)));
    
    tube1 &= IntervalVector(2, Interval(2.,6.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(2.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(2.)));
    
    tube1 |= IntervalVector(2, Interval(6.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(2.,6.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(2.,6.)));
    
    // Now with scalar tubes...
    
    tube1.set(IntervalVector(2, Interval(-2.,2.)));
    tube1 *= Tube(domain, Interval(1.,2.));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-4.,4.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-4.,4.)));
    
    // Now with scalar trajectories...

    tube1.set(IntervalVector(2, Interval(-3.,-1.)));
    tube1 *= Trajectory(domain, TFunction("(4.)"));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-12.,-4.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-12.,-4.)));
    
    // Now with scalar values...

    tube1.set(IntervalVector(2, Interval(-2.,2.)));
    tube1 *= Interval(1.,2.);
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-4.,4.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-4.,4.)));
  }

  SECTION("Test assignment operators for Tube")
  {
    Interval domain(0.,10.);
    Tube tube1(domain);
    tube1.set(Interval(0.,1.));
    
    // Assignments with scalar tubes...
    
    tube1.set(Interval(-1.,1.));
    tube1 += Tube(domain, Interval(2.,3.));
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 -= Tube(domain, Interval(2.,3.));
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 *= Tube(domain, Interval(1.,2.));
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 &= Tube(domain, Interval(2.,6.));
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 |= Tube(domain, Interval(6.));
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));
    
    tube1 /= Tube(domain, Interval(2.));
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    // Now with scalar trajectories...

    tube1.set(Interval(-1.,1.));
    tube1 += Trajectory(domain, TFunction("(2.)"));
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    tube1 -= Trajectory(domain, TFunction("(4.)"));
    CHECK(tube1.codomain() == Interval(-3.,-1.));
    CHECK(tube1(0.) == Interval(-3.,-1.));
    
    tube1 *= Trajectory(domain, TFunction("(4.)"));
    CHECK(tube1.codomain() == Interval(-12.,-4.));
    CHECK(tube1(0.) == Interval(-12.,-4.));
    
    tube1 |= Trajectory(domain, TFunction("(11.)"));
    CHECK(tube1.codomain() == Interval(-12.,11.));
    CHECK(tube1(0.) == Interval(-12.,11.));
    
    tube1 &= Trajectory(domain, TFunction("(1.)"));
    CHECK(tube1.codomain() == Interval(1.));
    CHECK(tube1(0.) == Interval(1.));
    
    tube1 /= Trajectory(domain, TFunction("(2.)"));
    CHECK(tube1.codomain() == Interval(0.5));
    CHECK(tube1(0.) == Interval(0.5));
    
    // Now with scalar values...

    tube1.set(Interval(-1.,1.));
    tube1 += Interval(2.,3.);
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 -= Interval(2.,3.);
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 *= Interval(1.,2.);
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 &= Interval(2.,6.);
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 |= Interval(6.);
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));
    
    tube1 /= Interval(2.);
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
  }

  SECTION("Test set operators for TubeVector")
  {
    Interval domain(0.,10.);
    TubeVector tube1(domain, 2);
    tube1.set(IntervalVector(2, Interval(0.,1.)));
    
    // Assignments with vector tubes...

    TubeVector first = tube1;

    tube1 = tube1 + TubeVector(domain, IntervalVector(2, Interval(0.,4.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(0.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(0.,5.)));
    
    tube1 = tube1 - TubeVector(domain, IntervalVector(2, Interval(0.,4.)));
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-4.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-4.,5.)));

    tube1 = first;

    tube1 = TubeVector(domain, IntervalVector(2, Interval(0.,4.))) + tube1;
    CHECK(tube1.codomain() == IntervalVector(2, Interval(0.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(0.,5.)));
    
    tube1 = - TubeVector(domain, IntervalVector(2, Interval(0.,4.))) + tube1;
    CHECK(tube1.codomain() == IntervalVector(2, Interval(-4.,5.)));
    CHECK(tube1(0.) == IntervalVector(2, Interval(-4.,5.)));

    TubeVector result = tube1;

    result = tube1 | TubeVector(domain, IntervalVector(2, Interval(-10.,11.)));
    CHECK(result.codomain() == IntervalVector(2, Interval(-10.,11.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-10.,11.)));

    tube1 = result;
    result = tube1 & TubeVector(domain, IntervalVector(2, Interval(-1.,1.)));
    CHECK(result.codomain() == IntervalVector(2, Interval(-1.,1.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-1.,1.)));

    result = tube1;

    result = TubeVector(domain, IntervalVector(2, Interval(-10.,11.))) | tube1;
    CHECK(result.codomain() == IntervalVector(2, Interval(-10.,11.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-10.,11.)));

    tube1 = result;
    result = TubeVector(domain, IntervalVector(2, Interval(-1.,1.))) & tube1;
    CHECK(result.codomain() == IntervalVector(2, Interval(-1.,1.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-1.,1.)));
    
    // Now with vector trajectories...

    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    result = tube1 + TrajectoryVector(domain, TFunction("(2.;2.)"));
    TrajectoryVector traj(domain, TFunction("(2.;2.)"));
    CHECK(traj(0.) == Vector(2, 2.));
    CHECK(result.codomain() == IntervalVector(2, Interval(1.,3.)));
    CHECK(result(0.) == IntervalVector(2, Interval(1.,3.)));

    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    result = TrajectoryVector(domain, TFunction("(2.;2.)")) + tube1;
    CHECK(traj(0.) == Vector(2, 2.));
    CHECK(result.codomain() == IntervalVector(2, Interval(1.,3.)));
    CHECK(result(0.) == IntervalVector(2, Interval(1.,3.)));
    
    tube1 = result;
    result = tube1 - TrajectoryVector(domain, TFunction("(4.;4.)"));
    CHECK(result.codomain() == IntervalVector(2, Interval(-3.,-1.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-3.,-1.)));
    
    tube1 = result;
    result = tube1 | TrajectoryVector(domain, TFunction("(11.,11.)"));
    CHECK(result.codomain() == IntervalVector(2, Interval(-3.,11.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-3.,11.)));
    
    tube1 = result;
    result = tube1 & TrajectoryVector(domain, TFunction("(1.;1.)"));
    CHECK(result.codomain() == IntervalVector(2, Interval(1.)));
    CHECK(result(0.) == IntervalVector(2, Interval(1.)));
    
    // Now with boxes...
    
    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    result = tube1 + IntervalVector(2, Interval(2.,3.));
    CHECK(result.codomain() == IntervalVector(2, Interval(1.,4.)));
    CHECK(result(0.) == IntervalVector(2, Interval(1.,4.)));
    
    tube1.set(IntervalVector(2, Interval(-1.,1.)));
    result = IntervalVector(2, Interval(2.,3.)) + tube1;
    CHECK(result.codomain() == IntervalVector(2, Interval(1.,4.)));
    CHECK(result(0.) == IntervalVector(2, Interval(1.,4.)));
    
    tube1 = result;
    result = tube1 - IntervalVector(2, Interval(2.,3.));
    CHECK(result.codomain() == IntervalVector(2, Interval(-2.,2.)));
    CHECK(result(0.) == IntervalVector(2, Interval(-2.,2.)));
    
    tube1 = result;
    result = tube1 & IntervalVector(2, Interval(2.,6.));
    CHECK(result.codomain() == IntervalVector(2, Interval(2.)));
    CHECK(result(0.) == IntervalVector(2, Interval(2.)));
    
    tube1 = result;
    result = tube1 | IntervalVector(2, Interval(6.));
    CHECK(result.codomain() == IntervalVector(2, Interval(2.,6.)));
    CHECK(result(0.) == IntervalVector(2, Interval(2.,6.)));
    
    // Now with scalar tubes...

    result = Tube(domain, Interval(1.,2.)) * tube1;
    CHECK(result.codomain() == IntervalVector(2, Interval(2.,4.)));
    CHECK(result(0.) == IntervalVector(2, Interval(2.,4.)));
  }

  SECTION("Test set operators for Tube")
  {
    Interval domain(0.,10.);
    Tube tube1(domain);
    tube1.set(Interval(0.,1.));
    
    // Assignments with scalar tubes...
    
    tube1.set(Interval(-1.,1.));
    tube1 = tube1 + Tube(domain, Interval(2.,3.));
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 = tube1 - Tube(domain, Interval(2.,3.));
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 = tube1 * Tube(domain, Interval(1.,2.));
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 = tube1 & Tube(domain, Interval(2.,6.));
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 = tube1 | Tube(domain, Interval(6.));
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));
    
    tube1 = tube1 / Tube(domain, Interval(2.));
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    tube1.set(Interval(-1.,1.));
    tube1 = Tube(domain, Interval(2.,3.)) + tube1;
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 = - Tube(domain, Interval(2.,3.)) + tube1;
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 = Tube(domain, Interval(1.,2.)) * tube1;
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 = Tube(domain, Interval(2.,6.)) & tube1;
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 = Tube(domain, Interval(6.)) | tube1;
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));

    // Now with scalar trajectories...

    tube1.set(Interval(-1.,1.));
    tube1 = tube1 + Trajectory(domain, TFunction("(2.)"));
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    tube1 = tube1 - Trajectory(domain, TFunction("(4.)"));
    CHECK(tube1.codomain() == Interval(-3.,-1.));
    CHECK(tube1(0.) == Interval(-3.,-1.));
    
    // todo ? tube1 = tube1 * Trajectory(domain, TFunction("(4.)"));
    // todo ? CHECK(tube1.codomain() == Interval(-12.,-4.));
    // todo ? CHECK(tube1(0.) == Interval(-12.,-4.));
    tube1.set(Interval(-12.,-4.));

    tube1 = tube1 | Trajectory(domain, TFunction("(11.)"));
    CHECK(tube1.codomain() == Interval(-12.,11.));
    CHECK(tube1(0.) == Interval(-12.,11.));
    
    tube1 = tube1 & Trajectory(domain, TFunction("(1.)"));
    CHECK(tube1.codomain() == Interval(1.));
    CHECK(tube1(0.) == Interval(1.));
    
    tube1 = tube1 / Trajectory(domain, TFunction("(2.)"));
    CHECK(tube1.codomain() == Interval(0.5));
    CHECK(tube1(0.) == Interval(0.5));

    tube1.set(Interval(-1.,1.));
    tube1 = Trajectory(domain, TFunction("(2.)")) + tube1;
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    // todo: tube1 = - Trajectory(domain, TFunction("(4.)")) + tube1;
    // todo: CHECK(tube1.codomain() == Interval(-3.,-1.));
    // todo: CHECK(tube1(0.) == Interval(-3.,-1.));
    tube1.set(Interval(-3.,-1.));
    
    tube1 = Trajectory(domain, TFunction("(4.)")) * tube1;
    CHECK(tube1.codomain() == Interval(-12.,-4.));
    CHECK(tube1(0.) == Interval(-12.,-4.));
    
    tube1 = Trajectory(domain, TFunction("(11.)")) | tube1;
    CHECK(tube1.codomain() == Interval(-12.,11.));
    CHECK(tube1(0.) == Interval(-12.,11.));
    
    tube1 = Trajectory(domain, TFunction("(1.)")) & tube1;
    CHECK(tube1.codomain() == Interval(1.));
    CHECK(tube1(0.) == Interval(1.));
    
    // Now with scalar values...

    tube1.set(Interval(-1.,1.));
    tube1 = tube1 + Interval(2.,3.);
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 = tube1 - Interval(2.,3.);
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 = tube1 * Interval(1.,2.);
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 = tube1 & Interval(2.,6.);
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 = tube1 | Interval(6.);
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));
    
    tube1 = tube1 / Interval(2.);
    CHECK(tube1.codomain() == Interval(1.,3.));
    CHECK(tube1(0.) == Interval(1.,3.));
    
    tube1.set(Interval(-1.,1.));
    tube1 = Interval(2.,3.) + tube1;
    CHECK(tube1.codomain() == Interval(1.,4.));
    CHECK(tube1(0.) == Interval(1.,4.));
    
    tube1 = - Interval(2.,3.) + tube1;
    CHECK(tube1.codomain() == Interval(-2.,2.));
    CHECK(tube1(0.) == Interval(-2.,2.));
    
    tube1 = Interval(1.,2.) * tube1;
    CHECK(tube1.codomain() == Interval(-4.,4.));
    CHECK(tube1(0.) == Interval(-4.,4.));
    
    tube1 = Interval(2.,6.) & tube1;
    CHECK(tube1.codomain() == Interval(2.,4.));
    CHECK(tube1(0.) == Interval(2.,4.));
    
    tube1 = Interval(6.) | tube1;
    CHECK(tube1.codomain() == Interval(2.,6.));
    CHECK(tube1(0.) == Interval(2.,6.));
  }

  SECTION("Test tube1&tube2")
  {
    Tube tube1 = tube_test_1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tube_test2();
    tube1 &= tube2;

    // todo: find a way to catch assert abort: CHECK_THROWS(tube1 &= tube_test4_05(););
    CHECK(tube1.codomain() == Interval(-2,4));
    CHECK(tube1(0) == Interval::EMPTY_SET);
    CHECK(tube1(2.5) == Interval(1,3));
    CHECK(tube1(15.0) == Interval(-2,0));
    CHECK(tube1(44.5) == Interval(0,2));
    CHECK(tube1(39) == Interval(-1));
    CHECK(tube1(Interval(0,6)) == Interval(1,4));
    CHECK(tube1(Interval(11,19)) == Interval(-2,1));
    CHECK(tube1(Interval(39.5,40.3)) == Interval(-1));

    // Gates
    tube1 = tube_test_1();
    tube2 = tube_test2();

    tube1.set(Interval(4.), 0.);
    tube2.set(Interval(-1.,0.), 0.);
    tube1 &= tube2;
    CHECK(tube1(0.) == Interval::EMPTY_SET);
    CHECK(tube1(0) == Interval::EMPTY_SET);

    CHECK(tube1(39.999) == Interval(-1.));
    CHECK(tube1(40.001) == Interval(-1.));
    tube1.set(Interval(-2.,0.), 40.);
    CHECK(tube1(40.) == Interval(-1.));
    tube2.set(Interval(-1.2), 40.);
    tube1 &= tube2;
    CHECK(tube1(40.) == Interval::EMPTY_SET);
  }

  SECTION("Test operation with different slicing")
  {
    Tube tube1(Interval(0.,1.), 0.5, Interval(2.,4.));
    CHECK(tube1.nb_slices() == 2);

    Tube tube2(tube1);
    tube2.set(Interval(1.,2.));

    Tube tube3 = tube1 + tube2;

    CHECK(tube3.codomain() == Interval(3.,6.));
    CHECK(tube3(0) == Interval(3.,6.));

    // New slicing for tube2

    tube2.sample(0.01);
    tube2.set(Interval(1.,4.), 0);
    CHECK(tube1.nb_slices() == 2);
    CHECK(tube2.nb_slices() == 3);
    tube3 = tube1 + tube2;

    CHECK(tube3.codomain() == Interval(3.,8.));
    CHECK(tube3(0) == Interval(3.,8.));
    CHECK(tube3(1) == Interval(3.,6.));
  }
}