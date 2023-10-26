#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  auto tdomain = codac2::create_tdomain(Interval(0,10), 0.01, true); // last argument creates "gates" (degenerated slices at scalar timesteps)
  codac2::Tube<IntervalVector> x(tdomain,
    TFunction("(sin(sqrt(t)+((t-5)^2)*[-0.01,0.01]) ; cos(t)+sin(t/0.2)*[-0.1,0.1])"));
  codac2::Tube<IntervalVector> u(tdomain);

    // If you want to iterate each slice (including gates = degenerate slices)
    for(auto& sx : x)
    {
      //cout << sx << endl;
    }

  vibes::beginDrawing();

  codac::TubeVector x_codac1 = codac2::to_codac1(x); // may take time

  VIBesFigTube fig("Tube");
  fig.set_properties(100, 100, 600, 300);
  fig.add_tube(&x_codac1[1], "x");
  fig.show(true);

  vibes::endDrawing();
  
  return EXIT_SUCCESS;
}