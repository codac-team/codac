/** 
 *  Codac - Examples
 *  Set-membership state estimation by solving data association 
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Set-membership state estimation by solving data association"
 *              Simon Rohou, Benoît Desrochers, Luc Jaulin
 *              ICRA 2020, Paris
 *
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>
#include <codac-pyibex.h>

using namespace std;
using namespace codac;


int main()
{
  /* =========== CREATING DATA =========== */

    double dt = 0.02;
    Interval tdomain(0.,6.);

    // Truth (analytic function)

      TrajectoryVector state_truth(tdomain,
        // Lissajous function (px,py,theta):
        TFunction("(240*cos(t) ; \
                    120*sin(2*t) ; \
                    atan2(240*cos(2*t),-240*sin(t)))"));
      state_truth[2].sample(dt).make_continuous();

    // Sets of trajectories

      TubeVector x(tdomain, dt, 3); // unbounded 2d tube vector
      x[2] = Tube(state_truth[2], dt); // heading computed from truth...
      //x[2].inflate(1.2); // ...with some uncertainty

      // Derivatives of positions, with uncertainties:
      TubeVector v(tdomain, dt,
        // Lissajous derivative
        TFunction("(-240*sin(t) ; \
                    240*cos(2*t))"));
      v.inflate(10.);
      v.resize(3);

    // Sets of observations

      // Creating random map of landmarks
      int nb_landmarks = 150;
      IntervalVector map_area(2, Interval(-400.,400.));
      vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

      // Generating observations obs=(t,range,bearing) of these landmarks
      int max_nb_obs = 50;
      Interval visi_range(0.,75.); // [0m,75m]
      Interval visi_angle(-M_PI/4.,M_PI/4.); // frontal sonar
      vector<IntervalVector> v_obs =
        DataLoader::generate_observations(state_truth, v_map, max_nb_obs, true, visi_range, visi_angle);

      // Adding uncertainties on the measurements
      for(auto& obs : v_obs) 
      {
        obs[1].inflate(2.); // range
        obs[2].inflate(0.1); // bearing
      }

      // Association set
      vector<IntervalVector> m(v_obs.size(), IntervalVector(2)); // unknown association
    

  /* =========== CUSTOM-BUILT CONTRACTORS =========== */

    CtcFunction ctc_plus(Function("a", "b", "c", "a-(b+c)")); // algebraic constraint a=b+c
    CtcFunction ctc_minus(Function("a", "b", "c", "a-(b-c)")); // algebraic constraint a=b-c
    CtcConstell ctc_constell(v_map); // constellation constraint


  /* =========== CONTRACTOR NETWORK =========== */

    ContractorNetwork cn;

    for(size_t i = 0 ; i < v_obs.size() ; i++)
    {
      // Measurement i
      Interval &t  = v_obs[i][0]; // time
      Interval &y1 = v_obs[i][1]; // range
      Interval &y2 = v_obs[i][2]; // bearing

      // Intermediate variables:
      Interval& a = cn.create_interm_var(Interval());
      IntervalVector& d = cn.create_interm_var(IntervalVector(2));
      IntervalVector& p = cn.create_interm_var(IntervalVector(3));
      
      cn.add(ctc_constell, {m[i]});
      cn.add(ctc_minus, {d, m[i], cn.subvector(p,0,1)});
      cn.add(ctc_plus, {a, p[2], y2});
      cn.add(ctc::polar, {d[0], d[1], y1, a});
      cn.add(ctc::eval, {t, p, x, v});
    }

    //cn.set_fixedpoint_ratio(0.);
    cn.contract(true);


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 1000, 600);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&state_truth, "x*", 0, 1, 2, "white");
    fig_map.add_observations(v_obs, &state_truth);
    for(const auto& b : v_map)
      fig_map.add_beacon(Beacon(b), 2.);
    fig_map.smooth_tube_drawing(true);
    fig_map.show();
    fig_map.axis_limits(-340., 340., -1., 1., true);
    

  /* =========== ENDING =========== */

    cout << endl << v_map.size() << " landmarks" << endl;
    int identified = 0;
    for(size_t i = 0 ; i < m.size() ; i++)
      if(m[i].volume() == 0 && !m[i].is_empty()) // degenerate box
      {
        identified ++;
        fig_map.add_beacon(m[i], 2., "#00A53B[#00A53B]");
      }
    cout << identified << "/" << m.size() << " observations identified" << endl << endl;
    fig_map.show(20.);
  
  vibes::endDrawing();
  // Checking if this example still works:
  bool success = x.contains(state_truth) == BoolInterval::YES;
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}