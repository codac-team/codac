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

    Tube x1(domain, tube_dt, Function("t", "100*cos(t)+[80,100]"));
    Tube x2(domain, tube_dt, Function("t", "100*sin(t)+[10,30]"));
    Tube x1dot(domain, tube_dt, Function("t", "-100*sin(t)+[-0.1,0.1]"));
    Tube x2dot(domain, tube_dt, Function("t", "100*cos(t)+[-0.1,0.1]"));
    Tube y = sqrt(sqr(x1) + sqr(x2) + sqr(beacon_depth));
    Tube ydot = (x1*x1dot + x2*x2dot) / y;
    Tube hdot(domain, tube_dt, Function("t", "[0.08,0.12]*t+[0.97,1.08]"));
    Tube h = hdot.primitive();

  /* =========== GRAPHICS =========== */

    Trajectory y_truth(domain, Function("t", "sqrt((90+100*cos(t))^2 + (20+100*sin(t))^2 + 100)"));
    Trajectory h_truth(domain, Function("t", "0.045*t^2+0.98*t"));

    VibesFigure_Tube::draw("Tube [y](·)", &y, &y_truth, 100, 100);
    VibesFigure_Tube::draw("Tube [h](·)", &h, &h_truth, 150, 150);

  /* =========== MEASUREMENTS =========== */

    vector<Interval> v_tau;
    vector<Interval> v_t;
    vector<Interval> v_z;

    v_tau.push_back(1.57);  v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(152.47, 156.47));
    v_tau.push_back(3.34);  v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(34.67, 38.67));
    v_tau.push_back(5.32);  v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(102.38, 106.38));
    v_tau.push_back(7.50);  v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(184.45, 188.45));
    v_tau.push_back(9.88);  v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(167.09, 171.09));
    v_tau.push_back(12.46); v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(60.03, 64.03));
    v_tau.push_back(15.25); v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(78.76, 82.76));
    v_tau.push_back(18.24); v_t.push_back(Interval::ALL_REALS); v_z.push_back(Interval(175.88, 179.88));

  /* =========== CONTRACTIONS =========== */

    bool contraction;
    int k = 1;
    do
    {
      cout << "Computation step " << k << "..." << endl;
      contraction = false;

      for(int i = 0 ; i < v_tau.size() ; i++)
      {
        v_t[i] &= h.invert(v_tau[i]);
        contraction |= y.ctc_eval(v_t[i], v_z[i], ydot);
        contraction |= h.ctc_eval(v_t[i], v_tau[i], hdot);
      }

      k++;
    } while(contraction);

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::draw("Tube [y](·)");
    VibesFigure_Tube::draw("Tube [h](·)");
    VibesFigure_Tube::end_drawing();

  // Checking if this example is still working:
  return (fabs(h.volume() - 9.86763) < 1e-2
       && fabs(y.volume() - 323.63) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}