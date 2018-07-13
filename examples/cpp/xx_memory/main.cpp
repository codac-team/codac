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
}