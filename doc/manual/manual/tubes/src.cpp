/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <codac2_ExprType.h>
#include <codac2_Interval.h>
#include <codac2_TDomain.h>
#include <codac2_SlicedTube.h>
#include <codac2_AnalyticTraj.h>
#include <codac2_SampledTraj.h>
#include <codac2_SlicedTube_operations.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("TDomain class - manual")
{
  {
    // [tdomain-class-1-beg]
    auto td0 = create_tdomain();                  // one unbounded slice [-oo,oo]
    auto td1 = create_tdomain({0,1});             // one slice [0,1]
    auto td2 = create_tdomain({0,1}, 0.5, false); // [0,0.5],[0.5,1]
    auto td3 = create_tdomain({0,1}, 0.5, true);  // [0],[0,0.5],[0.5],[0.5,1],[1]
    // [tdomain-class-1-end]
  }

  {
    // [tdomain-class-2-beg]
    auto td = create_tdomain({0,1}, 0.5, true); // returns a std::shared_ptr<TDomain>

    Interval dom = td->t0_tf();  // [0,1]
    size_t n = td->nb_tslices(); // 5
    size_t m = td->nb_tubes();   // 0 initially

    auto it = td->tslice(0.0);   // [0]
         it = td->tslice(0.1);   // [0,0.5]
         it = td->tslice(0.5);   // [0.5]
         it = td->tslice(0.6);   // [0.5,1]
         it = td->tslice(1.0);   // [1]
    // [tdomain-class-2-end]
    CHECK(n == 5);
    CHECK(m == 0);
    CHECK(dom == Interval(0,1));
  }

  {
    // [tdomain-class-3-beg]
    auto td = create_tdomain(); // returns a std::shared_ptr<TDomain>
    td->sample(1., false); // [-oo,1],[1,oo]
    td->sample(10., true); // [-oo,1],[1,10],[10],[10,oo]
    // [tdomain-class-3-end]
  }

  {
    // [tdomain-class-4-beg]
    auto td = create_tdomain({0,2}, 1.0, false); // false: without gates

    SlicedTube x(td, Interval(0,1));
    SlicedTube v(td, Interval(-1,1));

    cout << x << endl;   // outputs [0,2]->[0,1], 2 slices
    cout << v << endl;   // outputs [0,2]->[0,1], 2 slices

    size_t n = td->nb_tslices(); // 2: [0,1],[1,2]
    x.set({0.5,1}, 1.3);         // local update, will refine the partition at t=1.3
    size_t m = td->nb_tslices(); // now 4: [0,1],[1,1.3],[1.3],[1.3,2]

    cout << x << endl;   // outputs [0,2]->[-1,1], 4 slices
    cout << v << endl;   // outputs [0,2]->[-1,1], 4 slices (v is also impacted by x.set(..))
    // [tdomain-class-4-end]
    CHECK(n == 2);
    CHECK(m == 4);
  }

  {
    // [tdomain-class-5-beg]
    auto td = create_tdomain({0,3}, 1.0, false);
    SlicedTube<Interval> x(td, Interval(0,1));

    cout << td->nb_tubes() << endl; // 1
    td->truncate({0.5,2.5});

    cout << td->t0_tf() << endl;    // [0.5,2.5]
    cout << x << endl;              // x now uses the truncated shared partition
    // [tdomain-class-5-end]
  }
}

