/** 
 *  Codac - Examples
 *  Reliable loop detection of a mobile robot
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the papers:
 *              "Proving the existence of loops in robot trajectories"
 *              Simon Rohou, Peter Franek, Clément Aubry, Luc Jaulin
 *              and
 *              "Loop detection of mobile robots using interval analysis"
 *              Clément Aubry, Rozenn Desmare, Luc Jaulin
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;

int main(int argc, char** argv)
{
  Tube::enable_syntheses(); // faster integral computations

  /* =========== LOADING DATA =========== */

    TubeVector *x;
    TrajectoryVector *x_truth;
    DataLoaderRedermor data_loader(argv[1]);
    data_loader.load_data(x, x_truth, 0.2);

  /* =========== LOOPS DETECTION =========== */

    clock_t t_start = clock();

    TPlane tplane(x->tdomain());
    tplane.compute_detections(10., x->subvector(0,1), x->subvector(3,4));

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 500, 500);
    fig_map.add_tube(x, "x", 0, 1);
    fig_map.add_trajectory(x_truth, "x*", 0, 1, "white");
    fig_map.show();

    VIBesFigPaving fig_tplane("t-plane", &tplane);
    fig_tplane.set_properties(1550, 150, 500, 500);
    fig_tplane.show();

    vibes::endDrawing();

  /* =========== ENDING =========== */

    printf("t-plane computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
    delete x;
    delete x_truth;
    return EXIT_SUCCESS;
}
