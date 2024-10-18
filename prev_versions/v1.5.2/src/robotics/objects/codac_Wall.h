/** 
 *  Beacon class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_WALL_H__
#define __CODAC_WALL_H__

#include "codac_Vector.h"
#include "codac_IntervalVector.h"

namespace codac
{
  class Wall
  {
    public:

      Wall(const Vector& c1, const Vector& c2);
      bool contains(const Vector& p) const;
    
    //protected:

      Vector c1, c2;
  };

  Vector operator&(const Wall& w1, const Wall& w2);

  double shorter_dist_to_walls(const std::vector<Wall>& v_walls, const Vector& p, double bearing);
  Vector shorter_contact_to_walls(const std::vector<Wall>& v_walls, const Vector& p);
}

#endif