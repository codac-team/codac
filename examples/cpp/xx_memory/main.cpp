#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char *argv[])
{
  IntervalVector *test = new IntervalVector(2);
  delete test;

  Interval domain(0.,1.);

  Tube x(domain, 0.5);
  cout << "nbslices: " << x.nb_slices() << endl;




  /*vibes::beginDrawing();

  Tube y(domain, 0.001, tubex::Function("cos(t)+[-0.01,0.02]*t"));
  VibesFigure_Tube figy("fig y");
  figy.add_tube(&y, "y");
  figy.set_properties(0, 0, 400, 200);
  figy.show();

  Tube z(domain, 0.001, tubex::Function("sin(t)+[-0.01,0.02]*t"));
  VibesFigure_Tube figz("fig z");
  figz.add_tube(&z, "z");
  figz.set_properties(0, 0, 400, 200);
  figz.show();

  figy.draw_box(IntervalVector(2, Interval(0.,0.8)));
  figz.draw_box(IntervalVector(2, Interval(0.6,0.8)));
  figz.draw_box(IntervalVector(2, Interval(0.5,0.8)));
  figy.draw_box(IntervalVector(2, Interval(0.,0.3)));*/

}