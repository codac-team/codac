#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  set_nb_threads(max_threads());
  
  // {psi0,Sigma} is a gnomonic atlas of the box [-1,1]^2
  VectorVar X(1);
  AnalyticFunction psi0 ({X},{X[0],1});

  OctaSym id ({1, 2});
  OctaSym s ({-2, 1});

  vector<OctaSym> Sigma ({id,s,s*s,s.invert()});

  // In this box, we consider that the x-axis is the width of the linear sensor
  // and the y-axis is the time
  VectorVar y (2);
  auto L = 0.1*y[0];
  auto t = 1.2*y[1];

  // We construct the trajectory of the robot
  auto traj  = vec(pow(t,3)-t, 1-sqr(t));
  // We need its derivative to compute the orthogonal to the trajectory (for the sensor)
  auto dtraj =  vec(3*sqr(t)-1, -2*t);
  auto dtraj_norm = sqrt(sqr(dtraj[0])+sqr(dtraj[1]));

  // The image of the box [-1,1]^2 by f is the swept area
  AnalyticFunction f ({y},{traj[0]-L*dtraj[1]/dtraj_norm, traj[1]+L*dtraj[0]/dtraj_norm});

  // For the SepImage, we need a contractor on the initial set (here a simple box)
  IntervalVector X0 = IntervalVector::constant(2,{-1,1});
  CtcWrapper ctc_in (X0);
  
  // Separator on the area seen by a robot
  SepImage sep (f,psi0,Sigma,0.01,ctc_in);

  // Visualizing the separator
  IntervalVector Y0 ({{-0.8,0.8},{-0.7,1.3}});

  DefaultFigure::pave(Y0,sep,0.01);
}