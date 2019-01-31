/* ============================================================================
 *  tubex-lib - Example: robot localization among low-cost beacons
 *
 *  Example from the paper "Reliable Non-Linear State Estimation Involving Time Uncertainties"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  /* =========== INITIALIZATION =========== */

    float tube_dt = 0.01;
    Interval domain(0., 64.);

    TubeVector x(domain, tube_dt, 4); // state vector

    IntervalVector x0(4);
    x0[2] = Interval(M_PI/2.).inflate(0.01);
    x0[3] = Interval(0.).inflate(0.01);
    x.set(x0, 0.);

  /* =========== DATA (MAP + MEASUREMENTS) =========== */

    vector<Beacon> v_beacons;
    v_beacons.push_back(Beacon(30.,20.));
    v_beacons.push_back(Beacon(80.,-5.));
    v_beacons.push_back(Beacon(125.,20.));

    IntervalVector ti_zi(2);
    map<int,vector<IntervalVector> > m_obs;
    ti_zi[0] = Interval(14.75, 15.55); ti_zi[1] = Interval(11.69, 12.69); m_obs[1].push_back(ti_zi);
    ti_zi[0] = Interval(20.8, 21.6);   ti_zi[1] = Interval(15.4, 16.4);   m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(23.8, 24.6);   ti_zi[1] = Interval(10.62, 11.62); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(26.8, 27.6);   ti_zi[1] = Interval(11.05, 12.05); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(29.8, 30.6);   ti_zi[1] = Interval(11.87, 12.87); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(32.8, 33.6);   ti_zi[1] = Interval(15.31, 16.31); m_obs[0].push_back(ti_zi);
    ti_zi[0] = Interval(44.35, 45.15); ti_zi[1] = Interval(13.65, 14.65); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(47.35, 48.15); ti_zi[1] = Interval(13.32, 14.32); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(50.35, 51.15); ti_zi[1] = Interval(12.03, 13.03); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(53.35, 54.15); ti_zi[1] = Interval(15.98, 16.98); m_obs[2].push_back(ti_zi);
    ti_zi[0] = Interval(56.75, 57.55); ti_zi[1] = Interval(17.45, 18.45); m_obs[1].push_back(ti_zi);

  /* =========== STATE ESTIMATION =========== */

    CtcEval ctc_eval;
    CtcDeriv ctc_deriv;

    ctc_eval.set_fast_mode(true);
    ctc_deriv.set_fast_mode(true);

    const char* xdyn[8];
    xdyn[0] = "x"; xdyn[1] = "y";
    xdyn[2] = "phi"; xdyn[3] = "v";
    xdyn[4] = "bx"; xdyn[5] = "by";
    xdyn[6] = "g"; xdyn[7] = "gdot";

    tubex::Function f_obs(8, xdyn, "(sqrt((x-bx)^2+(y-by)^2) - g ; \
                                     v*cos(phi)*sign(x-bx)/sqrt(1+((y-by)^2)/((x-bx)^2)) \
                                   + v*sin(phi)*sign(y-by)/sqrt(1+((x-bx)^2)/((y-by)^2)) - gdot)");
    tubex::Function f_evol(8, xdyn, "(v*cos(phi) ; \
                                      v*sin(phi) ; \
                                      -0.45 * cos(0.2*t) + [-0.001,0.001] ; \
                                      0.1 + sin(0.25*t) + [-0.001,0.001])");

    tubex::CtcFwdBwd ctc_obs_fwdbwd(f_obs);

    map<int,TubeVector*> m_x;
    for(int i = 0 ; i < v_beacons.size() ; i++)
    {
      m_x[i] = new TubeVector(x);
      m_x[i]->resize(8);
      (*m_x[i]) &= IntervalVector(8, Interval(-999.,999.)); // todo: remove this

      (*m_x[i])[4].set(v_beacons[i].pos_box()[0]);
      (*m_x[i])[5].set(v_beacons[i].pos_box()[1]);
    }

    int a = 0;
    double volume_x;

    do
    {
      a++;
      volume_x = x.volume();
      cout << "Iteration " << a << endl;

      for(int i = 0 ; i < v_beacons.size() ; i++)
      {
        cout << "  - seamark " << (i+1) << endl;

        for(int j = 0 ; j < 4 ; j++)
          (*m_x[i])[j] &= x[j];

        for(int k = 0 ; k < m_obs[i].size() ; k++)
          ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[6], (*m_x[i])[7]);

        ctc_obs_fwdbwd.contract(*m_x[i]);

        TubeVector xdot = f_evol.eval_vector((*m_x[i]));
        for(int j = 0 ; j < xdot.size() ; j++)
          ctc_deriv.contract((*m_x[i])[j], xdot[j]);

        for(int j = 0 ; j < 4 ; j++)
          x[j] &= (*m_x[i])[j];
      }

    } while(a < 4 && volume_x != x.volume());

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VibesFigure_Map fig_map("Map");
    fig_map.set_properties(50, 50, 550, 350);
    fig_map.add_tubevector(&x, "x", 0, 1);
    //fig_map.add_trajectoryvector(&x_truth, "x*", 1, 0, "white");
    fig_map.add_beacons(v_beacons);
    fig_map.show(2.5);

    vibes::endDrawing();


  // Checking if this example still works:
  return (fabs(x.volume() - 1354.26) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
  // Truth

  TrajectoryVector x_truth(8);
  x_truth.set(domain.lb(), Vector(8, 0.));
  double simu_dt = 0.01;
  double prev_t = domain.lb();
  for(double t = domain.lb() + simu_dt ; t < domain.ub() ; t+= simu_dt)
  {
    Vector x_temp(9, t);
    x_temp.put(1, x_truth(prev_t));

    Vector x_current(8, 0.);
    x_current.put(0, f_evol.eval_vector(x_temp).mid());
    x_truth.set(t, x_truth(prev_t) + simu_dt * x_current);
    prev_t = t;
  }

  for(double t = domain.lb() + simu_dt ; t < domain.ub() ; t+= simu_dt)
  {
    Vector vect_x = x_truth(t);
    vect_x[1] = -vect_x[1];
    x_truth.set(t, vect_x);
  }
*/