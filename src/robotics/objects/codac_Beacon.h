/** 
 *  Beacon class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_BEACON_H__
#define __CODAC_BEACON_H__

#include "codac_Vector.h"
#include "codac_IntervalVector.h"

namespace codac
{
  class Beacon
  {
    public:

      Beacon(double x = 0., double y = 0., double z = 0.);
      Beacon(const Vector& pos);
      Beacon(const IntervalVector& pos_box);
      double x() const;
      double y() const;
      double z() const;
      const Vector& pos() const;
      const IntervalVector& pos_box() const;

    protected:

      Vector m_pos = Vector(3, 0.);
      IntervalVector m_pos_box = IntervalVector(3);
  };
}

#endif