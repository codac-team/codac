#include "ibex.h"
#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  TubeVector *x;
  TrajectoryVector *x_truth;
  DataLoader_Redermor data_loader("./data/redermor/gesmi.txt");
  data_loader.load_data(x, x_truth);

  vibes::beginDrawing();
  VibesFigure_Tube fig("Redermor", x, x_truth);
  fig.set_properties(1550, 50, 600, 300);
  fig.show();
  vibes::endDrawing();

  delete x;
  delete x_truth;
  return EXIT_SUCCESS;
}