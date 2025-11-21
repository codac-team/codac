#include <codac>
#include <codac-capd.h>

using namespace std;
using namespace codac2;

int main()
{
  // Julien's example

  capd::IMap vectorField_2d("var:x1,x2; fun:1, sin(x1);");

  double tf_2d = 8.0;
  
  VectorVar X_2d(1);
  AnalyticFunction psi0_2d ({X_2d},{0.5*X_2d[0],0.5});

  OctaSym id_2d ({1,2});
  OctaSym s ({-2,1});
  
  auto peibos_output = PEIBOS(vectorField_2d, tf_2d, psi0_2d, {id_2d,s,s*s,s.invert()}, 0.1, true);

  Figure2D output ("julien_3_4",GraphicOutput::VIBES);
  output.set_axes(axis(0,{-1,10}),axis(1,{-1,3}));
  output.set_window_properties({50,100},{800,800});

  ColorMap cmap = ColorMap::rainbow();

  vector<double> time_samples_2d {1e-20,2.0,4.0,6.0,8.0};

  for (double t : time_samples_2d)
  {
    auto v_par_2d = reach_set(peibos_output,t);
    for (const auto& par: v_par_2d)
      output.draw_parallelepiped(par, cmap.color(t/tf_2d));
  }

  // Pendulum

  capd::IMap vectorField_pend("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
 
  vectorField_pend.setParameter("l",capd::interval(2.));
  vectorField_pend.setParameter("g",capd::interval(10.));

  double tf_pend = 20.0;
  
  AnalyticFunction psi0_pend ({X_2d},{1e-2*X_2d[0],1e-2});

  auto peibos_output_pend = PEIBOS(vectorField_pend, tf_pend, psi0_pend, {id_2d,s,s*s,s.invert()}, 0.1, {-M_PI/2.,0.}, true);

  Figure2D output_pend ("Pendulum",GraphicOutput::VIBES | GraphicOutput::IPE);
  output_pend.set_axes(axis(0,{-2,2}),axis(1,{-4,4}));
  output_pend.set_window_properties({50,100},{800,800});

  for (double t = 1e-20; t <= tf_pend; t += 0.05)
  {
    auto v_par_2d = reach_set(peibos_output_pend,t);
    for (const auto& par: v_par_2d)
      output_pend.draw_parallelepiped(par, cmap.color(t/tf_pend));
  }
}