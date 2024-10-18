#include <cstdio>
#include "catch_interval.hpp"
#include "codac_CtcCartProd.h"
#include "codac_Function.h"
#include "ibex_CtcFwdBwd.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;


TEST_CASE("CtcCartProd")
{
  SECTION("Test CtcCartProd")
  {
    // C1
    Interval y1(0, 1);
    Function f1("x", "sin(x)");
    CtcFwdBwd C1(f1, y1);

    // C2
    Interval y2(0, 1);
    Function f2("x", "sqr(x)");
    CtcFwdBwd C2(f2, y2);

    // CtcCartProd
    CtcCartProd C(C1, C2);

    // Expected
    IntervalVector e1 = {{-5, 1}};
    IntervalVector e2 = {{-3, 6}};
    C1.contract(e1);
    C2.contract(e2);

    // // Testing
    IntervalVector x = {{-5, 1}, {-3, 6}};
    C.contract(x);

    CHECK(x[0] == e1[0]);
    CHECK(x[1] == e2[0]);
  }

  SECTION("Test cart_prod")
  {
    // C1
    Interval y1(0, 1);
    Function f1("x", "sin(x)");
    CtcFwdBwd C1(f1, y1);

    // C2
    Interval y2(0, 1);
    Function f2("x", "sqr(x)");
    CtcFwdBwd C2(f2, y2);

    // CtcCartProd 1
    CtcCartProd Cx1(C1, C2);
    IntervalVector x1 = {{-5, 1}, {-3, 6}};
    Cx1.contract(x1);

    // CtcCartProd 2
    CtcCartProd Cx2 = cart_prod(C1, C2);
    IntervalVector x2 = {{-5, 1}, {-3, 6}};
    Cx2.contract(x2);

    CHECK(x1[0] == x2[0]);
    CHECK(x1[1] == x2[1]);
  }
}

TEST_CASE("cart_prod")
{
  SECTION("Test card_prod")
  {
    // C1
    Interval y1(0, 1);
    Function f1("x", "sin(x)");
    CtcFwdBwd C1(f1, y1);

    // C2
    Interval y2(0, 1);
    Function f2("x", "sqr(x)");
    CtcFwdBwd C2(f2, y2);

    // CtcCartProd
    CtcCartProd C = cart_prod(C1, C2);

    // Expected
    IntervalVector e1 = {{-5, 1}};
    IntervalVector e2 = {{-3, 6}};
    C1.contract(e1);
    C2.contract(e2);

    // // Testing
    IntervalVector x = {{-5, 1}, {-3, 6}};
    C.contract(x);

    CHECK(x[0] == e1[0]);
    CHECK(x[1] == e2[0]);
  }
}