/** 
 *  Codac - Examples
 *  Static range-bearing localization
 * ----------------------------------------------------------------------------
 *
 *  \date       2020
 *  \author     Simon Rohou
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
  /* =================== 0. Parameters, truth and data ==================== */

    // Truth (unknown pose)
    Vector x_truth({0.,0.,M_PI/6.}); // (x,y,heading)

    // Creating random map of landmarks
    int nb_landmarks = 1;
    IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);
    
    // The following function generates a set of [range]x[bearing] values
    vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, v_map, false);

    // Adding uncertainties on the measurements
    for(auto& y : v_obs) // for each observation:
    {
      y[0].inflate(0.3); // range
      y[1].inflate(0.1); // bearing
    }


  /* =============== 1. Defining domains for our variables ================ */

    IntervalVector x(2); // unknown position
    Interval heading = Interval(x_truth[2]).inflate(0.01); // measured heading


  /* =========== 2. Defining contractors to deal with equations =========== */

    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c")); // a+b=c
    CtcFunction ctc_minus(Function("a", "b", "c", "a-b-c")); // a-b=c
    // We also use the predefined contractor ctc::polar, no need to build it


  /* =============== 3. Adding the contractors to a network =============== */

    ContractorNetwork cn;

    for(int i = 0 ; i < v_obs.size() ; i++)
    {
      // Intermediate variables
      Interval& alpha = cn.create_interm_var(Interval());
      IntervalVector& d = cn.create_interm_var(IntervalVector(2));

      cn.add(ctc_plus, {v_obs[i][1], heading, alpha});
      cn.add(ctc_minus, {v_map[i], x, d});
      cn.add(ctc::polar, {d, v_obs[i][0], alpha});
    }


  /* ======================= 4. Solving the problem ======================= */

    cn.contract();


  /* ============================ 5. Graphics ============================= */

    vibes::beginDrawing();

    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 600, 600);

    for(const auto& iv : v_map)
      fig.add_beacon(iv.mid(), 0.2);

    for(auto& y : v_obs)
    {
      fig.draw_pie(x_truth[0], x_truth[1], y[0] | 0., heading+y[1], "lightGray");
      fig.draw_pie(x_truth[0], x_truth[1], y[0], heading + y[1], "gray");
    }

    fig.draw_vehicle(x_truth, 0.5); // last param: vehicle size
    fig.draw_box(x); // estimated position
    fig.show();

    vibes::endDrawing();
    return x.contains(x_truth.subvector(0,1)) ? EXIT_SUCCESS : EXIT_FAILURE;
}