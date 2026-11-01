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
#include <fstream>
#include <sstream>
#include <filesystem>
#include <codac2_ExprType.h>
#include <codac2_Interval.h>
#include <codac2_TDomain.h>
#include <codac2_SlicedTube.h>
#include <codac2_AnalyticTraj.h>
#include <codac2_SampledTraj.h>
#include <codac2_SlicedTube_operations.h>
#include <codac2_traj_cart_prod.h>
#include <codac2_tube_cart_prod.h>
#include <codac2_SampledTraj_operations.h>
#include <codac2_serialization.h>
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

    auto it = td->tslice(0.);   // [0]
         it = td->tslice(0.1);  // [0,0.5]
         it = td->tslice(0.5);  // [0.5]
         it = td->tslice(0.6);  // [0.5,1]
         it = td->tslice(1.);   // [1]
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
    auto td = create_tdomain({0,2}, 1., false); // false: without gates

    SlicedTube x(td, Interval(0,1));
    SlicedTube v(td, Interval(-1,1));

    cout << x << endl;   // codomain [0,1], 2 temporal elements
    cout << v << endl;   // codomain [-1,1], 2 temporal elements

    size_t n = td->nb_tslices(); // 2: [0,1],[1,2]
    x.set({0.5,1}, 1.3);         // local update, will refine the partition at t=1.3
    size_t m = td->nb_tslices(); // now 4: [0,1],[1,1.3],[1.3],[1.3,2]

    cout << x << endl;   // codomain [0,1], 4 temporal elements
    cout << v << endl;   // codomain [-1,1], 4 temporal elements (shared partition refined)
    // [tdomain-class-4-end]
    CHECK(n == 2);
    CHECK(m == 4);
  }

  {
    // [tdomain-class-5-beg]
    auto td = create_tdomain({0,3}, 1., false);
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
    SlicedTube x(td, Interval(-1,1));        // scalar tube: SlicedTube<Interval>
    SlicedTube y(td, IntervalVector(2));     // vector tube: SlicedTube<IntervalVector>
    SlicedTube z(td, IntervalMatrix(2,2));   // matrix tube: SlicedTube<IntervalMatrix>

    // Explicit template notation remains possible:
    SlicedTube<Interval> x2(td, Interval(-1,1));

    // From an analytic function
    ScalarVar t;
    AnalyticFunction f({t}, sin(t));
    SlicedTube xf(td, f); // scalar tube: SlicedTube<Interval>

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
    auto td = create_tdomain({0,3}, 1., false);
    SlicedTube x(td, Interval());

    x.set({1,5}, {0,1});
    x.set({2,8}, {1,2});
    x.set({6,9}, {2,3});

    Interval y0 = x(0.5);   // [1,5]
    Interval y1 = x(1.5);   // [2,8]
    Interval y2 = x({0,3}); // [1,9]
    Interval y3 = x(-1.);   // [-oo,oo]

    // No explicit gates: boundary values come from adjacent-slice intersections
    Interval y4 = x(1.);   // [2,5]
    Interval y5 = x(2.);   // [6,8]
    Interval y6 = x(3.);   // [6,9]
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

    for(const auto& t : v_t)
    {
      IntervalVector z = cart_prod(t,y);
      DefaultFigure::draw_box(z, Color::red());
    }
    // [slicedtube-class-5-end]
  }

  {
    // [slicedtube-class-6-beg]
    auto td = create_tdomain({0,2}, 1., false);

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

    auto y = A * b; // vector tube: SlicedTube<IntervalVector>
    // [slicedtube-class-7-end]
  }

  {
    // [slicedtube-class-function-eval-beg]
    auto td = create_tdomain({0,5}, 0.1, true);

    ScalarVar t;
    SlicedTube heading(td, AnalyticFunction({t}, 0.2*sin(t)));
    SlicedTube speed(td, Interval(1,2));

    ScalarVar h, v;
    AnalyticFunction f({h,v}, {v*cos(h), v*sin(h)});
    auto velocity = f.tube_eval(heading,speed); // vector tube: SlicedTube<IntervalVector>

    VectorVar p(2);
    AnalyticFunction g({p}, sqr(p[0])+sqr(p[1]));
    auto velocity_norm_squared = g.tube_eval(velocity); // scalar tube: ||velocity||^2
    // [slicedtube-class-function-eval-end]

    CHECK(velocity.size() == 2);
    CHECK(velocity_norm_squared.size() == 1);
  }

  {
    auto td = create_tdomain({0,2});
    SlicedTube x(td, Interval());

    // [slicedtube-class-8-beg]
    x.inflate(0.2); // constant inflation
    
    SampledTraj<double> rad({{0.,0.11}, {1.,0.3}, {2.,0.2}});
    x.inflate(rad); // time-varying inflation radius
    // [slicedtube-class-8-end]
  }

  {
    auto td = create_tdomain();
    SlicedTube x(td, IntervalVector(3));

    // [slicedtube-class-9-beg]
    // Component and subvector extraction
    auto x0 = x[0];              // scalar tube: SlicedTube<Interval>
    auto x12 = x.subvector(1,2); // vector tube: SlicedTube<IntervalVector>
    // [slicedtube-class-9-end]
  }

}


