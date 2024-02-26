/** 
 *  Codac - Examples
 *  Arithmetic on tubes: simple example
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Guaranteed Computation of Robot Trajectories"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main()
{

  float dt = 0.001;
  Interval tdomain(0.,10.);

  TFunction f("(cos(t) ; cos(t)+t/10 ; sin(t)+t/10 ; sin(t))"); // 4d temporal function
  TrajectoryVector traj(tdomain, f); // 4d trajectory defined over [0,10]

  // 1d tube [x](·) defined as a union of the 4 trajectories
  Tube x = Tube(traj[0], dt) | traj[1] | traj[2] | traj[3];

  vibes::beginDrawing();

  VIBesFigTube fig("Tube");
  fig.set_properties(100, 100, 600, 300);
  fig.add_tube(&x, "x", "#376D7C[lightGray]");
  fig.add_trajectories(&traj, "trajs");
  fig.show();

  vibes::endDrawing();
}