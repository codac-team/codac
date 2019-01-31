#include "tubex.h"
#include "tubex-robotics.h"
#include "ibex_CtcPolar.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  clock_t t_start = clock();
  
  /* =========== LOADING DATA =========== */

    TubeVector *x;
    TrajectoryVector *x_truth;
    DataLoader_Lissajous data_loader;
    data_loader.load_data(x, x_truth);

    vector<Beacon> v_seamarks = data_loader.get_beacons(x->codomain().subvector(0,1));
    vector<IntervalVector> v_obs = data_loader.get_observations(*x_truth, v_seamarks);

    vector<IntervalVector> v_obs_mapped;
    for(int i = 0 ; i < v_obs.size() ; i++)
      v_obs_mapped.push_back(IntervalVector(2)); // unknown position

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_TubeVector fig_x("Robot's state", 0, 3);
    fig_x.set_properties(2000, 50, 500, 250);
    fig_x.add_tubevector(x, "x");
    fig_x.add_trajectoryvector(x_truth, "x*");
    fig_x.show();

    VibesFigure_Map fig_map("Map");
    fig_map.set_properties(1450, 50, 600, 400);
    fig_map.add_tubevector(x, "x", 0, 1);
    fig_map.add_trajectoryvector(x_truth, "x*", 0, 1, "white");
    fig_map.add_beacons(v_seamarks);
    fig_map.add_observations(v_obs, x_truth);
    fig_map.show();

  /* =========== CREATING CONTRACTORS =========== */

    CtcPolar ctc_polar;
    CtcConstellation ctc_constellation(v_seamarks);
    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;
    ctc_eval.enable_temporal_propagation(false); // faster use

  /* =========== DATA ASSOCIATION =========== */

    int k = 0;
    double volume;

    do
    {
      k++;
      cout << "Iteration " << k << "..." << endl;
      volume = x->volume();

      for(int i = 0 ; i < v_obs.size() ; i++)
      {
        double t = v_obs[i][0].mid();

        // Polar constraint

          v_obs_mapped[i][0] -= (*x)[0](t);
          v_obs_mapped[i][1] -= (*x)[1](t);
          ctc_polar.contract(v_obs_mapped[i][0], v_obs_mapped[i][1], v_obs[i][1], v_obs[i][2]);
          v_obs_mapped[i][0] += (*x)[0](t);
          v_obs_mapped[i][1] += (*x)[1](t);

        // Map / observation constraint (constellation contractor)

          ctc_constellation.contract(v_obs_mapped[i]);

        // Polar constraint

          IntervalVector rob_pos(2);
          rob_pos[0] = (*x)[0](t); rob_pos[1] = (*x)[1](t);
          rob_pos -= v_obs_mapped[i]; // relative to the beacon
          v_obs[i][2] += M_PI; // relative to the beacon
          ctc_polar.contract(rob_pos[0], rob_pos[1], v_obs[i][1], v_obs[i][2]);
          v_obs[i][2] -= M_PI; // relative to the map
          rob_pos += v_obs_mapped[i]; // relative to the map

        // Trajectory evaluation constraint

          ctc_eval.contract(t, rob_pos[0], (*x)[0], (*x)[2]);
          ctc_eval.contract(t, rob_pos[1], (*x)[1], (*x)[3]);
      }

      // Evolution constraint (differential equation)

        ctc_deriv.contract((*x)[0], (*x)[2]);
        ctc_deriv.contract((*x)[1], (*x)[3]);

    } while(volume != x->volume());

  /* =========== ENDING =========== */

    for(int i = 0 ; i < v_obs_mapped.size() ; i++)
      if(v_obs_mapped[i].volume() != 0) // degenerate box
        cout << v_obs_mapped[i] << " NOT IDENTIFIED" << endl;

    fig_x.show();
    fig_map.show();
    vibes::endDrawing();
    bool success = x->contains(*x_truth) == YES;
    delete x; delete x_truth;


  printf("Time taken: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
  // Checking if this example still works:
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}