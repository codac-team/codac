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

#include <functional>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Slice.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  enum class DomainType { INTERVAL, INTERVAL_VECTOR, SLICE, TUBE, TUBE_VECTOR };

  class AbstractContractor;

  class AbstractDomain
  {
    public:

      AbstractDomain(const AbstractDomain& ad);
      AbstractDomain(ibex::Interval& i);
      AbstractDomain(ibex::IntervalVector& iv);
      AbstractDomain(tubex::Slice& s);
      AbstractDomain(tubex::Tube& t);
      AbstractDomain(tubex::TubeVector& tv);
      ~AbstractDomain();

      DomainType type() const;

      double volume() const;
      bool is_empty() const;
      
      bool operator==(const AbstractDomain& x) const;
      bool operator!=(const AbstractDomain& x) const;
      bool is_component_of(const AbstractDomain* x) const;
      bool is_slice_of(const AbstractDomain* x) const;
      
      friend std::ostream& operator<<(std::ostream& str, const AbstractDomain& x);

    //protected:

      const DomainType m_type;

      union
      {
        std::reference_wrapper<ibex::Interval> m_i;
        std::reference_wrapper<ibex::IntervalVector> m_iv;
        std::reference_wrapper<tubex::Slice> m_s;
        std::reference_wrapper<tubex::Tube> m_t;
        std::reference_wrapper<tubex::TubeVector> m_tv;
      };

      std::vector<AbstractContractor*> m_v_ctc;
      double m_volume = 0.;
  };
}

#endif