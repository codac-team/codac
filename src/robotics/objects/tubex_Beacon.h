/** 
 *  Beacon class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_BEACON_H__
#define __TUBEX_BEACON_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  class Beacon
  {
    public:

      Beacon(double x = 0., double y = 0., double z = 0.);
      Beacon(const ibex::Vector& pos);
      Beacon(const ibex::IntervalVector& pos_box);
      double x() const;
      double y() const;
      double z() const;
      const ibex::Vector& pos() const;
      const ibex::IntervalVector& pos_box() const;

    protected:

      ibex::Vector m_pos = ibex::Vector(3, 0.);
      ibex::IntervalVector m_pos_box = ibex::IntervalVector(3);
  };
}

#endif