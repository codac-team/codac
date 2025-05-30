#include <codac>

using namespace std;
using namespace codac2;

int main(){

  // Representation of centered form using zonotopes
  Figure2D fig4 ("SpiralCentered",GraphicOutput::VIBES|GraphicOutput::IPE);

  fig4.set_window_properties({500,50},{500,500});
  fig4.set_axes(axis(0,{-10,10}), axis(1,{-10,10}));

  double a=0.5;

  ScalarVar t;
  // we use Fermat's spiral
  AnalyticFunction f1 ({t},{a*sqrt(t)*cos(t),a*sqrt(t)*sin(t)});

  double dt=0.2;
  double time=0.0;
  while (time<100.0) {
     Interval T(time,time+dt);
     // using eval_<true> would be easier, but it's not allowed :(
     IntervalVector df = f1.diff(T);
     if (df.is_empty() || df.is_unbounded()) {
		 /* not differentiable, we use a box */
       IntervalVector bx = f1.eval(T);
       fig4.draw_box(bx,{Color::blue(),Color::yellow(0.1)});
     } else {
       IntervalVector cent = f1.eval(T.mid());
     // f(t) \in cent + df (t-T.mid())
     //      \in cent.mid + [-1,1] * T.rad*df.mid + [-1,1]*cent.rad + 
     //        [-1,1] * T.rad*df.rad
       Vector inflationbox = cent.rad() + T.rad()*df.rad();
       std::vector<Vector> v
  	{ (T.rad()*df.mid()),
	  { inflationbox[0], 0.0 }, { 0.0, inflationbox[1] } };
       fig4.draw_zonotope(cent.mid(),v,{Color::red(),Color::yellow(0.1)});
     }
     time = time+dt;
  }
  
  AnalyticTraj traj4 (f1,{0,100});
  fig4.draw_trajectory(traj4,Color::black());
}
