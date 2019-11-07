/** 
 *  tubex-lib - Examples
 *  Simple asynchronous dynamic localization
 * ----------------------------------------------------------------------------
 *
 *  \brief      Simple example
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

double dist(const TrajectoryVector& x, const Beacon& b, double t)
{
  return sqrt(pow(x[0](t) - b.x(), 2) + pow(x[1](t) - b.y(), 2));
}

int main(int argc, char** argv)
{
  /* =========== LOADING DATA =========== */

    float dt = 0.01;
    Interval domain(0., 3.);
    TrajectoryVector x_truth(domain, tubex::Function("(10*cos(t)+t;5*sin(2*t)+t)")); // actual trajectory
    TrajectoryVector v_truth(domain, tubex::Function("(-10*sin(t)+1;10*cos(2*t)+1)")); // actual velocity

    // Building beacons and related range-only measurements
    float r = 0.1;
    Beacon b[3]; b[0] = Beacon(8.,3.); b[1] = Beacon(0.,5.); b[2] = Beacon(-2.,1.);
    double t[3]; t[0] = 0.3; t[1] = 1.5; t[2] = 2.0; // times of measurements
    Interval d[3]; for(int i = 0 ; i < 3 ; i++) d[i] = dist(x_truth, b[i], t[i]) + Interval(-r,r); // range-only observations

    TubeVector x(domain, dt, 2); // no initial condition on x
    TubeVector v(v_truth, dt); // velocities from the analytical expression
    v.inflate(Vector(2,0.01)); // some uncertainties added on the velocities

  /* =========== SOLVING =========== */

    CtcEval ctc_eval; // differential observation contractor
    ctc_eval.preserve_slicing(true); // the contraction will not change the slicing
    ibex::Function f("x", "y", "bx", "by", "range", "sqrt((x-bx)^2+(y-by)^2)-range"); // range-only observation function
    ibex::CtcFwdBwd ctc_rangeonly(f); // static contractor

    double vol;

    do
    {
      vol = x.volume();

      for(int i = 0 ; i < 3 ; i++)
      {
        IntervalVector m(5); // preparing static contraction in a 5d box
        m[0] = x[0](t[i]); m[1] = x[1](t[i]);
        m[2] = b[i].x(); m[3] = b[i].y();
        m[4] = d[i];
        ctc_rangeonly.contract(m);
        // Propagating the static contraction to tubes:
        ctc_eval.contract(t[i], m[0], x[0], v[0]);
        ctc_eval.contract(t[i], m[1], x[1], v[1]);
      }
    } while(fabs(x.volume() / vol) < 0.01); // will a fixed point has not been reached


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 900, 550);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
    fig_map.smooth_tube_drawing(true);

    for(int i = 0 ; i < 3 ; i++)
    {
      // Drawing beacons
      fig_map.add_beacon(b[i], 0.2, "#D65A00[#F88900]");
      // Drawing range-only measurements
      fig_map.draw_circle(b[i].x(), b[i].y(), dist(x_truth, b[i], t[i]) - r, "#8D8D8D");
      fig_map.draw_circle(b[i].x(), b[i].y(), dist(x_truth, b[i], t[i]) + r, "#8D8D8D");
      // Drawing robot position at t
      fig_map.draw_vehicle(t[i], &x_truth, 0.8);
    }

    fig_map.show(0.);
    vibes::endDrawing();

  /* =========== ENDING =========== */

  // Checking if this example still works:
  return (fabs(x.volume() - 2.62768) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}
