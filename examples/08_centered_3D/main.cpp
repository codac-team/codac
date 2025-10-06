// The generated .obj files can be visualized on https://3dviewer.net

#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(2);

  Figure3D fig_zon("zonotopes");
  fig_zon.draw_axes(1.0);

  AnalyticFunction f { {x},
    {
          (2.0+cos(x[0])/(1+(sin(x[0])^2.0)))*sin(x[1]),
          (2.0+cos(x[0])/(1+(sin(x[0])^2.0)))*cos(x[1]),
          2.0*sin(x[0])*cos(x[0])/(1+(sin(x[0])^2.0))
          
    }
  };
  
  double phi=0.0;
  double psi=0.0;
  double dphi=0.3;
  double dpsi=0.3;

  while (phi<2*PI) {
      psi=0.0;
      while (psi<2*PI) {
        IntervalVector T { {phi,phi+dphi}, {psi,psi+dpsi} };
        IntervalMatrix df = f.diff(T);
        if (df.is_empty() || df.is_unbounded()) {
           IntervalVector bx = f.eval(T);
           fig_zon.draw_box(bx, Color::blue(0.3)); 
        } else {
           IntervalVector cent = f.eval(T.mid());
	   Vector inflationbox = cent.rad() + df.rad()*T.rad();
           Matrix v (3,5);
            v << (T[0].rad()*df.col(0).mid()), (T[1].rad()*df.col(1).mid()),
                  Vector({ inflationbox[0], 0.0, 0.0 }),Vector({ 0.0, inflationbox[1], 0.0 }),
                  Vector({ 0.0, 0.0, inflationbox[2] });
           fig_zon.draw_zonotope({cent.mid(),v},Color::red(0.3));
        }
        psi=psi+dpsi;
      }
      phi=phi+dphi;
  }

  fig_zon.draw_surface({0,0,0}, Matrix::Identity(3,3),
		 Interval(0,2*PI),
		0.05*PI, Interval(0,2*PI), 0.05*PI,
		[](double phi,double psi) -> Vector
    { return {
          (2.0+cos(phi)/(1+(sin(phi)*sin(phi))))*sin(psi),
          (2.0+cos(phi)/(1+(sin(phi)*sin(phi))))*cos(psi),
          2.0*sin(phi)*cos(phi)/(1+(sin(phi)*sin(phi)))
      }; 
    } ,
		{ Color::green(0.6), "surface" });
  
}
