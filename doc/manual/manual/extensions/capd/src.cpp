// Author : Maël GODARD
// Adapted from CAPD examples

// [codac-capd-1-beg]
#include <codac-core.h>
#include <codac-graphics.h>
#include <codac-capd.h>
#include <capd/capdlib.h>
// [codac-capd-1-end]

using namespace codac2;

// [peibos-capd-2-beg]
Parallelepiped parallelepiped_inclusion_(const IntervalVector& Y, const IntervalMatrix& Jf, const Matrix& Jf_tild, const AnalyticFunction<VectorType>& psi_0, const OctaSym& sigma, const IntervalVector& X)
{
  // Computation of the Jacobian of g = f o sigma(psi_0)
  IntervalMatrix Jg = Jf * (sigma.permutation_matrix().template cast<Interval>()) * psi_0.diff(X);

  Vector z = Y.mid();
  // A is an approximation of the Jacobian of g at the center of X
  Matrix A = (Jf_tild * sigma.permutation_matrix() * (psi_0.diff(X.mid()).mid()));

  // Maximum error computation
  double rho = error_peibos(Y, z, Jg, A, X);

  // Inflation of the parallelepiped
  Matrix A_inf = inflate_flat_parallelepiped(A, X.rad(), rho);

  return Parallelepiped(z, A_inf);
}
// [peibos-capd-2-end]

// [peibos-capd-1-beg]
std::map<double,std::vector<Parallelepiped>> reach_set_(const std::map<double, std::vector<std::tuple<PEIBOS_CAPD_Key,IntervalVector,IntervalMatrix>>>& peibos_output)
{
  std::map<double,std::vector<Parallelepiped>> output;

  for (const auto& [time,vec] : peibos_output)
  {
    for (const auto& [key, z, Jf] : vec)
    {
      auto p = parallelepiped_inclusion_(z, Jf, Jf.mid(), key.psi_0, key.sigma, key.box);

      output[time].push_back(p);
    }
  }
  
  return output;
}
// [peibos-capd-1-end]

int main()
{
  set_nb_threads(max_threads());
  
  // Equation of the pendulum with friction
  // [codac-capd-2-beg]
  capd::IMap vectorField("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
 
  vectorField.setParameter("l",capd::Interval(2.)); // length of the pendulum equal to 2
  vectorField.setParameter("g",capd::Interval(10.));  // gravity acceleration equal to 10
  // [codac-capd-2-end]

  // the solver uses high order enclosure method to verify the existence 
  // of the solution. The order is set to 20.
  // [codac-capd-3-beg]
  capd::IOdeSolver solver(vectorField,20);
  solver.setAbsoluteTolerance(1e-10);
  solver.setRelativeTolerance(1e-10);
  // [codac-capd-3-end]

  // [codac-capd-4-beg]
  capd::ITimeMap timeMap(solver);
  capd::Interval initialTime(0.);  // initial time (t0)
  capd::Interval finalTime(20.); // final time (tf)
  // [codac-capd-4-end]

  // initial set
  // [codac-capd-5-beg]
  capd::IVector c(2);
  c[0] = -M_PI/2.;
  c[1] = 0.;
  // take some box around c
  c[0] += capd::Interval(-1,1)*1e-2;
  c[1] += capd::Interval(-1,1)*1e-2;

  // define a doubleton representation of the interval vector c
  capd::C0HORect2Set s(c);
  // [codac-capd-5-end]

  // [codac-capd-7-beg]
  capd::ITimeMap::SolutionCurve solution(initialTime);
  timeMap(finalTime,s,solution);
  // [codac-capd-7-end]

  // we integrate the set s over the time T
  // [codac-capd-6-beg]
  capd::Interval T(1);
  capd::IVector result = timeMap(T,s);
  // [codac-capd-6-end]

  // [codac-capd-8-beg]

  auto tdomain = create_tdomain(Interval(0,20),0.05, true); // true to have gates
  auto codac_tube = to_codac(solution, tdomain);

  // [codac-capd-8-end]

  if (false)
  {
    // [codac-capd-9-beg]
    std::cout << "\ninitial set: " << c;
    std::cout << "\ndiam(initial set): " << diam(c) << std::endl;

    std::cout << "\n\nafter time=" << T << " the image is: " << result;
    std::cout << "\ndiam(image): " << diam(result) << std::endl << std::endl;

    DefaultFigure::set_axes(axis(0,{-2,1.5}),axis(1,{-2,3}));

    DefaultFigure::draw_tube(codac_tube, ColorMap::blue_tube());
    DefaultFigure::draw_tube(codac_tube, Color::black());

    for (float t=0.;t<20.;t+=0.05)
      DefaultFigure::draw_box(to_codac(solution(t)), {Color::none(), Color::orange(0.5)});

    DefaultFigure::draw_box(to_codac(c),Color::green());
    DefaultFigure::draw_box(to_codac(result),Color::red());
    // [codac-capd-9-end]
  }

  {
    // [peibos-capd-3-beg]
    capd::IMap vectorField_pend("par:l,g;var:t,w;fun:w,-sin(t)*5 - 0.5*w;");
 
    VectorVar X_2d(1);
    AnalyticFunction psi0_pend ({X_2d},{0.1*X_2d[0],0.1});
    OctaSym id_2d ({1,2});
    OctaSym s ({-2,1});

    auto peibos_output_pend = PEIBOS(vectorField_pend, 20., 0.2, psi0_pend, {id_2d,s,s*s,s.invert()}, 0.02, {-M_PI/2.,0.});

    auto m_v_par_2d_pend = reach_set(peibos_output_pend);
    // [peibos-capd-3-end]
  }

  {
    // [peibos-capd-4-beg]
    capd::IMap vectorField_lorenz("par:sigma,rho,beta;var:x1,x2,x3;fun:10*(x2-x1),28*x1-x2-x1*x3,-2.6*x3+x1*x2;");
    vectorField_lorenz.setParameter("sigma", 10.);
    vectorField_lorenz.setParameter("rho", 28.);
    vectorField_lorenz.setParameter("beta", 8/3);

    VectorVar X_3d(2);
    AnalyticFunction psi0_lorenz ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});
    OctaSym id_3d ({1,2,3});
    OctaSym s1 ({-2,1,3});
    OctaSym s2 ({3,2,-1});

    auto peibos_output_lorenz = PEIBOS(vectorField_lorenz, 0.1, 0.05, psi0_lorenz, {id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()}, 0.1);

    auto m_v_par_lorenz = reach_set(peibos_output_lorenz);
    // [peibos-capd-4-end]
  }
 
}