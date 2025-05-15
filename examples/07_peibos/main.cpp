// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>
#include <codac-unsupported.h>

using namespace std;
using namespace codac2;

int main()
{
  // 2D example of the PEIBOS algorithm
  VectorVar y_2d(2);
  double a = 1.4; double b = 0.3;
  AnalyticFunction f_2d({y_2d},{y_2d[1]+1-a*sqr(y_2d[0]),b*y_2d[0]});

  VectorVar X_2d(1);
  AnalyticFunction psi0_2d ({X_2d},{cos(X_2d[0]*PI/4.-PI/2),sin(X_2d[0]*PI/4.-PI/2)});

  vector<vector<int>> generators_2d ({{1,2},
                                      {-2,1}});

  auto v_par_2d = PEIBOS(f_2d, psi0_2d, generators_2d, 0.1, {-0.2,0.});

  Figure2D figure_2d ("Henon Map", GraphicOutput::VIBES);
  figure_2d.set_window_properties({25,50},{500,500});
  figure_2d.set_axes(axis(0,{-1.4,2.2}), axis(1,{-0.4,0.3}));

  for (const auto& p : v_par_2d)
  {
    figure_2d.draw_parallelepiped(p.z, p.A, {Color::green(),Color::green(0.5)});
    for (const auto& vertice : p.vertices())
    {
      figure_2d.draw_point(vertice, {Color::red(),Color::red(0.5)});
    }
  }

  // 3D example of the PEIBOS algorithm
  VectorVar y_3d(3);
  AnalyticFunction f_3d({y_3d},{sqr(y_3d[0])-sqr(y_3d[1])+y_3d[0],2*y_3d[0]*y_3d[1]+y_3d[1],y_3d[2]});
  
  VectorVar X_3d(2);
  AnalyticFunction psi0_3d ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});

  vector<vector<int>> generators_3d ({{1,2,3},
                                      {-2,1,3},
                                      {3,2,-1}});
  
  Figure3D figure3d ("Conform");
  figure3d.draw_axes();

  auto v_par_3d = PEIBOS(f_3d, psi0_3d, generators_3d, 0.2);

  for (const auto& p : v_par_3d)
    figure3d.draw_parallelepiped(p.z, p.A, Color::green(0.5));

  // nD example of the PEIBOS algorithm

  VectorVar y_nd(3);
  AnalyticFunction f_nd({y_nd},{y_nd[0],y_nd[1],y_nd[2]});

  VectorVar X_nd(1);
  AnalyticFunction psi0_nd ({X_nd},{X_nd[0],1,1});

  vector<vector<int>> generators_nd ({{1,2,3},
                                      {-2,1,3},
                                      {3,2,-1},
                                      {1,-2,-3}});


  Figure2D figure_2d_nd_xy ("XY Plane", GraphicOutput::VIBES);
  figure_2d_nd_xy.set_window_properties({575,50},{500,500});
  figure_2d_nd_xy.set_axes(axis(0,{-1.2,1.2}), axis(1,{-1.2,1.2}));

  Figure2D figure_2d_nd_zy ("ZY Plane", GraphicOutput::VIBES);
  figure_2d_nd_zy.set_window_properties({1125,50},{500,500});
  figure_2d_nd_zy.set_axes(axis(2,{-1.2,1.2}), axis(1,{-1.2,1.2}));

  auto v_par_nd = PEIBOS(f_nd, psi0_nd, generators_nd, 0.02);

  for (const auto& p : v_par_nd)
  {
    auto vertices = p.vertices();
    for (const auto& vertice : vertices)
    {
      figure_2d_nd_zy.draw_point(vertice, {Color::red(),Color::red(0.5)});
      figure_2d_nd_xy.draw_point(vertice, {Color::red(),Color::red(0.5)});
    }
  }
    
}