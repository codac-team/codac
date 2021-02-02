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

#include "codac_IntervalVector.h"

namespace codac
{
  class Beacon
  {
    public:

      Beacon(double x = 0., double y = 0., double z = 0.);
      Beacon(const ibex::Vector& pos);
      Beacon(const IntervalVector& pos_box);
      double x() const;
      double y() const;
      double z() const;
      const ibex::Vector& pos() const;
      const IntervalVector& pos_box() const;

    protected:

      ibex::Vector m_pos = ibex::Vector(3, 0.);
      IntervalVector m_pos_box = IntervalVector(3);
  };
}

#endif