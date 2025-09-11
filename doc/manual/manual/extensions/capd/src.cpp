// Author : Maël GODARD
// Adapted from CAPD examples

// [codac-capd-1-beg]
#include <codac-graphics.h> //can be replace by <codac>
#include <codac-capd.h>
#include <capd/capdlib.h>
// [codac-capd-1-end]


int main()
{
  // Equation of the pendulum with friction
  // [codac-capd-2-beg]
  capd::IMap vectorField("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
 
  vectorField.setParameter("l",capd::Interval(2.)); // length of the pendulum equal to 2
  vectorField.setParameter("g",capd::Interval(10.));  // gravity acceleration equal to 10
  // [codac-capd-2-end]

  // the solver uses high order enclosure method to verify the existence 
  // of the solution. The order is set to 20.
  // [codac-capd-3-beg]
  capd::IOdeSolver solver(vectorField,20);
  solver.setAbsoluteTolerance(1e-10);
  solver.setRelativeTolerance(1e-10);
  // [codac-capd-3-end]

  // [codac-capd-4-beg]
  capd::ITimeMap timeMap(solver);
  capd::Interval initialTime(0.);  // initial time (t0)
  capd::Interval finalTime(20.); // final time (tf)
  // [codac-capd-4-end]

  // initial set
  // [codac-capd-5-beg]
  capd::IVector c(2);
  c[0] = -M_PI/2.;
  c[1] = 0.;
  // take some box around c
  c[0] += capd::Interval(-1,1)*1e-2;
  c[1] += capd::Interval(-1,1)*1e-2;

  // define a doubleton representation of the interval vector c
  capd::C0HORect2Set s(c);
  // [codac-capd-5-end]

  // [codac-capd-7-beg]
  capd::ITimeMap::SolutionCurve solution(initialTime);
  timeMap(finalTime,s,solution);
  // [codac-capd-7-end]

  // we integrate the set s over the time T
  // [codac-capd-6-beg]
  capd::Interval T(1);
  capd::IVector result = timeMap(T,s);
  // [codac-capd-6-end]

  // [codac-capd-8-beg]

  auto tdomain = create_tdomain(codac2::Interval(0,20),0.05, true); // true to have gates
  auto codac_tube = to_codac(solution, tdomain);

  // [codac-capd-8-end]

  if (false)
  {
    // [codac-capd-9-beg]
    std::cout << "\ninitial set: " << c;
    std::cout << "\ndiam(initial set): " << diam(c) << std::endl;

    std::cout << "\n\nafter time=" << T << " the image is: " << result;
    std::cout << "\ndiam(image): " << diam(result) << std::endl << std::endl;

    codac2::DefaultFigure::set_axes(codac2::axis(0,{-2,1.5}),codac2::axis(1,{-2,3}));

    codac2::DefaultFigure::draw_tube(codac_tube, codac2::ColorMap::blue_tube());
    codac2::DefaultFigure::draw_tube(codac_tube, codac2::Color::black());

    for (float t=0.;t<20.;t+=0.05)
      codac2::DefaultFigure::draw_box(codac2::to_codac(solution(t)), {codac2::Color::none(), codac2::Color::orange(0.5)});

    codac2::DefaultFigure::draw_box(codac2::to_codac(c),codac2::Color::green());
    codac2::DefaultFigure::draw_box(codac2::to_codac(result),codac2::Color::red());
    // [codac-capd-9-end]
  }
 
}