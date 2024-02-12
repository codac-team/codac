/** 
 *  Wall class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifdef _MSC_VER
// Enable additional features in math.h.
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <math.h>
#endif // _MSC_VER

#include <iostream>
#include "codac_Wall.h"
#include "codac_predef_values.h"

using namespace std;
using namespace ibex;

namespace codac
{
  Wall::Wall(const Vector& c1_, const Vector& c2_) : c1(c1_), c2(c2_)
  {
  }

  bool Wall::contains(const Vector& p) const
  {
    Vector ab { c2[0]-c1[0], c2[1]-c1[1] };
    Vector ac { p[0]-c1[0], p[1]-c1[1] };

    double dp_AB = ab[0]*ab[0] + ab[1]*ab[1];
    double dp_AC = ab[0]*ac[0] + ab[1]*ac[1];
    return Interval(0.,dp_AB).contains(dp_AC);
  }

  Vector operator&(const Wall& w1, const Wall& w2)
  {
    const double& x1 = w1.c1[0]; const double& x2 = w1.c2[0];
    const double& x3 = w2.c1[0]; const double& x4 = w2.c2[0];

    const double& y1 = w1.c1[1]; const double& y2 = w1.c2[1];
    const double& y3 = w2.c1[1]; const double& y4 = w2.c2[1];

    return {
      ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4)),
      ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))
    };
  }

  double shorter_dist_to_walls(const std::vector<Wall>& v_walls, const Vector& p, double bearing)
  {
    Wall w0 { p, { p[0]+999999.*cos(bearing), p[1]+999999.*sin(bearing) }};
    double min_dist = oo;

    for(const auto& wi : v_walls)
    {
      Vector pi = w0 & wi;
      if(!wi.contains(pi) || !w0.contains(pi))
        continue;
      double dist = pow(p[0]-pi[0],2) + pow(p[1]-pi[1],2);
      min_dist = dist < min_dist ? dist : min_dist;
    }

    return sqrt(min_dist);
  }

  Vector shorter_contact_to_walls(const std::vector<Wall>& v_walls, const Vector& p)
  {
    double min_dist = oo;
    double bearing;

    for(double a = 0. ; a < 2.*M_PI ; a+=0.1)
    {
      double dist = shorter_dist_to_walls(v_walls, p, a);
      if(dist < min_dist)
      {
        min_dist = dist;
        bearing = a;
      }
    }

    return { min_dist, bearing };
  }
}