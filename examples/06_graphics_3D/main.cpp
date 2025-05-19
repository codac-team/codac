// The generated .obj files can be visualized on https://3dviewer.net

#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(3);

  AnalyticFunction f { {x},
    {
      -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
      2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
    }
  };
  

  CtcInverse ctc(f, IntervalVector::zero(2));
  auto p_ctc = pave({{0,2},{2,4},{0,10}}, ctc, 0.02);
  Figure3D fig_ctc("Paving contractor");
  fig_ctc.draw_paving(p_ctc);


  SepInverse sep_ellipsoid1(
      AnalyticFunction({x},  0.5*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+2*sqr(x[1])+2*sqr(x[2])),
      Interval(0.4,1));
  SepInverse sep_ellipsoid2(
      AnalyticFunction({x}, 3*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+sqr(x[1])+sqr(x[2])),
      Interval(0,1));

  auto p_sep = pave({{-1.1,1.1},{-1.1,1.1},{-1.1,1.1}}, sep_ellipsoid1&sep_ellipsoid2, 0.1);

  Figure3D fig_sep("Paving separator");
  fig_sep.draw_axes(0.4);
  fig_sep.draw_paving(p_sep);


  Figure3D fig_examples("3D examples");
  fig_examples.draw_axes(1.0);
  fig_examples.draw_triangle({1,0,0},{0,1,0},{0,0,1},{ Color::dark_green(0.5), "triangle1" });
  fig_examples.draw_triangle({2,0,0},{{-1,0,0},{0,1,1},{0,0,-1}},
		{1,0,0},{0,1,0},{0,0,1},Color::purple(0.5));
  fig_examples.draw_sphere({0,0,2},{{-1,0,0},{0,1,1},{0,0,-1}},
		{ Color::yellow(0.6), "sphere" });
  fig_examples.draw_arrow({0,2,0},{{-1,0,0},{0,1,1},{0,0,-1}},
		Color::red(1.0));
  fig_examples.draw_car({-1,0,0},0.3*Matrix::Identity(3,3),
		{ Color::green(0.8), "car" });
  fig_examples.draw_plane({3,0,0},0.5*Matrix::Identity(3,3),true,
		{ Color::dark_gray(0.8), "plane" });

  fig_examples.draw_surface({0,-2,0}, 0.5*Matrix::Identity(3,3), Interval(0,2*PI),
		0.05*PI, Interval(0,2*PI), 0.05*PI,
		[](double phi,double psi) -> Vector
		{ return {(1-cos(phi))*sin(phi),
			(1-cos(2*phi))*cos(phi)*cos(psi),
			 (1-cos(phi))*cos(phi)*sin(psi) }; },
		{ Color::red(0.6), "example_surface" });
  
}