TEST_CASE("SlicedTube class - manual")
{
  Figure2D fig("Tube", GraphicOutput::VIBES);
  fig.set_window_properties({50,50},{800,400});

  {
    // [slicedtube-class-1-beg]
    auto td = create_tdomain({0,10}, 0.1, true);

    // Sliced tube from interval-type codomains
    SlicedTube x(td, Interval(-1,1)); // x has type SlicedTube<Interval>
    SlicedTube y(td, IntervalVector(2)); // y has type SlicedTube<IntervalVector>
    SlicedTube z(td, IntervalMatrix(2,2)); // z has type SlicedTube<IntervalMatrix>

    // Explicit template notation remains possible:
    SlicedTube<Interval> x2(td, Interval(-1,1));

    // From an analytic function
    ScalarVar t;
    AnalyticFunction f({t}, sin(t));
    SlicedTube xf(td, f);
    // xf has type SlicedTube<Interval> because f outputs scalar values

    // From an analytic trajectory or sampled trajectory
    AnalyticTraj at({0,10}, AnalyticFunction({t},cos(t)));
    SampledTraj st = AnalyticTraj({0,10}, AnalyticFunction({t},cos(t)+t/10)).sampled(1e-2);
    auto xu = SlicedTube(td,at) | SlicedTube(td,st); // union (hull) of tubes

    fig.plot_tube(xf, {Color::dark_blue(),Color::light_gray()});
    fig.plot_tube(xu, {Color::dark_blue(),Color::light_gray()});
    // [slicedtube-class-1-end]
  }

  {
    // [slicedtube-class-2-beg]
    auto td = create_tdomain({0,3});
    SlicedTube x(td, IntervalVector(2));
    x.set({{1,5},{-oo,2}}, {0,1});
    x.set({{2,8},{-oo,3}}, {1,2});
    x.set({{6,9},{-oo,4}}, {2,3});

    auto s0 = x.first_slice();
    auto s1 = s0->next_slice();

    for(const auto& s : x)
      std::cout << s.t0_tf() << " -> " << s.codomain() << std::endl;
    // [slicedtube-class-2-end]
  }

  {
    // [slicedtube-class-4-beg]
    auto td = create_tdomain({0,3}, 1.0, false);
    SlicedTube x(td, Interval());

    x.set({1,5}, {0,1});
    x.set({2,8}, {1,2});
    x.set({6,9}, {2,3});

    Interval y0 = x(0.5);   // [1,5]
    Interval y1 = x(1.5);   // [2,8]
    Interval y2 = x({0,3}); // [1,9]
    Interval y3 = x(-1.0);  // [-oo,oo]

    // No explicit gates: boundary values come from adjacent-slice intersections
    Interval y4 = x(1.0);   // [2,5]
    Interval y5 = x(2.0);   // [6,8]
    Interval y6 = x(3.0);   // [6,9]
    // [slicedtube-class-4-end]
  }

  {
    Interval T(0,10);
    auto td = create_tdomain(T, 1e-1);

    ScalarVar t;

    AnalyticTraj z1(T, AnalyticFunction({t},cos(t)));
    AnalyticTraj z2(T, AnalyticFunction({t},cos(t)+t/10));
    AnalyticTraj z3(T, AnalyticFunction({t},sin(t)+t/10));
    SampledTraj z4 = AnalyticTraj(T, AnalyticFunction({t},sin(t))).sampled(1e-2);

    SlicedTube x1(td, z1);
    SlicedTube x2(td, z2);
    SlicedTube x3(td, z3);
    SlicedTube x4(td, z4);

    auto x = x1 | x2 | x3 | x4;

    // [slicedtube-class-5-beg]
    std::vector<Interval> v_t;
    Interval y(0,0.2);
    x.invert(y, v_t);

    for(const auto& ti : v_t)
    {
      IntervalVector z = cart_prod(ti,y);
      DefaultFigure::draw_box(z, Color::red());
    }
    // [slicedtube-class-5-end]
  }

  {
    // [slicedtube-class-6-beg]
    auto td = create_tdomain({0,2}, 1.0, false);

    SlicedTube x(td, Interval(1,2));
    SlicedTube y(td, Interval(-1,3));

    auto z_add = x + y;  // addition of two tubes
    auto z_mul = 2. * x; // multiplication by a scalar/interval
    auto z_hul = x | y;  // hull (union) of two tubes
    auto z_int = x & y;  // intersection of two tubes

    auto u = sin(x) + exp(y);
    // [slicedtube-class-6-end]
  }

  {
    auto td = create_tdomain();

    // [slicedtube-class-7-beg]
    SlicedTube A(td, IntervalMatrix(2,2));
    SlicedTube b(td, IntervalVector(2));

    auto y = A * b; // type: SlicedTube<IntervalVector>
    // [slicedtube-class-7-end]
  }

  {
    auto td = create_tdomain({0,2});
    SlicedTube x(td, Interval());

    // [slicedtube-class-8-beg]
    x.inflate(0.2); // constant inflation
    
    SampledTraj<double> rad({{0.0,0.11}, {1.0,0.3}, {2.0,0.2}});
    x.inflate(rad); // time-varying inflation radius
    // [slicedtube-class-8-end]
  }

  {
    auto td = create_tdomain();
    SlicedTube x(td, IntervalVector(3));

    // [slicedtube-class-9-beg]
    // Component and subvector extraction
    auto x0 = x[0];              // type: SlicedTube<Interval>
    auto x12 = x.subvector(1,2); // type: SlicedTube<IntervalVector>
    // [slicedtube-class-9-end]
  }

}