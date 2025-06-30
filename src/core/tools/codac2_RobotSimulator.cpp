/** 
 *  codac2_RobotSimulator.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <math.h>
#include "codac2_RobotSimulator.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  RobotSimulator::RobotSimulator()
  { }

  Vector RobotSimulator::controller(const Vector& x, const Vector& wpt) const
  {
    assert_release(x.size() == 4);
    assert_release(wpt.size() == 2);

    Vector d = wpt - x.subvector(0,1);
    double dist_to_goal = std::sqrt(std::pow(d[0],2)+std::pow(d[1],2));
    double angle_to_goal = std::atan2(d[1],d[0]);
    double angle_error = angle_to_goal-x[2];
    angle_error = std::atan2(std::sin(angle_error),std::cos(angle_error)); // [-pi,pi]

    Vector u(2);

    // Rotational speed (proportional)
    u[0] = 2.0*angle_error;
    u[0] = std::clamp(u[0], -w_max, w_max);

    // Linear speed command
    double v_desired = v_max*std::tanh(dist_to_goal);
    u[1] = 2.0*(v_desired-x[3]);
    u[1] = std::clamp(u[1], -a_max, a_max);

    return u;
  }

  SampledTraj<Vector> RobotSimulator::simulate(
    const Vector& x0, double dt, std::list<Vector> wpts,
    SampledTraj<Vector>& u) const
  {
    assert_release(dt > 0.);
    assert_release(x0.size() == 4);

    double t = 0., dtheta = 0.;
    Vector xi = x0;
    SampledTraj<Vector> x;
    x.set(xi,t);

    bool away_from_wpt = false;
    while(!wpts.empty())
    {
      const auto& wpt = wpts.front();
      auto ui = controller(xi, wpt);

      xi[3] += ui[1]*dt;
      xi[3] = std::clamp(xi[3], v_min, v_max);

      xi[0] += xi[3]*std::cos(xi[2])*dt;
      xi[1] += xi[3]*std::sin(xi[2])*dt;

      double r = ui[0]*dt;
      dtheta += r;
      xi[2] += r;

      t += dt;
      x.set(xi,t);
      u.set(ui,t);

      double dist_to_goal = std::sqrt(std::pow(wpt[0]-xi[0],2)+std::pow(wpt[1]-xi[1],2));
      // If the new waypoint is already below the attainment threshold,
      // we move away from it to return to it
      away_from_wpt |= dist_to_goal >= dist_threshold;

      if((std::fabs(dtheta) > 2*PI) || (away_from_wpt && dist_to_goal < dist_threshold))
      {
        dtheta = 0.;
        away_from_wpt = false;
        wpts.pop_front();
      }
    }

    return x;
  }
}