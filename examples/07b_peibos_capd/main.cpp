// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>
#include <codac-unsupported.h>

using namespace std;
using namespace codac2;

int main()
{  
  capd::IMap vectorField("par:sigma,rho,beta;var:x1,x2,x3;fun:sigma*(x2-x1),rho*x1-x2-x1*x3,-beta*x3+x1*x2;");
  vectorField.setParameter("sigma", 10.);
  vectorField.setParameter("rho", 28.);
  vectorField.setParameter("beta", 8/3);
  
  double tf=0.05;
  
  VectorVar X(2);
  AnalyticFunction psi0 ({X},{1/sqrt(1+sqr(X[0])+sqr(X[1])),X[0]/sqrt(1+sqr(X[0])+sqr(X[1])),X[1]/sqrt(1+sqr(X[0])+sqr(X[1]))});

  vector<vector<int>> generators ({{1,2,3},
                                            {-2,1,3},
                                            {3,2,-1}});

  double epsilon = 0.1;

  Figure3D figure3d_lorenz ("Lorenz");

  vector<Parallelepiped> v_par_lorenz;

  v_par_lorenz = PEIBOS3D(vectorField, tf, psi0, generators, epsilon);

  for (const auto& p : v_par_lorenz)
  {
    figure3d_lorenz.draw_parallelepiped(p.z, p.A, Color::green(0.5));
  }
}