#include <codac>
#include <codac-sympy.h>

using namespace std;
using namespace codac2;

int main()
{
  ScalarVar x,y;
  AnalyticFunction f({x,y}, x*y + sin(x));

  auto dfdx = sympy_partial_diff(f,x);
  auto dfdy = sympy_partial_diff(f,y);

  cout << dfdx << endl;
  cout << dfdy << endl;
}