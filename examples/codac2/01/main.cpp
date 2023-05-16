#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  codac2::TDomain tdomain(Interval(0,10), 0.01, true); // last argument creates "gates" (degenerated slices at scalar timesteps)
  codac2::Tube<IntervalVector> x(2, tdomain,
    TFunction("(sin(sqrt(t)+((t-5)^2)*[-0.01,0.01]) ; cos(t)+sin(t/0.2)*[-0.1,0.1])"));
  codac2::Tube<IntervalVector> u(2, tdomain);

  TFunction tf("x[2]", "u[2]", "(sin(x[1]) ; -sin(x[0]))");
  codac2::CtcDiffInclusion ctc_diffincl(tf);
  ctc_diffincl.contract(x,u);


  vibes::beginDrawing();

  codac::TubeVector x_codac1 = x.to_codac1(); // may take time
  codac::Tube xi_codac1 = x[1].to_codac1(); // may take time

  VIBesFigTube fig("Tube");
  fig.set_properties(100, 100, 600, 300);
  fig.add_tube(&xi_codac1, "x");
  fig.show(true);

  vibes::endDrawing();
  
  return EXIT_SUCCESS;
}