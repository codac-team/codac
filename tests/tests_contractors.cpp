#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Contracting tube: ctcFwd(xdot)", "[ctc]")
{
  SECTION("Test ctcFwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-0.5, 1.));
    tube.set(Interval(-1.,1.), 0);

    Tube tube_test_fwd(tube);
    tube_test_fwd.ctcFwd(tubedot);

    Tube tube_test_fwdbwd(tube);
    tube_test_fwdbwd.ctcFwdBwd(tubedot);

    CHECK(tube_test_fwd == tube_test_fwdbwd);
    CHECK(tube_test_fwd[0] == Interval(-1.,1.));
    CHECK(tube_test_fwd[1] == Interval(-1.5,2.));
    CHECK(tube_test_fwd[2] == Interval(-2.,3.));
    CHECK(tube_test_fwd[3] == Interval(-2.5,4.));
    CHECK(tube_test_fwd[4] == Interval(-3.,5.));
    CHECK(tube_test_fwd[5] == Interval(-3.5,6.));
  }
}

TEST_CASE("Contracting tube: ctcBwd(xdot)", "[ctc]")
{
  SECTION("Test ctcBwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1., 0.5));
    tube.set(Interval(-1.,1.), 5);

    Tube tube_test_bwd(tube);
    tube_test_bwd.ctcBwd(tubedot);

    Tube tube_test_fwdbwd(tube);
    tube_test_fwdbwd.ctcFwdBwd(tubedot);

    CHECK(tube_test_bwd == tube_test_fwdbwd);
    CHECK(tube_test_bwd[0] == Interval(-3.5,6.));
    CHECK(tube_test_bwd[1] == Interval(-3.,5.));
    CHECK(tube_test_bwd[2] == Interval(-2.5,4.));
    CHECK(tube_test_bwd[3] == Interval(-2.,3.));
    CHECK(tube_test_bwd[4] == Interval(-1.5,2.));
    CHECK(tube_test_bwd[5] == Interval(-1.,1.));
  }
}

TEST_CASE("Contracting tube: ctcFwdBwd(xdot)", "[ctc]")
{
  SECTION("Test ctcFwdBwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1., 0.5));
    tube.set(Interval(-1.,1.), 5);
    tube.set(Interval(-1.,1.), 0);

    tube.ctcFwdBwd(tubedot);

    CHECK(tube[0] == Interval(-1.,1.));
    CHECK(tube[1] == Interval(-2,1.5));
    CHECK(tube[2] == Interval(-2.5,2.));
    CHECK(tube[3] == Interval(-2.,2.5));
    CHECK(tube[4] == Interval(-1.5,2.));
    CHECK(tube[5] == Interval(-1.,1.));
  }
}