/** 
 *  \file
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_ABSTRACT_DOMAIN_H__
#define __TUBEX_ABSTRACT_DOMAIN_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

#ifdef INTERVAL
#undef INTERVAL
#endif

namespace tubex
{
  enum DomainType { INTERVAL, INTERVAL_VECTOR, TUBE, TUBE_VECTOR };

  class AbstractDomain
  {
    public:

      AbstractDomain(ibex::Interval& i);
      AbstractDomain(ibex::IntervalVector& iv);
      AbstractDomain(tubex::Tube& t);
      AbstractDomain(tubex::TubeVector& tv);

      DomainType type() const;

      double volume() const;
      bool is_empty() const;
      
      friend std::ostream& operator<<(std::ostream& str, const AbstractDomain& x);

    //protected:

      const DomainType m_type;

      ibex::Interval& m_i;
      ibex::IntervalVector& m_iv;
      tubex::Tube& m_t;
      tubex::TubeVector& m_tv;

      static ibex::Interval m_dump_i;
      static ibex::IntervalVector m_dump_iv;
      static tubex::Tube m_dump_t;
      static tubex::TubeVector m_dump_tv;
  };
}

#endif