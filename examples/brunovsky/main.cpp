/** 
 *  Codac - Examples
 *  Brunovsky decomposition for dynamic interval localization
 * ----------------------------------------------------------------------------
 *
 *  \brief      Brunovsky decomposition for dynamic interval localization
 *              Simon Rohou, Luc Jaulin
 *
 *  \date       2021
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  /* =========== TRUTH =========== */

    double dt = 0.003; // tubes timestep
    Interval tdomain(0.,3.); // temporal domain

    // Unknown truth
    TrajectoryVector x_truth(tdomain, TFunction(" \
      (10*cos(t) ; \
      5*sin(2*t) ; \
      atan2(10*cos(2*t),-10*sin(t)) ; \
      sqrt((-10*sin(t))^2+(10*cos(2*t))^2))"));
    x_truth[2].sample(dt/10.).make_continuous();

    // Input trajectories
    TrajectoryVector u_truth(tdomain, TFunction(" \
      ((2*sin(t)*sin(2*t) + cos(t)*cos(2*t))/((sin(t)^2)+(cos(2*t)^2)), \
      10*(sin(t)*cos(t)-2*cos(2*t)*sin(2*t))/sqrt((sin(t)^2)+(cos(2*t)^2)))"));

    // Landmarks
    Vector m_a({-5,6}), m_b({0,-4});


  /* =========== CREATING TUBES =========== */

    TubeVector x { 
      Tube(tdomain, dt),
      Tube(tdomain, dt),
      Tube(tdomain, dt, x_truth[2].codomain()),
      Tube(tdomain, dt, x_truth[3].codomain()),
    };

    TubeVector u(u_truth, dt); // centered on analytical expression
    // Intermediate variables:
    TubeVector v(tdomain, dt, 4), a(tdomain, dt, 4);


  /* =========== CREATING CONTRACTORS =========== */

    // Block (I): static non-linear constraints

      CtcFunction ctc_v(Function("v[4]", "x[4]", "u[2]",
                        "(v[0]-x[3]*cos(x[2]) ; \
                          v[1]-x[3]*sin(x[2]) ; \
                          v[2]-u[0] ; \
                          v[3]-u[1])"));

      CtcFunction ctc_a(Function("a[4]", "x[4]", "u[2]",
                        "(u[1]*cos(x[2])-x[3]*sin(x[2])*u[0]-a[0] ; \
                          u[1]*sin(x[2])+x[3]*cos(x[2])*u[0]-a[1])"));

    // Block (II): differential relations

      CtcChain ctc_chain;


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 800, 500);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
    fig_map.smooth_tube_drawing(true);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_beacon(m_a, 0.4);
    fig_map.add_beacon(m_b, 0.4);
    fig_map.set_tube_color(&x, "#006A82[#006A82]");
    fig_map.show(1.);


  /* =========== CREATING OBSERVATIONS =========== */

    // See the next "for" loop: observations are directly added to a CN


  /* =========== SOLVING =========== */

    ContractorNetwork cn;
    //cn.set_fixedpoint_ratio(0.001); // faster and less accurate propag.

    // Block (I): static non-linear constraints
    cn.add(ctc_v, {v, x, u});
    cn.add(ctc_a, {a, x, u});

    // Eval constraints
    int nb_obs = 4;
    for(int i = 0 ; i < nb_obs ; i++)
    {
      // New domains and intermediate variables:
      Vector& landmark((i%2 == 0) ? m_a : m_b); // selecting landmark
      IntervalVector& b = cn.create_interm_var(IntervalVector(landmark));
      Interval& t = cn.create_interm_var((i+1)*tdomain.diam()/nb_obs+tdomain.lb());
      Interval& d = cn.create_interm_var(sqrt(sqr(b[0]-x_truth(t)[0])+sqr(b[1]-x_truth(t)[1])));
      IntervalVector& p = cn.create_interm_var(IntervalVector(4)); // state at time t

      d.inflate(0.025); // bounded measurement
      cout << i << "\t" << ((i%2 == 0) ? "m_a" : "m_b") << "\t" << t.mid() << "\t" << d << endl;

      // Adding observation constraints to the CN:
      cn.add(ctc::eval, {t, p, x, v});
      cn.add(ctc::dist, {p[0], p[1], b[0], b[1], d});

      // Drawing range-only measurement:
      fig_map.draw_line({b[0].lb(),x_truth(t.lb())[0]},{b[1].lb(),x_truth(t.lb())[1]}, "gray");
    }

    clock_t t_start = clock();
    
    // Towards fixed point:
    for(int i = 0 ; i < 3 ; i++) // fixed point considered as reached after n iterations
    {
      cn.trigger_all_contractors();
      cn.contract(true); // Block (I): static non-linear constraints

      // Block (II): differential relations (two chains)
      ctc_chain.contract(x[0], v[0], a[0]);
      ctc_chain.contract(x[1], v[1], a[1]);
    }

    printf("Computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
    fig_map.show(0.8);
    fig_map.axis_limits(x_truth.codomain().subvector(0,1), true, 0.05);


  /* =========== GRAPHICS =========== */

    VIBesFigTubeVector fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.add_trajectory(&x_truth, "x_truth");
    fig_x.set_tube_color(&x, "#006A82[#006A82]");
    fig_x.set_trajectory_color(&x_truth, "#fff");
    fig_x.show();

    vibes::endDrawing();


  // Checking if this example still works:
  bool success = x.contains(x_truth) == BoolInterval::YES;
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}