from codac import *
import numpy as np

if __name__=="__main__":
  set_nb_threads(max_threads())

  # {psi0,Sigma} is a gnomonic atlas of the box [-1,1]^2
  X = VectorVar(1)
  psi0 = AnalyticFunction([X],[X[0],1])

  id = OctaSym([1,2])
  s = OctaSym([-2,1])

  Sigma = [id,s,s*s,s.invert()]

  # In this box, we consider that the x-axis is the width of the linear sensor
  # and the y-axis is the time
  y = VectorVar(2)
  L = 0.1*y[0]
  t = 1.2*y[1]

  # We construct the trajectory of the robot
  traj = vec(pow(t,3)-t,1-sqr(t))
  # We need its derivative to compute the orthogonal to the trajectory (for the sensor)
  dtraj = vec(3*sqr(t)-1, -2*t)
  dtraj_norm = sqrt(sqr(dtraj[0])+sqr(dtraj[1]))

  # The image of the box [-1,1]^2 by f is the swept area
  f = AnalyticFunction([y], [traj[0]-L*dtraj[1]/dtraj_norm, traj[1]+L*dtraj[0]/dtraj_norm])

  # For the SepImage, we need a contractor on the initial set (here a simple box)
  X0 = IntervalVector.constant(2,[-1,1])
  ctc_in = CtcWrapper(X0)

  # Separator on the area seen by a robot
  sep = SepImage(f,psi0,Sigma,0.01,ctc_in)

  # Visualizing the separator
  Y0 = IntervalVector([[-0.8,0.8],[-0.7,1.3]])

  DefaultFigure.pave(Y0,sep,0.01)

