/** 
 *  Codac - Examples
 *  Static range-only localization
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

using namespace std;
using namespace codac;

int main()
{
  /* =================== 0. Parameters, truth and data ==================== */

    // Truth (unknown pose)
    Vector x_truth({0.,0.,M_PI/6.}); // (x,y,heading)

    // Creating random map of landmarks
    int nb_landmarks = 3;
    IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);
    
    // The following function generates a set of [range]x[bearing] values
    vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, v_map, false);

    // We keep range-only observations from v_obs, and add uncertainties
    vector<Interval> v_range;
    for(auto& obs : v_obs)
      v_range.push_back(obs[0].inflate(0.1)); // adding uncertainties: [-0.1,0.1]


  /* =============== 1. Defining domains for our variables ================ */

    IntervalVector x(2);


  /* =========== 2. Defining contractors to deal with equations =========== */

    // We use the predefined contractor ctc::dist, no need to build it


  /* =============== 3. Adding the contractors to a network =============== */

    ContractorNetwork cn;

    for(int i = 0 ; i < nb_landmarks ; i++)
      cn.add(ctc::dist, {x, v_map[i], v_range[i]});


  /* ======================= 4. Solving the problem ======================= */

    cn.contract();


  /* ============================ 5. Graphics ============================= */

    vibes::beginDrawing();

    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 600, 600);

    for(const auto& iv : v_map)
      fig.add_beacon(iv.mid(), 0.2);

    for(int i = 0 ; i < nb_landmarks ; i++)
      fig.draw_ring(v_map[i][0].mid(), v_map[i][1].mid(), v_range[i], "gray");

    fig.draw_vehicle(x_truth, 0.5); // last param: vehicle size
    fig.draw_box(x); // estimated position
    fig.show();

    vibes::endDrawing();
    return x.contains(x_truth.subvector(0,1)) ? EXIT_SUCCESS : EXIT_FAILURE;
}