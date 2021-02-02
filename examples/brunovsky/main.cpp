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
#include <codac-rob.h>
#include <unsupported/Eigen/MatrixFunctions> // for computing e^At

using namespace std;
using namespace codac;

IntervalMatrix exp(const Matrix& A, const Interval& intv_t) // computes e^At
{
  IntervalMatrix A_exp(2, 2, Interval::EMPTY_SET);
  vector<double> v_t({intv_t.lb(), intv_t.ub()});

  for(const auto& t : v_t)
  {
    Eigen::MatrixXd eigen_A(2,2);
    eigen_A <<  A[0][0], A[0][1],
                A[1][0], A[1][1];
    eigen_A = eigen_A * t;
    Eigen::MatrixXd eigen_A_exp = eigen_A.exp();

    A_exp[0][0] |= eigen_A_exp(0,0); A_exp[0][1] |= eigen_A_exp(0,1);
    A_exp[1][0] |= eigen_A_exp(1,0); A_exp[1][1] |= eigen_A_exp(1,1);
  }
  
  return A_exp;
}

TrajectoryVector simu_truth(const Matrix& A, const Vector& B, const TFunction& f_u, double dt, const Interval& tdomain)
{
  TrajectoryVector truth(2);
  Vector x0(2, 0.), x = x0;

  for(double t = tdomain.lb() ; t < tdomain.ub()+dt ; t+=dt)
  {
    Vector xdot(2);
    xdot[0] = x[1];
    xdot[1] = -x[0]-x[1]+f_u.eval(t).mid();

    x += dt*xdot;
    truth.set(x, t);
  }

  truth.truncate_tdomain(tdomain); // clean truncature [t0,tf]
  return truth;
}

int main()
{
  /* =========== TRUTH =========== */

    double dt = 0.01;
    Interval tdomain(0.,4.);

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
    
    Matrix A(2,2);
    A[0][0] = 0.; A[0][1] = 1.;
    A[1][0] = 0.; A[1][1] = 0.;

    Vector b(2);
    b[0] = 0.; b[1] = 1.;

    CtcLinobs ctc_linobs(A, b, &exp);
    CtcChain ctc_chain;


  /* =========== CREATING OBSERVATIONS =========== */

    int nb_obs = 10;
    vector<double> v_t(nb_obs);
    vector<IntervalVector> v_obs(nb_obs);

    for(int i = 0 ; i < nb_obs ; i++)
    {
      // Random time in the temporal domain
      v_t[i] = (rand()/double(RAND_MAX))*tdomain.diam()+tdomain.lb();
      // Getting observation from the truth
      v_obs[i] = IntervalVector(x_truth(v_t[i])).inflate(0.01);
    }


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 800, 800);
    fig_map.add_trajectory(&p_truth, "x*", 0, 1, "white");
    fig_map.smooth_tube_drawing(true);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.set_tube_color(&x, "#006A82[#006A82]");


  /* =========== SOLVING =========== */

    double vol = 0.;

    ContractorNetwork cn;
    cn.set_fixedpoint_ratio(0.0001);
    //cn.add(ctc_chain, {x[0], v[0], a[0]});
    //cn.add(ctc_chain, {x[1], v[1], a[1]});
    cn.add(ctc::deriv, {x, v});
    cn.add(ctc::deriv, {v, a});
    cn.add(ctc::deriv, {x[2], u[0]});

    for(int i = 0 ; i < nb_obs ; i++)
    {
      cn.add(ctc::eval, {v_t[i], v_obs[i][0], x[0], v[0]});
      cn.add(ctc::eval, {v_t[i], v_obs[i][1], x[1], v[1]});
    }

    cn.add(ctc::deriv, {x[3], u[1]});
    cn.add(ctc_f, {v, x, u});
    cn.add(ctc_h, {a, x, u});
    //cn.print_dot_graph("dot");

    TubeVector c0({
      x[0],
      v[0]
    });

    TubeVector c1({
      x[1],
      v[1]
    });

    do
    {
      vol = x.volume();

      cn.contract(true);

      //ctc_linobs.contract(c0, a[0]);
      //ctc_linobs.contract(c1, a[1]);

      x[0] &= c0[0]; c0[0] &= x[0]; 
      v[0] &= c0[1]; c0[1] &= v[0];
      x[1] &= c1[0]; c1[0] &= x[1];
      v[1] &= c1[1]; c1[1] &= v[1];

      fig_map.show(0.);

    } while(x.volume() != vol);



    //fig_map.draw_polygons(polygons_fwd, ColorMap::BLUE_TUBE);
    //fig_map.draw_polygons(polygons_fwdbwd, "#1B4054");
    //fig_map.draw_boxes(v_obs, "red");




    //VIBesFigTubeVector fig_xdot("v");
    //fig_xdot.set_properties(100, 100, 600, 300);
    //TubeVector ttemp = v.subvector(0,1);
    //fig_xdot.add_tubevector(&ttemp, "v");
    //fig_xdot.add_trajectoryvector(&pdot_truth, "pdot_truth");
    //fig_xdot.show();

    VIBesFigTubeVector fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.add_trajectory(&x_truth, "x_truth");
    fig_x.show();


    vibes::endDrawing();

  // Checking if this example still works:
  //bool success = x.contains(x_truth) == BoolInterval::YES;
  //return success ? EXIT_SUCCESS : EXIT_FAILURE;
}