/* ============================================================================
 *  tubex-lib - Drifting clock example
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
  /* =========== PARAMETERS =========== */

    double tube_dt = 0.005;
    Interval domain(0., 13.5);
    float beacon_depth = 10.;

  /* =========== INITIALIZATION =========== */

    TubeVector x(domain, 4);
    x[0] = Tube(domain, tube_dt, tubex::Function("100*cos(t)+[80,100]"));
    x[1] = Tube(domain, tube_dt, tubex::Function("100*sin(t)+[10,30]"));
    x[2] = Tube(domain, tube_dt, tubex::Function("-100*sin(t)+[-0.1,0.1]"));
    x[3] = Tube(domain, tube_dt, tubex::Function("100*cos(t)+[-0.1,0.1]"));
    
    TubeVector y(domain, 2);
    y[0] = sqrt(sqr(x[0]) + sqr(x[1]) + sqr(beacon_depth));
    y[1] = (x[0]*x[2] + x[1]*x[3]) / y[0];
    
    TubeVector h(domain, 2);
    h[1] = Tube(domain, tube_dt, tubex::Function("[0.08,0.12]*t+[0.97,1.08]"));
    h[0] = h[1].primitive();

  /* =========== GRAPHICS =========== */

    Trajectory y_truth(domain, tubex::Function("sqrt((90+100*cos(t))^2 + (20+100*sin(t))^2 + 100)"));
    Trajectory h_truth(domain, tubex::Function("0.045*t^2+0.98*t"));

    vibes::beginDrawing();

    VibesFigure_Tube fig_y("Tube [y](·)", &y[0], &y_truth);
    fig_y.set_properties(100, 100, 500, 250);
    fig_y.show();

    VibesFigure_Tube fig_h("Tube [h](·)", &h[0], &h_truth);
    fig_h.set_properties(150, 150, 500, 250);
    fig_h.show();

  /* =========== MEASUREMENTS =========== */

    IntervalVector obs(3);
    vector<IntervalVector> v_obs; // (tau, t, z)
    obs[0] = 1.57;  obs[2] = Interval(152.47, 156.47); v_obs.push_back(obs);
    obs[0] = 3.34;  obs[2] = Interval(34.67, 38.67); v_obs.push_back(obs);
    obs[0] = 5.32;  obs[2] = Interval(102.38, 106.38); v_obs.push_back(obs);
    obs[0] = 7.50;  obs[2] = Interval(184.45, 188.45); v_obs.push_back(obs);
    obs[0] = 9.88;  obs[2] = Interval(167.09, 171.09); v_obs.push_back(obs);
    obs[0] = 12.46; obs[2] = Interval(60.03, 64.03); v_obs.push_back(obs);
    obs[0] = 15.25; obs[2] = Interval(78.76, 82.76); v_obs.push_back(obs);
    obs[0] = 18.24; obs[2] = Interval(175.88, 179.88); v_obs.push_back(obs);
    // nb: obs[1] set to ALL_REALS by default
  
  /* =========== CONTRACTIONS =========== */

    CtcDeriv ctc_deriv;
    CtcEval ctc_eval(false, false);
    bool contraction;
    int k = 1;
    do
    {
      cout << "Computation step " << k << "..." << endl;
      contraction = false;

      for(int i = 0 ; i < v_obs.size() ; i++)
      {
        contraction |= ctc_eval.contract(v_obs[i][1], v_obs[i][0], h[0], h[1]);
        contraction |= ctc_eval.contract(v_obs[i][1], v_obs[i][2], y[0], y[1]);
        contraction |= ctc_eval.contract(v_obs[i][1], v_obs[i][0], h[0], h[1]);
      }

      contraction |= ctc_deriv.contract(h[0], h[1]);
      contraction |= ctc_deriv.contract(y[0], y[1]);
      k++;
    } while(contraction);

  /* =========== GRAPHICS =========== */

    fig_y.show();
    fig_h.show();
    vibes::endDrawing();

  // Checking if this example is still working:
  return (y[0].encloses(y_truth) && h[0].encloses(h_truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}