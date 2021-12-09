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

  truth.truncate_tdomain(tdomain); // clean truncature of the tdomain [t0,tf]
  return truth;
}

int main()
{
  /* =========== PROBLEM DEFINITION =========== */

    srand(time(nullptr));

    Interval tdomain(0.,10.);

    TFunction f_u("cos(t)+sin(t/3)+t/10");

    Matrix A(2,2);
    A[0][0] = 0.;  A[0][1] = 1.;
    A[1][0] = -1.; A[1][1] = -1.;

    Vector b(2);
    b[0] = 0.; b[1] = 1.;


  /* =========== SIMULATING THE TRUTH =========== */

    double dt_truth = 0.0001;
    TrajectoryVector x_truth = simu_truth(A, b, f_u, dt_truth, tdomain);


  /* =========== TESTING SEVERAL dt VALUES =========== */

    map<double,Trajectory> m_pol_area;
    map<double,double> m_pol_total_area;

    vector<double> v_dt = { 0.1, 0.09, 0.08, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.01, 0.005, 0.0025, 0.001 };
    // v_dt.push_back(0.0001); // uncomment this in order to exactly obtain the figure of the paper

    for(const auto& dt: v_dt)
    {
      /* =========== CREATING TUBES =========== */

        Tube u(Tube(tdomain, dt, f_u).inflate(0.1)); // u* + [-0.1,0.1]
        TubeVector x(tdomain, dt, 2);


      /* =========== CREATING OBSERVATIONS =========== */

        int nb_obs = 8;
        vector<double> v_t(nb_obs);
        vector<IntervalVector> v_obs(nb_obs);

        v_t[0] = 2./3.; v_obs[0] = Vector({0.188, 0.493});
        v_t[1] = 1.9;   v_obs[1] = Vector({0.783, 0.261});
        v_t[2] = 2.99;  v_obs[2] = Vector({0.728,-0.308});
        v_t[3] = 4.33;  v_obs[3] = Vector({0.380, 0.009});
        v_t[4] = 6.4;   v_obs[4] = Vector({1.747, 0.976});
        v_t[5] = 6.5;   v_obs[5] = Vector({1.844, 0.947});
        v_t[6] = 6.6;   v_obs[6] = Vector({1.937, 0.909});
        v_t[7] = 9.;    v_obs[7] = Vector({1.700,-1.121});

        for(auto& obs : v_obs)
          obs.inflate(0.01);


      /* =========== CONTRACTING THE POLYGONS =========== */

        CtcLinobs ctc_linobs(A, b/*, &exp*/);
        vector<ConvexPolygon> polygons_fwdbwd;
        clock_t t_start = clock();
        ctc_linobs.contract(v_t, v_obs, x, u, polygons_fwdbwd, TimePropag::FORWARD | TimePropag::BACKWARD);

        double tube_polygons_volume = 0.;
        for(size_t i = 0 ; i < polygons_fwdbwd.size()-1 ; i++)
        {
          double dt_tk_kp1 = u.slice((int)i)->tdomain().diam();
          tube_polygons_volume += ctc_linobs.polygon_envelope(polygons_fwdbwd[i], dt_tk_kp1, A, b, u.slice((int)i)->codomain()).area().mid() * dt_tk_kp1;
        }

        printf("dt=%f\tTime taken: %.2fs\n", dt, (double)(clock() - t_start)/CLOCKS_PER_SEC);
        if(x.contains(x_truth) != BoolInterval::YES)
          return EXIT_FAILURE;


      /* =========== COMPUTING THE TUBE VOLUME =========== */

        m_pol_total_area[log10(dt)] = tube_polygons_volume;

        double dt_ = tdomain.diam()/(polygons_fwdbwd.size()-1);
        for(size_t i = 0 ; i < polygons_fwdbwd.size() ; i++)
          m_pol_area[dt_].set(polygons_fwdbwd[i].area().mid(), i*dt_);
    }


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigTube fig_area("Area");
    fig_area.set_properties(100, 100, 800, 400);
    IntervalVector view_box({tdomain,Interval(0.)});
    for(auto& [dt,pol_area] : m_pol_area)
    {
      view_box[1] |= pol_area.codomain();
      fig_area.add_trajectory(&pol_area, ("polygons_" + std::to_string(dt)), "#424242");
    }
    fig_area.axis_limits(view_box);
    fig_area.show();

    VIBesFigTube fig_total_area("Total area");
    fig_total_area.set_properties(100, 100, 800, 400);
    Trajectory traj_pol_total_area(m_pol_total_area);
    fig_total_area.add_trajectory(&traj_pol_total_area, "Total polygons area", "#424242");
    fig_total_area.axis_limits(IntervalVector({traj_pol_total_area.tdomain(),traj_pol_total_area.codomain() | 0.}));
    fig_total_area.show();

    vibes::endDrawing();

  // Checking if this example still works:
  return EXIT_SUCCESS;
}