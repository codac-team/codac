#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char *argv[])
{
  IntervalVector *test = new IntervalVector(2);
  delete test;

  Interval domain(0.,1.);

  Tube x(domain, 0.05);
  Tube y = x;
  x.sample(0.37638639863868);
  Tube z(x);
  z.set(Interval::EMPTY_SET);
  Tube a(domain, 0.05, tubex::Function("cos(t)+t*[-0.01,0.02]"));

  a.serialize("a.tube");

  Tube b("a.tube");

  cout << a << endl;
  cout << b << endl;
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