TEST_CASE("Trajectory classes - manual")
{
  {
    // [traj-sampled-create-beg]
    SampledTraj<double> depth;
    depth.set(2., 0.);
    depth.set(3., 1.);
    depth.set(4., 2.);

    SampledTraj<Vector> position;
    position.set(Vector({0,1}), 0.);
    position.set({1,2}, 1.); // equivalent shorter notation
    position.set({2,3}, 2.);
    // [traj-sampled-create-end]
    CHECK(depth.nb_samples() == 3);
    CHECK(position.size() == 2);
  }

  {
    // [traj-sampled-eval-beg]
    SampledTraj<double> x({{0.,2.}, {1.,4.}, {2.,3.}});
    double value = x(0.5);              // 3.
    Interval range = x({0.5,1.5});      // [3,4]
    auto sample_count = x.nb_samples(); // 3
    auto output_size = x.size();        // 1
    // [traj-sampled-eval-end]
    CHECK(value == 3);
    CHECK(range == Interval(3,4));
    CHECK(sample_count == 3);
    CHECK(output_size == 1);

    // [traj-sampled-iterate-beg]
    for(const auto& [ti, xi] : x)
      std::cout << ti << " -> " << xi << std::endl;
    // [traj-sampled-iterate-end]
    CHECK(value == 3.);
    CHECK(sample_count == 3);
    CHECK(output_size == 1);
    CHECK(range.contains(3.));
  }

  {
    // [traj-sampled-components-beg]
    SampledTraj<Vector> p({{0.,{0,1,2}}, {1.,{1,2,3}}});
    auto x = p[0];
    auto yz = p.subvector(1,2);
    // [traj-sampled-components-end]
    CHECK(yz.size() == 2);

  }

  {
    // [traj-sampled-time-beg]
    SampledTraj<double> x({{0.,0.}, {1.,1.}, {2.,0.}});
    x.truncate_tdomain({0.5,1.5}); // retains interpolated endpoints
    x.shift_tdomain(10.);          // time domain now [10.5,11.5]
    x.stretch_tdomain({0,1});      // rescale time, not values
    // [traj-sampled-time-end]
    CHECK(x.tdomain() == Interval(0,1));
  }

  {
    // [traj-ops-common-tdomain-beg]
    SampledTraj<double> x1({0.,4.}, {0,4});
    SampledTraj<Vector> x2({2.,5}, {Vector({1,2}), Vector({5,6})});
    truncate_to_common_tdomain(x1,x2); // common time domain: [2,4]
    // [traj-ops-common-tdomain-end]
    CHECK(x1.tdomain() == Interval(2,4));
    CHECK(x2.tdomain() == Interval(2,4));
  }

  {
    // [traj-analytic-create-beg]
    ScalarVar t;
    AnalyticFunction f({t}, 2*sin(t));
    AnalyticTraj traj_f({0,2}, f); // scalar trajectory

    AnalyticFunction g({t}, {cos(t), sin(t)});
    AnalyticTraj traj_g({0,2}, g); // vector trajectory
    // [traj-analytic-create-end]

    // [traj-analytic-eval-beg]
    double y_point = traj_f(0.5);         // scalar value
    Interval y_interval = traj_f({0,1}); // interval enclosure
    Vector p_point = traj_g(0.5);        // vector value
    // [traj-analytic-eval-end]
    CHECK(y_interval.contains(y_point));
    CHECK(p_point.size() == 2);

    // [traj-analytic-sampled-beg]
    SampledTraj<double> f_samples = traj_f.sampled(0.1);
    SampledTraj<Vector> g_samples = traj_g.sampled(0.1);
    // [traj-analytic-sampled-end]
    CHECK(f_samples.nb_samples() >= 20);
    CHECK(g_samples.size() == 2);
  }

  {
    // [traj-ops-calculus-beg]
    SampledTraj<double> speed({{0.,0}, {1.,1}, {2.,2}});
    auto acceleration = speed.derivative(); // numerical derivative
    auto distance = speed.primitive();      // trapezoidal integral, p(0)=0
    auto doubled_speed = 2. * speed;        // pointwise arithmetic
    // [traj-ops-calculus-end]
    CHECK(distance(2.) == 2.);
  }

  {
    // [traj-ops-cartprod-beg]
    SampledTraj<double> x1({0.,1}, {0.,1});
    SampledTraj<double> x2({0.,1}, {2.,3});
    auto x = traj_cart_prod(x1,x2);
    // [traj-ops-cartprod-end]
    CHECK(x.size() == 2);

    // [traj-ops-continuous-beg]
    SampledTraj<double> heading({{0.,6.1}, {1.,0.1}, {2.,0.2}});
    auto continuous_heading = continuous_traj(heading);
    // [traj-ops-continuous-end]
    CHECK(continuous_heading.nb_samples() == 3);
  }

  {
    // [traj-ops-eval-beg]
    SampledTraj<double> theta({{0.,0}, {1.,0.1}, {2.,0.2}});
    SampledTraj<double> v({{0.,1}, {1.,2}, {2.,3}});
    ScalarVar theta_var, v_var;
    AnalyticFunction f({theta_var,v_var}, {v_var*cos(theta_var), v_var*sin(theta_var)});
    auto u = f.traj_eval(theta, v);
    // [traj-ops-eval-end]
    CHECK(u.size() == 2);
  }

  {
    // [traj-ops-asfunction-beg]
    SampledTraj<double> measurements({{0.,1}, {1.,2}, {2.,1.5}});
    auto measurement_fn = measurements.as_function();
    ScalarVar t;
    AnalyticFunction energy({t}, sqr(measurement_fn(t)));
    auto estimated_energy = energy.eval(0.5);
    // [traj-ops-asfunction-end]
  }

  {
    // [traj-ops-tube-beg]
    ScalarVar t;
    AnalyticTraj traj({0,2}, AnalyticFunction({t}, sin(t)));
    auto td = create_tdomain({0,2}, 0.1);
    SlicedTube tube(td, traj);
    tube.inflate(0.05); // encloses the true signal if its error is bounded by 0.05
    // [traj-ops-tube-end]
  }

  {
    // [traj-ops-serialization-beg]
    SampledTraj<double> input({{0.,1}, {1.,1.5}, {2.,2}});
    {
      std::ofstream out("trajectory.cdc", std::ios::binary);
      serialize(out, input);
    }
    SampledTraj<double> restored;
    {
      std::ifstream in("trajectory.cdc", std::ios::binary);
      deserialize(in, restored);
    }
    // [traj-ops-serialization-end]
    CHECK(restored.nb_samples() == input.nb_samples());
  }
}


