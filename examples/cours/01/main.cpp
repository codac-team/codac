#include <tubex.h>

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  Interval domain(0.,10.);
  Tube x(domain);

  cout << "My first tube: " << x << endl;
}