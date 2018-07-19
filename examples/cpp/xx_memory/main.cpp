#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
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

  IntervalVector box1(2, Interval(0.,9.));
  Interval intv1(0.,9.);

  //box1 += intv1;

  Tube s1(domain), s2(s1);
  TubeVector t1(domain, 2), t2(t1);
  t1 += t2;
  t1 -= t2;
  t1 &= t2;
  t1 |= t2;
  t1 += box1;
  t1 += intv1;
  t1 += s1;
  t1 -= s1;
  t1 &= s1;
  t1 |= s1;
  t1 *= intv1;
  t1 *= s1;

  t1 = t1 + t2;
  t1 = t1 - t2;
  s1 = s1 + s2;
  s1 = s1 - s2;
  s1 = s1 / s2;
  s1 = s1 * s2;

  s1.set(Interval(0., 1.));
  //cout << s1 << endl;
  pair<Tube,Tube> p_s = s1.bisect(0.);
  s1.inflate(0.5);
  //cout << s1 << endl;

  IntervalVector test1(2), test2(2);
  IntervalVector test3 = test1 | test2;

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
  
  return EXIT_SUCCESS;
}