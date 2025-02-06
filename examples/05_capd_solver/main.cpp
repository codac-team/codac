// Author : Maël GODARD
// Adapted from CAPD examples

#include <codac>
#include <codac-capd.h>
#include <capd/capdlib.h>

using namespace std;
using namespace codac2;


int main()
{
  // Equation of the pendulum with friction
  capd::IMap vectorField("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
 
  vectorField.setParameter("l",capd::interval(2.));
  vectorField.setParameter("g",capd::interval(10.));

  // the solver, is uses high order enclosure method to verify the existence 
  // of the solution. The order is set to 20.
  capd::IOdeSolver solver(vectorField,20);
  solver.setAbsoluteTolerance(1e-10);
  solver.setRelativeTolerance(1e-10);

  capd::ITimeMap timeMap(solver);
  capd::interval initialTime (0.);
  capd::interval finalTime (20.);
  capd::ITimeMap::SolutionCurve solution(initialTime); 

  // initial set
  capd::IVector c(2);
  c[0] = -M_PI/2.;
  c[1] = 0.;
  // take some box around c
  c[0] += capd::interval(-1,1)*1e-2;
  c[1] += capd::interval(-1,1)*1e-2;


  // define a doubleton representation of the interval vector c
  capd::C0HORect2Set s(c);

  timeMap(finalTime,s,solution);

  // we integrate the set s over the time T
  capd::interval T(1);
  
  cout << "\ninitial set: " << c;
  cout << "\ndiam(initial set): " << diam(c) << endl;

  capd::IVector result = timeMap(T,s);

  cout << "\n\nafter time=" << T << " the image is: " << result;
  cout << "\ndiam(image): " << diam(result) << endl << endl;

  DefaultView::set_axes(axis(0,{-2,1.5}),axis(1,{-2,3}));

  for (float t=0.;t<20.;t+=0.05)
    DefaultView::draw_box(to_codac(solution(t)));

  DefaultView::draw_box(to_codac(c),Color::green());
  DefaultView::draw_box(to_codac(result),Color::red());
  
}