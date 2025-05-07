/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcInverseNotIn.h>
#include <codac2_pave.h>
#include <codac2_Figure2D.h>
#include <codac2_Approx.h>
#include <codac2_CtcWrapper.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcInverseNotIn")
{
  // Multivariable functions not supported (yet) for CtcInverseNotIn: ScalarVar x,y;
  // Multivariable functions not supported (yet) for CtcInverseNotIn: AnalyticFunction f { {x,y}, x-y };
  // Multivariable functions not supported (yet) for CtcInverseNotIn: CtcInverseNotIn<Interval> c(f, Interval(0.));

  {
    // Test not really relevant, but copied from CtcInverse:

    VectorVar x(2);
    AnalyticFunction f { {x}, vec(x[0]-x[1]) };
    CtcInverseNotIn<IntervalVector> c(f, {0});

    //pave(IntervalVector({{-10,10},{-10,10}}), c, 0.1);

    IntervalVector b(2);

    c.contract(b);
    CHECK(b == IntervalVector(2));

    b = IntervalVector({{1,2},{1,2}});
    c.contract(b);
    CHECK(b == IntervalVector({{1,2},{1,2}}));

    b = IntervalVector({{1,oo},{1,oo}});
    c.contract(b);
    CHECK(b == IntervalVector({{1,oo},{1,oo}}));

    b = IntervalVector({10,10});
    c.contract(b);
    CHECK(b == IntervalVector({10,10}));

    b = IntervalVector({{1,5},{8,9}});
    c.contract(b);
    CHECK(b == IntervalVector({{1,5},{8,9}}));
  }

  {
    VectorVar x(2);
    AnalyticFunction f { {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
    CtcInverseNotIn<IntervalVector> c(f, {{0,oo},{1}});

    {
      IntervalVector b({{0,0.8},{-2.28,-1.56}});
      c.contract(b);
      CHECK(b == IntervalVector({{0,0.8},{-2.28,-1.56}}));
    }

    {
      IntervalVector b({{4,5.4},{-0.05,2.45}});
      c.contract(b);
      CHECK(Approx(b,1e-2) == IntervalVector({{4,5.4},{-0.05,2.45}}));
    }

    {
      IntervalVector b({{6.25,6.7},{0.9,1.85}});
      c.contract(b);
      CHECK(Approx(b,1e-2) == IntervalVector({{6.25,6.70},{0.9,1.85}}));
    }

    {
      IntervalVector b({{-6,-5},{0,2}});
      c.contract(b);
      CHECK(b == IntervalVector({{-6,-5},{0,2}}));
    }

    {
      IntervalVector b({{2,3},{-1,1}});
      c.contract(b);
      CHECK(b == IntervalVector({{2,3},{-1,1}}));
    }
  }

  // disabled: {
  // disabled:   VectorVar x(2);
  // disabled:   AnalyticFunction f { {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
  // disabled:   CtcInverseNotIn<IntervalVector> c(f, CtcWrapper(IntervalVector({{0,oo},{1,1}})));
  // disabled:   
  // disabled:   {
  // disabled:     IntervalVector b({{0,0.8},{-2.28,-1.56}});
  // disabled:     c.contract(b);
  // disabled:     CHECK(b == IntervalVector({{0,0.8},{-2.28,-1.56}}));
  // disabled:   }
  // disabled:   
  // disabled:   {
  // disabled:     IntervalVector b({{4,5.4},{-0.05,2.45}});
  // disabled:     c.contract(b);
  // disabled:     CHECK(Approx(b,1e-2) == IntervalVector({{4,5.4},{-0.05,2.45}}));
  // disabled:   }
  // disabled:   
  // disabled:   {
  // disabled:     IntervalVector b({{6.25,6.7},{0.9,1.85}});
  // disabled:     c.contract(b);
  // disabled:     CHECK(Approx(b,1e-2) == IntervalVector({{6.25,6.70},{0.9,1.85}}));
  // disabled:   }
  // disabled:   
  // disabled:   {
  // disabled:     IntervalVector b({{-6,-5},{0,2}});
  // disabled:     c.contract(b);
  // disabled:     CHECK(b == IntervalVector({{-6,-5},{0,2}}));
  // disabled:   }
  // disabled:   
  // disabled:   {
  // disabled:     IntervalVector b({{2,3},{-1,1}});
  // disabled:     c.contract(b);
  // disabled:     CHECK(b == IntervalVector({{2,3},{-1,1}}));
  // disabled:   }
  // disabled: }

  {
    ScalarVar v;
    CtcInverseNotIn<Interval,Interval> c(AnalyticFunction({v}, v), Interval(-1,1));
    Interval x;

    c.contract(x);
    CHECK(x == Interval());

    x = Interval(0,8);
    c.contract(x);
    CHECK(x == Interval(1,8));
  }

  {
    VectorVar v(2);
    CtcInverseNotIn c(AnalyticFunction({v}, v), IntervalVector({{-1,1},{5,6}}));
    IntervalVector x(2);

    c.contract(x);
    CHECK(x == IntervalVector(2));

    x = IntervalVector({{0,8},{5.5,oo}});
    c.contract(x);
    CHECK(x == IntervalVector({{0,8},{5.5,oo}})); // unchanged
  }
}