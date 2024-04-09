#include <codac-core.h>
#include <codac-graphics.h>

using namespace std;
using namespace codac2;

#include "sivia.cpp"

int main()
{
  ArgVector x(2);

  VectorFunction f_half_wing {
    {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
  ScalarFunction f_half_head {
    {x}, 0.75+3*x[0]-x[1] };
  VectorFunction f_half_neck {
    {x}, vec(9-8*x[0]-x[1], (6.*sqrt(10.)/7.)+(1.5-0.5*x[0])-(6.*sqrt(10.)/14.)*sqrt(4-sqr(x[0]-1))-x[1]) };
  ScalarFunction f_half_legs {
    {x}, ((x[0]/2)-((3*sqrt(33)-7)/112)*sqr(x[0]))-3+sqrt(1-sqr(abs(x[0]-2)-1))-x[1] };

  auto sep_half_bat =
    inverse(f_half_wing, {{0,oo},{-oo,1}})
    & (IntervalVector({{-oo,oo},{-oo,2.25}}) | inverse(f_half_head, {0,oo}))
    & not(IntervalVector({{0.75,3},{-oo,oo}}) & inverse(f_half_neck, {{-oo,0},{-oo,0}}))
    & ((not(IntervalVector({{0,4},{-oo,oo}}))) | inverse(f_half_legs, {-oo,0}));

  auto sep_bat = sep_half_bat | OctaSym({-1,2})(sep_half_bat);

  vibes::beginDrawing();
  SIVIA_sep(sep_bat, {{-8,8},{-4,4}}, 0.1, "The batman");
  vibes::endDrawing();

  return EXIT_SUCCESS;
}