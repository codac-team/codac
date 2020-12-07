/** 
 *  tubex-lib - Examples
 *  Driftint clock
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Reliable non-linear state estimation involving time uncertainties"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>

using namespace std;
using namespace tubex;

int main()
{
  /* =========== PARAMETERS =========== */

    double tube_dt = 0.005;
    Interval tdomain(0., 13.5);
    float beacon_depth = 10.;

  /* =========== INITIALIZATION =========== */

    TubeVector x(tdomain, 4);
    x[0] = Tube(tdomain, tube_dt, TFunction("100*cos(t)+[80,100]"));
    x[1] = Tube(tdomain, tube_dt, TFunction("100*sin(t)+[10,30]"));
    x[2] = Tube(tdomain, tube_dt, TFunction("-100*sin(t)+[-0.1,0.1]"));
    x[3] = Tube(tdomain, tube_dt, TFunction("100*cos(t)+[-0.1,0.1]"));
    
    TubeVector y(tdomain, 2);
    y[0] = sqrt(sqr(x[0]) + sqr(x[1]) + ibex::sqr(beacon_depth));
    y[1] = (x[0]*x[2] + x[1]*x[3]) / y[0];
    
    TubeVector h(tdomain, 2);
    h[1] = Tube(tdomain, tube_dt, TFunction("[0.08,0.12]*t+[0.97,1.08]"));
    h[0] = h[1].primitive();

  /* =========== GRAPHICS =========== */

    Trajectory y_truth(tdomain, TFunction("sqrt((90+100*cos(t))^2 + (20+100*sin(t))^2 + 100)"));
    Trajectory h_truth(tdomain, TFunction("0.045*t^2+0.98*t"));

    vibes::beginDrawing();

    VIBesFigTube fig_y("Tube [y](·)", &y[0], &y_truth);
    fig_y.set_properties(100, 100, 500, 250);
    fig_y.show();

    VIBesFigTube fig_h("Tube [h](·)", &h[0], &h_truth);
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
    // nb: obs[1] set to [-oo,oo] by default
  
  /* =========== CONTRACTIONS =========== */

    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;
    ctc_eval.enable_time_propag(false); // faster use

    int k = 0;
    double volume;

    do
    {
      k++;
      volume = y.volume() + h.volume();
      cout << "Computation step " << k << "..." << endl;

      for(size_t i = 0 ; i < v_obs.size() ; i++)
      {
        ctc_eval.contract(v_obs[i][1], v_obs[i][0], h[0], h[1]);
        ctc_eval.contract(v_obs[i][1], v_obs[i][2], y[0], y[1]);
        ctc_eval.contract(v_obs[i][1], v_obs[i][0], h[0], h[1]);
      }

      ctc_deriv.contract(h[0], h[1]);
      ctc_deriv.contract(y[0], y[1]);

    } while(volume != y.volume() + h.volume());

  /* =========== GRAPHICS =========== */

    fig_y.show();
    fig_h.show();
    vibes::endDrawing();

  // Checking if this example still works:
  return (y[0].contains(y_truth) == BoolInterval::YES && h[0].contains(h_truth) == BoolInterval::YES) ? EXIT_SUCCESS : EXIT_FAILURE;
}