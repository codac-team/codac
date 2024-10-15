/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcInverse.h>
#include <codac2_analytic_operations.h>
#include <codac2_Figure2D.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcInverse")
{
  {
    ScalarVar x,y;
    AnalyticFunction f { {x,y}, x-y };
    CtcInverse<Interval> c(f, Interval(0.));
    CHECK(c.function().input_size() == 2);

    Interval a,b;

    c.contract(a,b);
    CHECK(a == Interval());
    CHECK(b == Interval());

    a = Interval(2,oo);
    b = Interval(2,oo);
    c.contract(a,b);
    CHECK(a == Interval(2,oo));
    CHECK(b == Interval(2,oo));

    a = Interval(1,2);
    b = Interval(1,2);
    c.contract(a,b);
    CHECK(a == Interval(1,2));
    CHECK(b == Interval(1,2));

    a = Interval(10);
    b = Interval(10);
    c.contract(a,b);
    CHECK(a == Interval(10));
    CHECK(b == Interval(10));

    a = Interval(1,5);
    b = Interval(8,9);
    c.contract(a,b);
    CHECK(a == Interval::empty());
    CHECK(b == Interval::empty());

    a = Interval(1,5);
    b = Interval(-oo,oo);
    c.contract(a,b);
    CHECK(a == Interval(1,5));
    CHECK(b == Interval(1,5));
  }

  {
    VectorVar x(2);
    AnalyticFunction f { {x}, x[0]-x[1] };
    CtcInverse_<Interval> c(f, {0});

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

    b = IntervalVector({{10},{10}});
    c.contract(b);
    CHECK(b == IntervalVector({{10},{10}}));

    b = IntervalVector({{1,5},{8,9}});
    c.contract(b);
    CHECK(b == IntervalVector::empty(2));
  }

  {
    VectorVar x(2);
    AnalyticFunction f { {x}, vec(x[0]-x[1]) };
    CtcInverse_<IntervalVector> c(f, {{0}});

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

    b = IntervalVector({{10},{10}});
    c.contract(b);
    CHECK(b == IntervalVector({{10},{10}}));

    b = IntervalVector({{1,5},{8,9}});
    c.contract(b);
    CHECK(b == IntervalVector::empty(2));
  }

  {
    VectorVar x(2);
    AnalyticFunction f { {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
    CtcInverse_<IntervalVector> c(f, {{0,oo},{1}});

    //pave(IntervalVector({{-10,10},{-10,10}}), c, 0.1);

    {
      IntervalVector b({{0,0.8},{-2.28,-1.56}});
      //DefaultView::draw_box(b);
      c.contract(b);
      CHECK(b == IntervalVector::empty(2));
      //DefaultView::draw_box(b,Color::blue());
    }

    {
      IntervalVector b({{4,5.4},{-0.05,2.45}});
      //DefaultView::draw_box(b);
      c.contract(b);
      CHECK(Approx(b,1e-2) == IntervalVector({{4.0397,5.40},{1.9089,2.45}}));
      //DefaultView::draw_box(b,Color::blue());
    }

    {
      IntervalVector b({{6.25,6.7},{0.9,1.85}});
      //DefaultView::draw_box(b);
      c.contract(b);
      CHECK(Approx(b,1e-2) == IntervalVector({{6.25,6.67},{0.9,1.351}}));
      //DefaultView::draw_box(b,Color::blue());
    }

    {
      IntervalVector b({{-6,-5},{0,2}});
      //DefaultView::draw_box(b);
      c.contract(b);
      CHECK(b == IntervalVector::empty(2));
      //DefaultView::draw_box(b,Color::blue());
    }

    {
      IntervalVector b({{2,3},{-1,1}});
      //DefaultView::draw_box(b);
      c.contract(b);
      CHECK(b == IntervalVector::empty(2));
      //DefaultView::draw_box(b,Color::blue());
    }
  }
}