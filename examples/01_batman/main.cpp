#include <codac-core.h>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(2);

  AnalyticFunction f_half_wing {
    {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
  AnalyticFunction f_half_head {
    {x}, vec(0.75+3*x[0]-x[1]) };
  AnalyticFunction f_half_neck {
    {x}, vec(9-8*x[0]-x[1], (6.*sqrt(10.)/7.)+(1.5-0.5*x[0])-(6.*sqrt(10.)/14.)*sqrt(4-sqr(x[0]-1))-x[1]) };
  AnalyticFunction f_half_legs {
    {x}, vec(((x[0]/2)-((3*sqrt(33)-7)/112)*sqr(x[0]))-3+sqrt(1-sqr(abs(x[0]-2)-1))-x[1]) };

  SetFunction half_bat {
    { /* no arguments */ }, 
    inverse(f_half_wing, {{0,oo},{-oo,1}})
    & (IntervalVector({{-oo,oo},{-oo,2.25}}) | inverse(f_half_head, {{0,oo}}))
    & not(IntervalVector({{0.75,3},{-oo,oo}}) & inverse(f_half_neck, {{-oo,0},{-oo,0}}))
    & ((not(IntervalVector({{0,4},{-oo,oo}}))) | inverse(f_half_legs, {{-oo,0}}))
  };

  SetFunction bat {
    { /* no arguments */ },
    half_bat() | OctaSym({-1,2})(half_bat())
  };

  DefaultView::set_window_properties({50,50},{1200,1200});
  pave(IntervalVector({{-8,8},{-4,4}}), *bat.create_sep(), 0.02);
}