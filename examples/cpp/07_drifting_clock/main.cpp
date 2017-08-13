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

#include "Tube.h"
#include "VibesFigure_Tube.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char *argv[])
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
    Tube h = hdot.primitive(Interval(0.));

  /* =========== SIMULATION (UNKNOWN TRUTH) =========== */

    map<double,double> map_y, map_h;
    for(float t = domain.lb() ; t < domain.ub() ; t+=0.001)
    {
      map_h[t] = 0.045*pow(t,2) + 0.98*t;
      map_y[t] = sqrt(pow(90+100*cos(t),2) + pow(20+100*sin(t),2) + pow(beacon_depth,2));
    }

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::show(&y, "Tube [y](·)", map_y, 100, 100);
    VibesFigure_Tube::show(&h, "Tube [h](·)", map_h, 150, 150);

  /* =========== MEASUREMENTS =========== */

    vector<float> v_tau;
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
    int k = 0;
    do
    {
      cout << "Computation step " << (k+1) << "..." << endl;
      contraction = false;

      for(int i = 0 ; i < v_tau.size() ; i++)
      {
        v_t[i] &= h.invert(v_tau[i]);
        contraction |= y.ctcObs(ydot, v_t[i], v_z[i]);
        contraction |= h.ctcObs(hdot, v_t[i], v_tau[i]);
      }

      k++;
    } while(contraction);

    VibesFigure_Tube::show(&y, "Tube [y](·)");
    VibesFigure_Tube::show(&h, "Tube [h](·)");

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::endDrawing();

  // Checking if this example is still working:
  return (fabs(h.volume() - 9.86763) < 1e-2
       && fabs(y.volume() - 323.63) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}