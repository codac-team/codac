#include <codac>
#include <codac-capd.h>

using namespace std;
using namespace codac2;

int main()
{
  set_nb_threads(max_threads());
  
  ColorMap cmap = ColorMap::rainbow();
  VectorVar X_2d(1);
  VectorVar X_3d(2);

  // Julien's example

  capd::IMap vectorField_2d("var:x1,x2; fun:1, sin(x1);");

  double tf_2d = 8.0;
  double dt_2d = 2.0;
  
  AnalyticFunction psi0_2d ({X_2d},{0.5*X_2d[0],0.5});

  OctaSym id_2d ({1,2});
  OctaSym s ({-2,1});
  
  auto peibos_output = PEIBOS(vectorField_2d, tf_2d, dt_2d, psi0_2d, {id_2d,s,s*s,s.invert()}, 0.1, true);

  Figure2D output ("julien_3_4",GraphicOutput::VIBES);
  output.set_axes(axis(0,{-1,10}),axis(1,{-1,3}));
  output.set_window_properties({25,100},{800,800});


  auto m_v_par_2d = reach_set(peibos_output);

  for (auto& [t, v_par_2d] : m_v_par_2d)
    for (const auto& par: v_par_2d)
      output.draw_parallelepiped(par, cmap.color(t/tf_2d));

  // Pendulum

  capd::IMap vectorField_pend("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
 
  vectorField_pend.setParameter("l",capd::interval(2.));
  vectorField_pend.setParameter("g",capd::interval(10.));

  double tf_pend = 20.0;
  double dt_pend = 0.2;
  
  AnalyticFunction psi0_pend ({X_2d},{0.1*X_2d[0],0.1});

  auto peibos_output_pend = PEIBOS(vectorField_pend, tf_pend, dt_pend, psi0_pend, {id_2d,s,s*s,s.invert()}, 0.02, {-M_PI/2.,0.}, true);

  Figure2D output_pend ("Pendulum",GraphicOutput::VIBES | GraphicOutput::IPE);
  output_pend.set_axes(axis(0,{-2,2}),axis(1,{-3,3}));
  output_pend.set_window_properties({800,100},{800,800});

  auto m_v_par_2d_pend = reach_set(peibos_output_pend);

  for (auto& [t, v_par_2d] : m_v_par_2d_pend)
    for (const auto& par: v_par_2d)
      output_pend.draw_parallelepiped(par, cmap.color(t/tf_pend));

  // Lorenz

  Figure3D figure_lorenz ("Lorenz");
  ColorMap cmap_lorenz = ColorMap::rainbow(0.5);

  capd::IMap vectorField_lorenz("par:sigma,rho,beta;var:x1,x2,x3;fun:10*(x2-x1),28*x1-x2-x1*x3,-2.6*x3+x1*x2;");
  vectorField_lorenz.setParameter("sigma", 10.);
  vectorField_lorenz.setParameter("rho", 28.);
  vectorField_lorenz.setParameter("beta", 8/3);

  double tf_lorenz = 0.1;
  double dt_lorenz = 0.05;

  AnalyticFunction psi0_lorenz ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});

  OctaSym id_3d ({1,2,3});
  OctaSym s1 ({-2,1,3});
  OctaSym s2 ({3,2,-1});

  auto peibos_output_lorenz = PEIBOS(vectorField_lorenz, tf_lorenz, dt_lorenz, psi0_lorenz, {id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()}, 0.1, true);

  auto m_v_par_lorenz = reach_set(peibos_output_lorenz);

  for (auto& [t, v_par_lorenz] : m_v_par_lorenz)
    for (const auto& par: v_par_lorenz)
      figure_lorenz.draw_parallelepiped(par, {cmap_lorenz.color(t/tf_lorenz),"t:"+to_string(t)});

}