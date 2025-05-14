// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>
#include <codac-unsupported.h>

using namespace std;
using namespace codac2;

int main()
{ 
  // 2D example of the PEIBOS algorithm

  capd::IMap vectorField_2d("var:x1,x2;fun:x2,(1-sqr(x1))*x2-x1;");

  double tf_2d = 2.0;
  
  VectorVar X_2d(1);
  AnalyticFunction psi0_2d ({X_2d},{cos(X_2d[0]*PI/4.),sin(X_2d[0]*PI/4.)});

  vector<vector<int>> generators_2d ({{1,2},
                                      {-2,1}});
  
  auto v_par_2d = PEIBOS(vectorField_2d, tf_2d, psi0_2d, generators_2d, 0.05);

  Figure2D output ("Van der Pol",GraphicOutput::VIBES|GraphicOutput::IPE);
  output.set_axes(axis(0,{-3,3}),axis(1,{-3,3}));
  output.set_window_properties({100,100},{800,800});

  for (const auto& p : v_par_2d)
  {
    output.draw_parallelepiped(p.z, p.A, {Color::green(),Color::green(0.5)});
  }

  // 3D example of the PEIBOS algorithm

  capd::IMap vectorField_3d("par:sigma,rho,beta;var:x1,x2,x3;fun:sigma*(x2-x1),rho*x1-x2-x1*x3,-beta*x3+x1*x2;");
  vectorField_3d.setParameter("sigma", 10.);
  vectorField_3d.setParameter("rho", 28.);
  vectorField_3d.setParameter("beta", 8/3);
  
  double tf_3d=0.05;
  
  VectorVar X_3d(2);
  AnalyticFunction psi0_3d ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});

  vector<vector<int>> generators_3d ({{1,2,3},
                                  {-2,1,3},
                                  {3,2,-1}});

  double epsilon = 0.1;

  auto v_par_3d = PEIBOS(vectorField_3d, tf_3d, psi0_3d, generators_3d, 0.1);

  Figure3D figure3d_lorenz ("Lorenz");

  for (const auto& p : v_par_3d)
  {
    figure3d_lorenz.draw_parallelepiped(p.z, p.A, Color::green(0.5));
  }
}