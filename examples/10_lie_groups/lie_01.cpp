// Codac example - Integration using Lie symmetries
//
// Author: Simon Rohou (2025), from the thesis of Julien Damers
//
// Reference:
// Lie symmetries applied to interval integration
//   Julien Damers, Luc Jaulin, Simon Rohou
//   Automatica, Volume 144, October 2022

#include <codac>
#include <codac-capd.h>
#include <capd/capdlib.h>

using namespace codac2;

SlicedTube<IntervalVector> capd_reference()
{
  capd::IMap f("var:x1,x2; fun: 1,-x2;");
  capd::IOdeSolver solver(f, 10); // order 10
  capd::ITimeMap time_map(solver);
  
  capd::IVector x0({0,1});
  capd::C0HORect2Set s(x0);

  capd::ITimeMap::SolutionCurve solution(capd::Interval(0));
  time_map({5}, s, solution);

  auto tdomain = create_tdomain({0,5}, 1e-2);
  return to_codac(solution, tdomain);
}

int main()
{
  auto a = capd_reference();

  IntervalVector x0({{0,1},{2,3}});
  VectorVar v(3);
  AnalyticFunction phi({v}, {v[0]+v[2], v[1]*exp(-v[2])});
  SepInverse S1(phi, x0);
  SepProj S2(S1, {0,1}, {-a.tdomain()->t0_tf()}, 1e-1);

  DefaultFigure::pave(
    {{-0.5,6.5},{-0.5,3.5}},
    S2, 1e-2,PavingStyle::blue_white());
  DefaultFigure::draw_tube(a);
  DefaultFigure::draw_box(x0, {Color::black(),Color::green(0.5)});
}