/** 
 *  \file
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DOMAIN_H__
#define __TUBEX_DOMAIN_H__

#include <functional>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Slice.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  enum class DomainType { INTERVAL, INTERVAL_VECTOR, SLICE, TUBE, TUBE_VECTOR };

  class Contractor;

  class Domain
  {
    public:

      Domain(const Domain& ad);
      Domain(ibex::Interval& i);
      Domain(ibex::IntervalVector& iv);
      Domain(tubex::Slice& s);
      Domain(tubex::Tube& t);
      Domain(tubex::TubeVector& tv);
      ~Domain();

      DomainType type() const;

      ibex::Interval& interval();
      const ibex::Interval& interval() const;
      ibex::IntervalVector& interval_vector();
      const ibex::IntervalVector& interval_vector() const;
      tubex::Slice& slice();
      const tubex::Slice& slice() const;
      tubex::Tube& tube();
      const tubex::Tube& tube() const;
      tubex::TubeVector& tube_vector();
      const tubex::TubeVector& tube_vector() const;

      std::vector<Contractor*>& contractors();

      double compute_volume() const;
      double get_saved_volume() const;
      void set_volume(double vol);

      bool is_empty() const;
      
      bool operator==(const Domain& x) const;
      bool operator!=(const Domain& x) const;
      
      friend std::ostream& operator<<(std::ostream& str, const Domain& x);


    protected:

      const DomainType m_type;

      union
      {
        std::reference_wrapper<ibex::Interval> m_i;
        std::reference_wrapper<ibex::IntervalVector> m_iv;
        std::reference_wrapper<tubex::Slice> m_s;
        std::reference_wrapper<tubex::Tube> m_t;
        std::reference_wrapper<tubex::TubeVector> m_tv;
      };

      std::vector<Contractor*> m_v_ctc;
      double m_volume = 0.;
  };
}

#endif