TEST_CASE("Trajectory reference - manual")
{
  {
    // [traj-sampled-grid-beg]
    SampledTraj<double> x({{0.,0}, {1.,2}, {2.,0}});
    auto merged = x.sampled(0.6);         // original samples also retained
    auto regular = x.sampled(0.6, false); // regular grid and final time only
    SampledTraj<double> clock({{0.,0}, {0.5,0}, {1.5,0}, {2.,0}});
    auto aligned = x.sampled_as(clock); // x evaluated at clock's timestamps
    // [traj-sampled-grid-end]
    CHECK(merged.nb_samples() > regular.nb_samples());
    CHECK(aligned.nb_samples() == clock.nb_samples());
  }
  {
    // [traj-sampled-construct-alt-beg]
    std::map<double,double> samples({{0.,3}, {1.,4}, {2.,5}});
    SampledTraj<double> x(samples);
    x.set(6., 1.); // replaces value at t=1
    SampledTraj<double> y({0.,1.,2.}, {3,6,5}); // two std::list arguments
    // [traj-sampled-construct-alt-end]
    CHECK(x.nb_samples() == 3);
    CHECK(x(1.) == 6.);
    CHECK(y(1.) == 6.);
  }
  {
    // [traj-analytic-advanced-beg]
    ScalarVar t;
    AnalyticTraj a({0,2}, AnalyticFunction({t}, sqr(t)));
    auto f = a.as_function();
    AnalyticFunction g({t}, f(t)+1.);
    // [traj-analytic-advanced-end]

  }
  {
    // [traj-ops-pointwise-beg]
    SampledTraj<double> x({{0.,0}, {1.,1}, {2.,2}});
    SampledTraj<double> y({{0.,2}, {1.,3}, {2.,4}});
    auto sum = x + y;    // computed at matching timestamps
    auto sq = sqr(x);    // square samples, then interpolate
    auto sin_x = sin(x); // transform samples, then interpolate
    // [traj-ops-pointwise-end]
    CHECK(sum(1.) == 4.);
    CHECK(sq(1.) == 1.);
  }
  {
    // [traj-ops-fun-vector-beg]
    SampledTraj<Vector> p({{0.,{3.,4}}, {1.,{0.,5}}});
    VectorVar p_var(2);
    AnalyticFunction f({p_var}, sqr(p_var[0]) + sqr(p_var[1]));
    auto result = f.traj_eval(p);
    // [traj-ops-fun-vector-end]
    CHECK(result.size() == 1);
    CHECK(result(0.) == 25.);
  }
  {
    // [traj-tubes-mid-beg]
    auto td = create_tdomain({0,2}, 0.5, true);
    ScalarVar t;
    AnalyticTraj a({0,2}, AnalyticFunction({t}, sin(t)));
    SlicedTube tube(td,a);
    auto midpoint = tube.mid(); // approximate midpoint trajectory
    // [traj-tubes-mid-end]
    CHECK(midpoint.nb_samples() > 1);
  }
  {
    // [traj-io-csv-beg]
    SampledTraj<double> measurements;
    std::ifstream csv(std::filesystem::path(__FILE__).parent_path() / "data.csv");
    std::string line;
    std::getline(csv, line); // skip header

    while(std::getline(csv, line))
    {
      double t, value;
      if(std::sscanf(line.c_str(), "%lf;%lf", &t, &value) == 2)
        measurements.set(value, t);
    }
    // [traj-io-csv-end]
    CHECK(measurements.nb_samples() == 3);
  }
}

