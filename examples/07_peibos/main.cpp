// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>
#include <codac-unsupported.h>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar y(3);
  AnalyticFunction f({y},{sqr(y[0])-sqr(y[1])+y[0],2*y[0]*y[1]+y[1],y[2]});
  
  VectorVar X(2);
  AnalyticFunction psi0 ({X},{1/sqrt(1+sqr(X[0])+sqr(X[1])),X[0]/sqrt(1+sqr(X[0])+sqr(X[1])),X[1]/sqrt(1+sqr(X[0])+sqr(X[1]))});

  vector<vector<int>> generators ({{1,2,3},
                                            {-2,1,3},
                                            {3,2,-1}});

  double epsilon = 0.1;
  
  Figure3D figure3d ("Conform");

  auto v_par = PEIBOS3D(f, psi0, generators, epsilon);

  for (const auto& p : v_par)
  {
    figure3d.draw_parallelepiped(p.z, p.A, Color::green(0.5));
  }
}