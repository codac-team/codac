/** 
 *  Codac - Examples
 *  Mobile robot localization evolving among range-only beacons
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Reliable non-linear state estimation involving time uncertainties"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;

int main()
{
  /* =========== INITIALIZATION =========== */

    float dt = 0.01;
    Interval tdomain(0., 64.);

    // Tube state vector
      TubeVector x(tdomain, dt, 4);

    // Initial state x0
      Vector x0(4, 0.);
      x0[2] = M_PI/2.;
      x0[3] = 0.;

    // Bounded initial state x0 (with uncertainties)
      IntervalVector ix0(x0);
      ix0[0] = Interval::ALL_REALS; // no knowledge on initial position
      ix0[1] = Interval::ALL_REALS;
      ix0[2].inflate(0.01);
      ix0[3].inflate(0.01);
      x.set(ix0, 0.); // tube state vector with initial condition on v, phi

    // Computing an approximation of the actual state trajectory
      Trajectory traj_vdot(tdomain, codac::Function("0.1 + sin(0.25*t)"), dt);
      Trajectory traj_phidot(tdomain, codac::Function("-0.45 * cos(0.2*t)"), dt);

      TrajectoryVector traj_state(4); // state equations
      traj_state[2] = traj_phidot.primitive(x0[2]);
      traj_state[3] = traj_vdot.primitive(x0[3]);
      traj_state[0] = (traj_state[3]*cos(traj_state[2])).primitive();
      traj_state[1] = (traj_state[3]*sin(traj_state[2])).primitive();


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

    codac::Function f_obs(8, xdyn, "(sqrt((x-bx)^2+(y-by)^2) - g ; \
                                     v*cos(phi)*sign(x-bx)/sqrt(1+((y-by)^2)/((x-bx)^2)) \
                                   + v*sin(phi)*sign(y-by)/sqrt(1+((x-bx)^2)/((y-by)^2)) - gdot)");
    codac::Function f_evol(8, xdyn, "(v*cos(phi) ; \
                                      v*sin(phi) ; \
                                      -0.45 * cos(0.2*t) + [-0.001,0.001] ; \
                                      0.1 + sin(0.25*t) + [-0.001,0.001])");

    codac::CtcFwdBwd ctc_obs_fwdbwd(f_obs);

    map<int,TubeVector*> m_x;
    for(size_t i = 0 ; i < v_beacons.size() ; i++)
    {
      m_x[i] = new TubeVector(x);
      m_x[i]->resize(8);
      (*m_x[i]) &= IntervalVector(8, Interval(-999.,999.)); // todo: remove this

      (*m_x[i])[4].set(v_beacons[i].pos_box()[0]);
      (*m_x[i])[5].set(v_beacons[i].pos_box()[1]);
    }

    size_t a = 0;
    double volume_x;

    do
    {
      a++;
      volume_x = x.volume();
      cout << "Iteration " << a << endl;

      for(size_t i = 0 ; i < v_beacons.size() ; i++)
      {
        cout << "  - seamark " << (i+1) << endl;

        for(int j = 0 ; j < 4 ; j++)
          (*m_x[i])[j] &= x[j];

        for(size_t k = 0 ; k < m_obs[i].size() ; k++)
          ctc_eval.contract(m_obs[i][k][0], m_obs[i][k][1], (*m_x[i])[6], (*m_x[i])[7]);

        ctc_obs_fwdbwd.contract(*m_x[i]);

        TubeVector xdot = f_evol.eval_vector((*m_x[i]));
        for(int j = 0 ; j < xdot.size() ; j++)
        {
          ctc_deriv.contract((*m_x[i])[j], xdot[j]);
          x[j] &= (*m_x[i])[j];
        }
      }

    } while(a < 4 && volume_x != x.volume());


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 550, 350);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&traj_state, "x*", 0, 1, 2, "white");
    fig_map.add_beacons(v_beacons);
    fig_map.show(2.5);

    vibes::endDrawing();

  // Checking if this example still works:
  return (fabs(x.volume() - 1367.94) < 1e-2
    && x.subvector(0,3).contains(traj_state) == BoolInterval::YES) ? EXIT_SUCCESS : EXIT_FAILURE;
}