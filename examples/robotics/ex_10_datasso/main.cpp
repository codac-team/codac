/** 
 *  tubex-lib - Examples
 *  Set-membership state estimation by solving data association 
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Set-membership state estimation by solving data association"
 *              Simon Rohou, Benoît Desrochers, Luc Jaulin
 *              ICRA 2020, Paris
 *
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>

using namespace std;
using namespace ibex;
using namespace tubex;


int main()
{
  /* =========== CREATING DATA =========== */

    const Interval domain(0.,6.);

    // Truth (analytical expression)

      const TrajectoryVector state_truth(domain,
        // Lissajous function (px,py,theta):
        tubex::Function("(240*cos(t) ; \
                          120*sin(2*t) ; \
                          atan2(240*cos(2*t),-240*sin(t)))"));

    // Sets of trajectories

      const double dt = 0.01; // tube timestep

      TubeVector x(domain, dt, 2); // unbounded 2d tube vector

      Tube heading(state_truth[2], dt); // heading computed from truth...
      //heading.inflate(1.2); // ...with some uncertainty

      // Derivatives of positions, with uncertainties:
      TubeVector v(domain, dt,
        // Lissajous derivative
        tubex::Function("(-240*sin(t) ; \
                          240*cos(2*t))"));
      v.inflate(10.);

    // Sets of observations

      // Creating random map of landmarks
      const int nb_landmarks = 150;
      const IntervalVector map_area(2, Interval(-400.,400.));
      const vector<Beacon> v_map = DataLoader::generate_landmarks(map_area, nb_landmarks);

      // Generating observations obs=(t,range,bearing) of these landmarks
      const int max_nb_obs = 50;
      const Interval visi_range(0.,75.); // [0m,75m]
      const Interval visi_angle(-M_PI/4.,M_PI/4.); // frontal sonar
      vector<IntervalVector> v_obs =
        DataLoader::generate_observations(state_truth, v_map, max_nb_obs, visi_range, visi_angle);

      // Adding uncertainties on the measurements
      for(auto& obs : v_obs) 
      {
        obs[1].inflate(2.); // range
        obs[2].inflate(0.1); // bearing
      }

      // Association set
      vector<IntervalVector> m(v_obs.size(), IntervalVector(2)); // unknown association
    

  /* =========== CREATING CONTRACTORS =========== */

    ibex::CtcFwdBwd ctc_plus(*new ibex::Function("a", "b", "c", "b+c-a")); // algebraic constraint a=b+c
    ibex::CtcFwdBwd ctc_minus(*new ibex::Function("a", "b", "c", "b-c-a")); // algebraic constraint a=b-c

    pyibex::CtcPolar ctc_polar; // polar constraint (px,py,rho,phi)

    CtcConstellation ctc_constell(v_map); // constellation constraint

    CtcDeriv ctc_deriv; // \dot{x}=v constraint

    CtcEval ctc_eval; // p=x(t_i) constraint
    ctc_eval.enable_temporal_propagation(false); // faster use // todo: remove this


  /* =========== CONTRACTOR NETWORK =========== */

    ContractorNetwork cn;
    cn.add(ctc_deriv, x, v); // (i)

    for(int i = 0 ; i < v_obs.size() ; i++)
    {
      // Measurement i
      Interval &t  = v_obs[i][0]; // time
      Interval &y1 = v_obs[i][1]; // range
      Interval &y2 = v_obs[i][2]; // bearing

      // Intermediate variables:
      Interval& psi = cn.create_var(Interval()); // robot heading
      Interval& a = cn.create_var(Interval());
      IntervalVector& d = cn.create_var(IntervalVector(2));
      IntervalVector& p = cn.create_var(IntervalVector(2));
      
      cn.add(ctc_constell, m[i]); // (ii)
      cn.add(ctc_minus, d, m[i], p); // (iii)
      cn.add(ctc_plus, a, psi, y2); // (iv)
      cn.add(ctc_polar, d[0], d[1], y1, a); // (v)
      cn.add(ctc_eval, t, p, x, v); // (vi)
      cn.add(ctc_eval, t, psi, heading); // (vii)
    }

    cn.contract();


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 1000, 600);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&state_truth, "x*", 0, 1, 2, "white");
    fig_map.add_observations(v_obs, &state_truth);
    fig_map.add_beacons(v_map, 2.);
    fig_map.smooth_tube_drawing();
    fig_map.show();
    fig_map.axis_limits(-340., 340., 0., 0., true);
    

  /* =========== ENDING =========== */

    cout << endl << v_map.size() << " landmarks" << endl;
    int identified = 0;
    for(int i = 0 ; i < m.size() ; i++)
      if(m[i].volume() == 0 && !m[i].is_empty()) // degenerate box
      {
        identified ++;
        fig_map.add_beacon(m[i], 2., "#00A53B[#00A53B]");
      }
    cout << identified << "/" << m.size() << " observations identified" << endl << endl;
    fig_map.show();
  
  vibes::endDrawing();
  // Checking if this example still works:
  bool success = x.contains(state_truth.subvector(0,1)) == YES;
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}