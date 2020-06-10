/** 
 *  tubex-lib - Examples
 *  Dynamic range-bearing localization
 * ----------------------------------------------------------------------------
 *
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-pyibex.h>

using namespace std;
using namespace tubex;
using namespace pyibex;

int main()
{
  /* =================== 0. Parameters, truth and data ==================== */

    float dt = 0.05;                                // timestep for simulation/tubes accuracy
    Interval tdomain(0.,3.);                        // temporal limits [t_0,t_f]=[0,3]

    TrajectoryVector x_truth(tdomain, TFunction("( \
      10*cos(t)+t ; \
      5*sin(2*t)+t ; \
      atan2((10*cos(2*t)+1),(-10*sin(t)+1)) ; \
      sqrt((-10*sin(t)+1)^2+(10*cos(2*t)+1)^2))")); // actual trajectory

    // Continuous measurements coming from the truth
    Trajectory& measured_psi = x_truth[2].sample(dt).make_continuous();
    measured_psi += RandTrajectory(tdomain, dt, Interval(-0.01,0.01)); // adding some noise
    Trajectory& measured_speed = x_truth[3].sample(dt);
    measured_speed += RandTrajectory(tdomain, dt, Interval(-0.01,0.01)); // adding some noise

    // Creating random map of landmarks
    int nb_landmarks = 30;
    IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

    // The following function generates a set of [t]x[range]x[bearing]x[beacon_pos] values
    int nb_obs = 10;
    vector<IntervalVector> v_obs = DataLoader::generate_observations_along_traj(x_truth, v_map, nb_obs);

    // Adding uncertainties on the measurements
    for(auto& obs : v_obs) // for each observation:
    {
      obs[1].inflate(0.3); // range
      obs[2].inflate(0.1); // bearing
    }


  /* =============== 1. Defining domains for our variables ================ */

    TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
    TubeVector v(tdomain, dt, 4);                     // 4d tube for derivatives of the states
    TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

    x[2] = Tube(measured_psi, dt).inflate(0.01);      // measured_psi is a set of measurements
    x[3] = Tube(measured_speed, dt).inflate(0.01);


  /* =========== 2. Defining contractors to deal with equations =========== */

    CtcFunction ctc_f(
      Function("v[4]", "x[4]", "u[2]",
               "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"));

    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c")); // a+b=c
    CtcFunction ctc_minus(Function("a", "b", "c", "a-b-c")); // a-b=c
    // We also use the predefined contractors ctc::polar, ctc::eval, no need to build them

  /* =============== 3. Adding the contractors to a network =============== */

    ContractorNetwork cn;        // creating a network
    cn.add(ctc_f, {v, x, u});    // adding the f constraint

    for(auto& y : v_obs)
    {
      // Intermediate variables
      Interval& alpha = cn.create_dom(Interval()); // absolute angle robot-landmark
      IntervalVector& d = cn.create_dom(IntervalVector(2)); // dist robot-landmark
      IntervalVector& p = cn.create_dom(IntervalVector(4)); // state at t_i

      cn.add(ctc_plus, {y[2], p[2], alpha});
      cn.add(ctc_minus, {cn.subvector(y,3,4), cn.subvector(p,0,1), d});
      cn.add(ctc::polar, {d, y[1], alpha});
      cn.add(ctc::eval, {y[0], p, x, v});
    }


  /* ======================= 4. Solving the problem ======================= */

    cn.contract(true);


  /* ============================ 5. Graphics ============================= */

    vibes::beginDrawing();
    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 900, 550);
    fig.add_tube(&x, "x", 0, 1);
    fig.add_trajectory(&x_truth, "x*", 0, 1, 2);
    fig.smooth_tube_drawing(true);

    for(const auto& b : v_map)
      fig.add_beacon(b.mid(), 0.2); // drawing beacons

    for(const auto& y : v_obs)
    {
      double t_obs = y[0].mid();
      Vector t_state = x_truth(t_obs);
      fig.draw_pie(t_state[0], t_state[1], y[1] | Interval(0.01), t_state[2] + y[2], "lightGray"); // drawing range-bearing measurements
      fig.draw_pie(t_state[0], t_state[1], y[1], t_state[2] + y[2], "darkGray"); // drawing range-bearing measurements
      fig.draw_vehicle(t_obs, &x_truth, 0.7); // drawing robot position at t
    }

    fig.show(0.);
    vibes::endDrawing();
    return x.contains(x_truth) != BoolInterval::NO ? EXIT_SUCCESS : EXIT_FAILURE;
}