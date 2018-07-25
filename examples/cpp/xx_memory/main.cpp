#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  float delta = 0.001;
  Interval domain(1.,4.5);
  Tube y(domain, delta, tubex::Function("sin(t)+sin(2*t)+sin(3*t)+sin(4*t)+(3-t)*[0.01,0.3]+[0,0.2]"));
  //Tube y(domain, delta, tubex::Function("cos(t)+2.*[-0.1,0.1]"));
  //Tube w(domain, delta, tubex::Function("-sin(t)+t*[-0.01,0.02]"));
  //Trajectory ytruth(domain, tubex::Function("cos(t)+0.08"));


  vibes::beginDrawing();
  VibesFigure_Tube fig("tubeval");
  fig.add_tube(&y, "y");
  //fig.add_trajectory(&ytruth, "ytruth", "black");
  fig.set_properties(200, 50, 800, 400);
  fig.show();

  //vibes::newGroup("eval_red", "red", vibesParams("figure", "tubeval"));
  //vibes::newGroup("eval_blue", "blue", vibesParams("figure", "tubeval"));

  //IntervalVector box(2);
  //box[0] = Interval(4.6,5.);
  //box[1] = Interval(0.3,0.8);
  //fig.draw_box(box, vibesParams("group", "eval_red", "figure", "tubeval"));
  //CtcEval ctc_eval;
  //ctc_eval.contract(box[0], box[1], y, w);
  //fig.draw_box(box, vibesParams("group", "eval_blue", "figure", "tubeval"));
  //fig.show();

  vibes::endDrawing();




  /*IntervalVector *test = new IntervalVector(2);
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
  IntervalVector test3 = test1 | test2;*/

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