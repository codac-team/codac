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
    DataLoader_Redermor data_loader("./data/redermor/gesmi.txt");
    data_loader.load_data(x, x_truth);

  /* =========== LOOPS DETECTION =========== */

    clock_t t_start = clock();
    TPlane tplane(IntervalVector(2, x->domain()));
    tplane.compute(1000., x->subvector(0,1), x->subvector(3,4));
    printf("Computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VibesFigure_Map fig_map("Map");
    fig_map.set_properties(1450, 50, 500, 500);
    fig_map.add_tubevector(x, "x", 0, 1);
    fig_map.add_trajectoryvector(x_truth, "x*", 0, 1, "white");
    fig_map.show();

    VibesFigure_Paving fig_tplane("t-plane", &tplane);
    fig_tplane.set_properties(1550, 150, 500, 500);
    fig_tplane.show();

    vibes::endDrawing();

  /* =========== ENDING =========== */

    delete x;
    delete x_truth;
    return EXIT_SUCCESS;
}