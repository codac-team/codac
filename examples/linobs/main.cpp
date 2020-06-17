/** 
 *  tubex-lib - Examples
 *  Bounded-error continuous time linear observer
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Bounded-error continuous time linear observer"
 *              Simon Rohou, Luc Jaulin
 *
 *  \date       2020
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>
#include <unsupported/Eigen/MatrixFunctions> // for computing e^At

using namespace std;
using namespace tubex;

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
  /* =========== PROBLEM DEFINITION =========== */

    srand(time(NULL));

    Interval tdomain(0.,10.);

    TFunction f_u("cos(t)+sin(t/3)+t/10");

    Matrix A(2,2);
    A[0][0] = 0.;  A[0][1] = 1.;
    A[1][0] = -1.; A[1][1] = -1.;

    Vector b(2);
    b[0] = 0.; b[1] = 1.;


  /* =========== SIMULATING THE TRUTH =========== */

    double dt = 0.01;
    TrajectoryVector x_truth = simu_truth(A, b, f_u, dt/10., tdomain);


  /* =========== CREATING TUBES =========== */

    Tube u(Tube(tdomain, dt, f_u).inflate(0.1)); // u* + [-0.1,0.1]
    TubeVector x(tdomain, dt, 2);

    // Optional: initial and final conditions on x:
    x.set(IntervalVector(x_truth.first_value()).inflate(0.0001), tdomain.lb()); // x0
    x.set(IntervalVector(x_truth.last_value()).inflate(0.0001), tdomain.ub()); // xf


  /* =========== CREATING OBSERVATIONS =========== */

    int nb_obs = 10;
    vector<double> v_t(nb_obs);
    vector<IntervalVector> v_obs(nb_obs);

    for(int i = 0 ; i < nb_obs ; i++)
    {
      // Random time in the temporal domain
      v_t[i] = Tools::rand_in_bounds(tdomain);
      // Getting observation from the truth
      v_obs[i] = IntervalVector(x_truth(v_t[i])).inflate(0.01);
    }


  /* =========== CONTRACTING THE POLYGONS =========== */

    CtcLinobs ctc_linobs(A, b, &exp);
    vector<ConvexPolygon> polygons_fwd, polygons_fwdbwd;
    ctc_linobs.contract(v_t, v_obs, x, u, polygons_fwd, TimePropag::FORWARD);
    ctc_linobs.contract(v_t, v_obs, x, u, polygons_fwdbwd, TimePropag::FORWARD | TimePropag::BACKWARD);


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 600, 600);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1);
    fig_map.draw_polygons(polygons_fwd, ColorMap::BLUE_TUBE);
    fig_map.draw_polygons(polygons_fwdbwd, "#1B4054");
    fig_map.draw_boxes(v_obs, "red");
    fig_map.show(0.);
    vibes::endDrawing();

  // Checking if this example still works:
  bool success = x.contains(x_truth) == BoolInterval::YES;
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}