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

  auto v_par_2d = PEIBOS(f_2d, psi0_2d, generate_symmetries(generators_2d, psi0_2d), 0.2, {-0.2,0.});

  Figure2D figure_2d ("Henon Map", GraphicOutput::VIBES);
  figure_2d.set_window_properties({25,50},{500,500});
  figure_2d.set_axes({0,{-1.4,2.2}}, {1,{-0.4,0.3}});

  for (const auto& p : v_par_2d)
  {

    figure_2d.draw_parallelepiped(p.z, p.A, {Color::green(),Color::green(0.5)});
    figure_2d.draw_box(p.bounding_box(), {Color::blue()});
    for (const auto& vertice : p.vertices())
      figure_2d.draw_point(vertice, {Color::red(),Color::red(0.5)});
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

  Figure2D figure_3d_proj ("Conform projected", GraphicOutput::VIBES);
  figure_3d_proj.set_window_properties({25,600},{500,500});
  figure_3d_proj.set_axes({0,{-1.5,2.5}}, {1,{-2,2}});

  auto v_par_3d = PEIBOS(f_3d, psi0_3d, generate_symmetries(generators_3d, psi0_3d), 0.2);  

  for (const auto& p : v_par_3d)
  {
    figure3d.draw_parallelepiped(p.z, p.A, Color::green(0.5));
    Zonotope z = p.project({0,1});
    figure_3d_proj.draw_zonotope(z.z ,z.A , {Color::black(),Color::green(0.2)});
  }
    
  // nD example of the PEIBOS algorithm

  VectorVar y_nd(3);
  Matrix rot_matrix_1 ({ {1,0,0},
                          {0,1/std::sqrt(2.0),-1/std::sqrt(2.0)},
                          {0,1/std::sqrt(2.0),+1/std::sqrt(2.0)} });
  Matrix rot_matrix_2 ({ {1/std::sqrt(2.0),-1/std::sqrt(2.0),0},
                          {1/std::sqrt(2.0),+1/std::sqrt(2.0),0},
                          {0,0,1} });
  AnalyticFunction g_nd ({y_nd}, {y_nd[0]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2])), y_nd[1]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2])), y_nd[2]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2]))});
  AnalyticFunction f_nd ({y_nd}, rot_matrix_1 * rot_matrix_2 * g_nd(y_nd));
  
  VectorVar X_nd(1);
  AnalyticFunction psi0_nd ({X_nd},{X_nd[0],1,1});

  vector<vector<int>> generators_nd ({{1,2,3},
                                      {-2,1,3},
                                      {3,2,-1},
                                      {1,-2,-3}});

  Figure3D figure_3d_nd ("Cube on Sphere");
  figure_3d_nd.draw_axes(0.5);

  Figure2D figure_2d_nd_xy ("XY Plane", GraphicOutput::VIBES);
  figure_2d_nd_xy.set_window_properties({575,50},{500,500});
  figure_2d_nd_xy.set_axes(axis(0,{-1.,1.}), axis(1,{-1.,1.}));

  Figure2D figure_2d_nd_zy ("ZY Plane", GraphicOutput::VIBES);
  figure_2d_nd_zy.set_window_properties({1125,50},{500,500});
  figure_2d_nd_zy.set_axes(axis(0,{-1.,1.}), axis(1,{-1.,1.}));

  auto v_par_nd = PEIBOS(f_nd, psi0_nd, generate_symmetries(generators_nd, psi0_nd), 0.1);

  for (const auto& p : v_par_nd)
  {
    figure_3d_nd.draw_parallelepiped(p.z, p.A, Color::green(0.5));
    Zonotope z_xy = p.project({0,1});
    Zonotope z_zy = p.project({2,1});
    figure_2d_nd_xy.draw_zonotope(z_xy.z, z_xy.A, {Color::black(),Color::green(0.2)});
    figure_2d_nd_zy.draw_zonotope(z_zy.z, z_zy.A, {Color::black(),Color::green(0.2)});
  }
    
}