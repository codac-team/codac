#include <codac>
#include <codac-unsupported.h>

using namespace std;
using namespace codac2;

int main()
{ 
  // 2D example of the PEIBOS algorithm

  IMapWrapper vectorField_2d_wrap("var:x1,x2;fun:x2,(1-sqr(x1))*x2-x1;");

  double tf_2d = 2.0;
  
  VectorVar X_2d(1);
  AnalyticFunction psi0_2d ({X_2d},{cos(X_2d[0]*PI/4.),sin(X_2d[0]*PI/4.)});

  vector<vector<int>> generators_2d ({{1,2},
                                      {-2,1}});
  
  auto v_par_2d = PEIBOS(vectorField_2d_wrap, tf_2d, psi0_2d, generate_symmetries(generators_2d, psi0_2d), 0.05, true);

  Figure2D output ("Van der Pol",GraphicOutput::VIBES|GraphicOutput::IPE);
  output.set_axes(axis(0,{-3,3}),axis(1,{-3,3}));
  output.set_window_properties({50,100},{800,800});

  for (const auto& p : v_par_2d)
  {
    output.draw_parallelepiped(p.z, p.A, {Color::green(),Color::green(0.5)});
  }

  // 3D example of the PEIBOS algorithm

  IMapWrapper vectorField_3d_wrap("par:sigma,rho,beta;var:x1,x2,x3;fun:sigma*(x2-x1),rho*x1-x2-x1*x3,-beta*x3+x1*x2;");
  vectorField_3d_wrap.setParameter("sigma",10.);
  vectorField_3d_wrap.setParameter("rho",28.);
  vectorField_3d_wrap.setParameter("beta",8./3.);
  
  double tf_3d=0.05;
  
  VectorVar X_3d(2);
  AnalyticFunction psi0_3d ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});

  vector<vector<int>> generators_3d ({{1,2,3},
                                  {-2,1,3},
                                  {3,2,-1}});

  auto v_par_3d = PEIBOS(vectorField_3d_wrap, tf_3d, psi0_3d, generate_symmetries(generators_3d, psi0_3d), 0.2, true);

  Figure3D figure3d_lorenz ("Lorenz");

  for (const auto& p : v_par_3d)
  {
    figure3d_lorenz.draw_parallelepiped(p.z, p.A, Color::green(0.5));
  }

  // Multiple discrete times

  IMapWrapper vectorField_discrete_wrap("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
  vectorField_discrete_wrap.setParameter("l",Interval(2.));
  vectorField_discrete_wrap.setParameter("g",Interval(10.));

  double tf_discrete = 10.0;
  double dt_discrete = tf_discrete/200.;
  
  VectorVar X_discrete(1);
  AnalyticFunction psi0_discrete ({X_discrete},{0.01*cos(X_discrete[0]*PI/4.0),0.01*sin(X_discrete[0]*PI/4.0)});

  vector<vector<int>> generators_discrete ({{1,2},
                                            {-2,1}});
  
  auto v_par_discrete = PEIBOS(vectorField_discrete_wrap, tf_discrete, dt_discrete, psi0_discrete, generate_symmetries(generators_discrete, psi0_discrete), 0.2, {-PI/2.,0.}, true);

  Figure2D output_discrete ("Pendulum",GraphicOutput::VIBES | GraphicOutput::IPE);
  output_discrete.set_axes(axis(0,{-2,1.5}),axis(1,{-2,3}));
  output_discrete.set_window_properties({1000,100},{800,800});

  for (const auto& pair : v_par_discrete) 
  {
    double t = pair.first;
    const auto& v_par = pair.second;

    for (const auto& p : v_par) {
        output_discrete.draw_parallelepiped(p.z, p.A, {ColorMap::rainbow().color(t/tf_discrete), ColorMap::rainbow_05().color(t/tf_discrete)});
    }

  }

}