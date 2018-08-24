#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  /* =========== LOADING DATA =========== */

    TubeVector *x;
    TrajectoryVector *x_truth;
    DataLoader_Lissajous data_loader;
    data_loader.load_data(x, x_truth);
    vector<Beacon> v_seamarks = data_loader.get_beacons();
    vector<IntervalVector> v_obs = data_loader.get_observations();

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_TubeVector fig_x("Robot's state", 4);
    fig_x.set_properties(2000, 50, 500, 250);
    fig_x.add_tubevector(x, "x");
    fig_x.add_trajectoryvector(x_truth, "x*");
    fig_x.show();

    VibesFigure_Map fig_map("Map");
    fig_map.set_properties(1450, 50, 600, 400);
    fig_map.add_tube(x, "x", 0, 1);
    fig_map.add_trajectory(x_truth, "x*", 0, 1);
    fig_map.add_beacons(v_seamarks);
    fig_map.show();

  /* =========== STATE ESTIMATION =========== */

    fig_x.show();
    fig_map.show();

  /* =========== ENDING =========== */

    vibes::endDrawing();

    delete x;
    delete x_truth;
    return EXIT_SUCCESS;
}