/** 
 *  Codac - Examples
 *  Contractors on tubes: temporal contractors
 * ----------------------------------------------------------------------------
 *
 *  \brief      Comparison between the Lohner & Picard temporal contractors
 *
 *  \date       2020
 *  \author     Auguste Bourgois
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main() // testcase
{
  /* =========== INITIALIZATION =========== */

    double dt = 0.1; // time step of the tubes
    Interval tdomain(0, 5); // time domain of the tubes

    TubeVector x(tdomain, dt, 1); // 1d tube vector
    x.set(IntervalVector(1, Interval(0.9, 1.1)), 0.); // initial condition
    TubeVector y(x); // copy of x

    vibes::beginDrawing();


  /* =========== COMPARISON OF CONTRACTORS =========== */

    Function f("x", "-sin(x)"); // xdot = -sin(x)

    // Picard contractor

    CtcPicard ctc_picard(f);
    ctc_picard.contract(x);

    VIBesFigTube fig1("Picard");
    fig1.set_properties(100, 100, 800, 400);
    fig1.add_tube(&x[0], "x");

    // Lohner contractor

    CtcLohner ctc_lohner(f);
    ctc_lohner.contract(y);

    VIBesFigTube fig2("Lohner");
    fig2.set_properties(100, 550, 800, 400);
    fig2.add_tube(&y[0], "y");


  /* =========== COMPUTING TRAJECTORIES AS ILLUSTRATION =========== */

    std::vector<Trajectory> v_traj(11);

    for (int i = 0; i < 11; ++i) {
      double x0 = 0.9 + 0.199 * (i / 10.); // initial conditions in [x0]=[0.9,1.1]
      v_traj[i] = Trajectory(tdomain,
        TFunction((std::to_string(x0) + "*2.*atan(exp(-t)*tan(0.5))").c_str()));
      fig1.add_trajectory(&v_traj[i], "truth" + std::to_string(i));
      fig2.add_trajectory(&v_traj[i], "truth" + std::to_string(i));
    }

    fig1.show(true);
    fig2.show(true);
    fig1.axis_limits(fig2.view_box());

    vibes::endDrawing();


  /* =========== ENDING =========== */

    // Checking if this example still works:

    for (const auto& traj_i : v_traj) {
      if(x[0].contains(traj_i) == BoolInterval::NO)
        {cout << "x" << traj_i(0.) << x[0](0.) << endl;
        return EXIT_FAILURE;
      }
      if(y[0].contains(traj_i) == BoolInterval::NO)
        {cout << "y" << traj_i(0.) << y[0](0.) << endl;
        return EXIT_FAILURE;
      }
    }
    
  return fabs(x[0].volume() - 13.9148) < 1e-2
    && fabs(y[0].volume() - 0.43186) < 1e-2 ?
      EXIT_SUCCESS : EXIT_FAILURE;
}