TEST_CASE("SlicedTube cross-reference examples - manual")
{
  {
    // [tube-example-1-beg]
    auto td = create_tdomain({0,3}, 1., false);
    SlicedTube x(td, Interval()); // [-∞,∞] by default for any time
    x.set({1,5}, {0,1});
    x.set({2,8}, {1,2});
    x.set({6,9}, {2,3});
    auto y0 = x({0,3});
    auto y1 = x(1.5);
    auto y2 = x(1.);
    // [tube-example-1-end]
    CHECK(!y0.is_empty());
  }
  {
    // [tube-example-2-beg]
    auto td = create_tdomain({0,1}, 0.1, false);
    SlicedTube x(td, IntervalVector(3));
    auto before = td->nb_tslices();
    x.set(IntervalVector::constant(3,{2,3}), -42.);
    auto after = td->nb_tslices(); // the partition was extended and refined
    // [tube-example-2-end]
    CHECK(after > before);
  }
  {
    // [tube-example-3-beg]
    auto td = create_tdomain({0,5}, 0.1, true);
    ScalarVar t;
    AnalyticFunction f({t}, 10*cos(t)+t);
    SlicedTube x(td, f); // encloses f(t) over each time slice
    // [tube-example-3-end]
    CHECK(!x.is_empty());
  }
  {
    // [tube-example-4-beg]
    auto td = create_tdomain({6.4,6.5});
    SlicedTube x(td, Interval(0.759,1.205));
    SlicedTube v(td, Interval(-0.216,-0.115));
    x.set(Interval(0.781,1.205), 6.4);
    x.set(Interval(0.759,1.194), 6.5);
    CtcDeriv ctc;
    ctc.contract(x,v);
    auto sx = x.first_slice();
    auto sv = v.first_slice();
    auto polygon = sx->polygon_slice(*sv);
    // [tube-example-4-end]
  }
  {
    // [tube-example-5-beg]
    auto td = create_tdomain({0,2}, 0.5, true);
    SlicedTube x(td, Interval(1,2));
    auto integral_at_one = x.integral(Interval(1));
    auto integral_over_range = x.integral(Interval(0.5,1.5));
    auto p = x.primitive(Interval(0));
    // [tube-example-5-end]
    CHECK(!integral_at_one.is_empty());
  }
  {
    // [tube-example-6-beg]
    auto td = create_tdomain({0,1}, 0.5, false);
    SlicedTube x1(td, Interval(0,1));
    SlicedTube x2(td, IntervalVector({{1,2},{-1,1}}));
    auto x = tube_cart_prod(x1,x2); // vector tube of dimension 3
    // [tube-example-6-end]
    CHECK(x.size() == 3);
  }
  {
    // [tube-parity-algebra-beg]
    auto td = create_tdomain({0,2}, 0.5, true);
    SlicedTube x(td, Interval(0,2));
    SlicedTube y(td, Interval(1,3));
    auto both = x & y;             // intersection of enclosures
    auto either = x | y;           // interval hull, not disconnected union
    auto sum = x + y;              // interval arithmetic on every slice
    auto xy = tube_cart_prod(x,y); // vector tube: SlicedTube<IntervalVector>
    // [tube-parity-algebra-end]
    CHECK(xy.size() == 2);
  }
  {
    // [tube-parity-conversion-beg]
    auto td = create_tdomain({0,2}, 0.5, true);
    SlicedTube tube(td, Interval(-1,1));
    SampledTraj<double> midpoint = tube.mid(); // an approximation
    SlicedTube middle_curve(td, midpoint);     // encloses the interpolant
    // [tube-parity-conversion-end]
    CHECK(midpoint.nb_samples() > 1);
  }
  {
    // [tube-parity-input-beg]
    SampledTraj<double> measured({{0.,1}, {1.,1.5}, {2.,2}});
    auto td = create_tdomain({0,2}, 0.5, true);
    SlicedTube x(td, measured); // encloses the piecewise-linear interpolant
    x.inflate(0.2);             // only guaranteed if the bound is justified
    // [tube-parity-input-end]
    CHECK(!x.is_empty());
  }
  {
    // [tube-parity-export-beg]
    auto td = create_tdomain({0,2}, 1., true);
    SlicedTube x(td, Interval(-1,1));
    std::ostringstream csv;
    csv << "t0;t1;lb;ub;is_gate\n";
    for(const auto& s : x)
      csv << s.t0_tf().lb() << ';' << s.t0_tf().ub() << ';'
          << s.codomain().lb() << ';' << s.codomain().ub() << ';'
          << (s.is_gate() ? 1 : 0) << '\n';
    // [tube-parity-export-end]
    CHECK(csv.str().find("is_gate") != std::string::npos);
  }
}

TEST_CASE("Slice reference - manual")
{
  {
    // [slice-endpoint-example-beg]
    auto td = create_tdomain({0,2}, 1., false);
    SlicedTube x(td, Interval());
    x.set({1,5}, {0,1});
    x.set({2,8}, {1,2});
    auto s = x.first_slice();
    auto boundary = s->output_gate(); // [2,5]
    // [slice-endpoint-example-end]
  }
  {
    // [slice-invert-example-beg]
    auto td = create_tdomain({0,1});
    SlicedTube x(td, Interval(0,10));
    x.set({2,3}, 0.);
    x.set({5,6}, 1.);
    auto s = x.first_slice();
    auto possible_times = s->invert(Interval(2.5,6), Interval(0.2,0.5));
    // [slice-invert-example-end]
  }
}
