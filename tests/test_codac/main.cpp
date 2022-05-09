#include <codac.h>
#include <iostream>

using namespace std;
using namespace ibex;
using namespace codac;

// Download and run https://github.com/ENSTABretagneRobotics/VIBES/releases 
// before to be able to see the tube in VIBes window...

int main()
{
  Tube x(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"));

  cout << "My first tube:" << x << endl;

  vibes::beginDrawing();
  VIBesFigTube fig("My first tube");
  fig.add_tube(&x, "x");
  fig.show();
  vibes::endDrawing();

  return 0;
}
