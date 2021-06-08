/** 
 *  Codac - Examples
 *  Bounded-error continuous time linear observer
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Bounded-error continuous time linear observer"
 *              Simon Rohou, Luc Jaulin
 *
 *  \date       2020
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

// sudo apt install libeigen3-dev

#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  /* =========== TRUTH =========== */

    double dt = 0.01;
    Interval tdomain(0.,3.);

    TrajectoryVector x_truth(tdomain, TFunction("(10*cos(t) ; 5*sin(2*t) ; atan2(10*cos(2*t),-10*sin(t)) ; sqrt((-10*sin(t))^2+(10*cos(2*t))^2))"));
    x_truth[2].sample(dt/10.).make_continuous();


    TrajectoryVector p_truth(tdomain, TFunction("(10*cos(t);5*sin(2*t))"));
    TrajectoryVector pdot_truth(tdomain, TFunction("(-10*sin(t);10*cos(2*t))"));
    TrajectoryVector pddot_truth(tdomain, TFunction("(-10*cos(t);-20*sin(2*t))"));
    Trajectory theta_truth(tdomain, TFunction("atan2(10*cos(2*t),-10*sin(t))"));
    Trajectory u1_truth(tdomain, TFunction("(2*sin(t)*sin(2*t) + cos(t)*cos(2*t))/((sin(t)^2)+(cos(2*t)^2))"));
    Trajectory v_truth(tdomain, TFunction("sqrt((-10*sin(t))^2+(10*cos(2*t))^2)"));
    Trajectory u2_truth(tdomain, TFunction("10*(sin(t)*cos(t)-2*cos(2*t)*sin(2*t))/sqrt((sin(t)^2)+(cos(2*t)^2))"));


  /* =========== CREATING TUBES =========== */

    TubeVector x { 
      Tube(tdomain, dt),
      Tube(tdomain, dt),
      Tube(tdomain, dt, x_truth[2].codomain()),//Tube(x_truth[2], dt),
      Tube(tdomain, dt, x_truth[3].codomain()),//Tube(x_truth[3], dt)
    };

    //x[2] = Tube(x_truth[2], dt);
    //x[3] = Tube(x_truth[3], dt);
    //x[3].set(x_truth[3](0.), 0.);

    TubeVector v(tdomain, dt, 4), a(tdomain, dt, 4);

    TubeVector u { 
      Tube(u1_truth, dt),
      Tube(u2_truth, dt)
    };

    //for(double t = tdomain.lb() ; t < tdomain.ub() ; t+=tdomain.diam()/10.)
    //{
    //  double t_ = x[0].slice(t)->tdomain().lb();
    //  x[0].set(Interval(p_truth[0](t_)).inflate(0.001), t_);
    //  x[1].set(Interval(p_truth[1](t_)).inflate(0.001), t_);
    //}


  /* =========== CREATING CONTRACTORS =========== */

    CtcFunction ctc_f(Function("v[4]", "x[4]", "u[2]",
                      "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"));

    CtcFunction ctc_h(Function("a[4]", "x[4]", "u[2]",
                      "(u[1]*cos(x[2])-x[3]*sin(x[2])*u[0]-a[0] ; u[1]*sin(x[2])+x[3]*cos(x[2])*u[0]-a[1] ; a[2]-a[2] ; a[3]-a[3])"));

    CtcFunction ctc_f_v(Function("v0", "v1", "v2", "v3", "x0", "x1", "x2", "x3", "u0", "u1",
                      "(v0-x3*cos(x2) ; v1-x3*sin(x2) ; v2-u0 ; v3-u1)"));

    CtcFunction ctc_h_v(Function("a0", "a1", "a2", "a3", "x0", "x1", "x2", "x3", "u0", "u1",
                      "(u1*cos(x2)-x3*sin(x2)*u0-a0 ; u1*sin(x2)+x3*cos(x2)*u0-a1 ; a2-a2 ; a3-a3)"));

    Matrix A(2,2);
    A[0][0] = 0.; A[0][1] = 1.;
    A[1][0] = 0.; A[1][1] = 0.;

    Vector b(2);
    b[0] = 0.; b[1] = 1.;

    CtcLinobs ctc_linobs(A, b);
    CtcChain ctc_chain;


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 800, 800);
    fig_map.add_trajectory(&p_truth, "x*", 0, 1, "white");
    fig_map.smooth_tube_drawing(true);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.set_tube_color(&x, "#006A82[#006A82]");
    fig_map.show(0.);


  /* =========== CREATING OBSERVATIONS =========== */



  /* =========== SOLVING =========== */

    double vol = 0.;

    ContractorNetwork subcn;
    //cn.set_fixedpoint_ratio(0.0001);

    int nb_obs = 6;
    for(int i = 0 ; i < nb_obs ; i++)
    {
      IntervalVector& b = subcn.create_dom(IntervalVector((i%2 == 0) ? Vector({-5,6}) : Vector({0,-4})));
      Interval& t = subcn.create_dom((i+1)*tdomain.diam()/nb_obs+tdomain.lb());
      Interval& d = subcn.create_dom(sqrt(sqr(b[0]-x_truth(t)[0])+sqr(b[1]-x_truth(t)[1])));
      IntervalVector& p = subcn.create_dom(IntervalVector(4));

      subcn.add(ctc::eval, {t, p, x, v});
      subcn.add(ctc::dist, {p[0], p[1], b[0], b[1], d});

      fig_map.draw_line({b[0].lb(),x_truth(t.lb())[0]},{b[1].lb(),x_truth(t.lb())[1]}, "gray");
    }
    //cn.add(ctc_chain, {x[0], v[0], a[0]});
    //cn.add(ctc_chain, {x[1], v[1], a[1]});

    //for(int i = 0 ; i < nb_obs ; i++)
    //{
    //  cn.add(ctc::eval, {v_t[i], v_obs[i][0], x[0], v[0]});
    //  cn.add(ctc::eval, {v_t[i], v_obs[i][1], x[1], v[1]});
    //}

    //cn.add(ctc::deriv, {x, v});
    //cn.add(ctc::deriv, {v, a});
    //cn.add(ctc::deriv, {x[2], u[0]});
    //cn.add(ctc::deriv, {x[3], u[1]});
    //cn.add(ctc_f, {v, x, u});
    //cn.add(ctc_h, {a, x, u});
    //cn.print_dot_graph("dot");

    subcn.add(ctc::deriv, {x, v});
    subcn.add(ctc::deriv, {v, a});
    subcn.add(ctc::deriv, {x[2], u[0]});
    subcn.add(ctc::deriv, {x[3], u[1]});

    subcn.add(ctc_f_v, {v[0], v[1], v[2], v[3], x[0], x[1], x[2], x[3], u[0], u[1]});
    subcn.add(ctc_h_v, {a[0], a[1], a[2], a[3], x[0], x[1], x[2], x[3], u[0], u[1]});

    /*cout << "BEFORE" << endl;
    ContractorNetwork cn;
    cn.add(subcn);
    cn.add(ctc_chain, {x[0], v[0], a[0]});
    cn.add(ctc_chain, {x[1], v[1], a[1]});
    cn.contract(true);
    fig_map.show(0.);
    fig_map.axis_limits(p_truth.codomain(), true);
    cout << "AFTER" << endl;*/
    //fig_map.show(0.);
    //fig_map.axis_limits(p_truth.codomain(), true);

    // too slow: cn.add(ctc_linobs, {c0, a[0]});
    // too slow: cn.add(ctc_linobs, {c1, a[1]});

    do
    {
      vol = x.volume();

      subcn.trigger_all_contractors();
      subcn.contract(true);

      ctc_chain.contract(x[0], v[0], a[0]);
      ctc_chain.contract(x[1], v[1], a[1]);

      fig_map.show(0.);
      fig_map.axis_limits(p_truth.codomain(), true);

    } while(fabs(x.volume() - vol) > 1e-3);


    VIBesFigTubeVector fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.add_trajectory(&x_truth, "x_truth");
    fig_x.show();

    VIBesFigTube fig_vx("Vx");
    fig_vx.set_properties(100, 600, 600, 300);
    fig_vx.add_tube(&v[0], "Vx");
    fig_vx.show();

    VIBesFigTube fig_vy("Vy");
    fig_vy.set_properties(100, 600, 600, 300);
    fig_vy.add_tube(&v[1], "Vy");
    fig_vy.show();


    vibes::endDrawing();

  // Checking if this example still works:
  //bool success = x.contains(x_truth) == BoolInterval::YES;
  //return success ? EXIT_SUCCESS : EXIT_FAILURE;
}