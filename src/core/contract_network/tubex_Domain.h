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
#include "tubex_Contractor.h"
#include "tubex_ContractorNetwork.h"

namespace tubex
{
  enum class DomainType { INTERVAL, INTERVAL_VECTOR, SLICE, TUBE, TUBE_VECTOR };

  class Ctc;
  class Contractor;
  class ContractorNetwork;

  class Domain
  {
    public:

      Domain(const Domain& ad);
      Domain(double d);
      Domain(ibex::Interval& i);
      Domain(ibex::Vector& v);
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

      void add_data(double t, const ibex::Interval& y, ContractorNetwork& cn);
      void add_data(double t, const ibex::IntervalVector& y, ContractorNetwork& cn);

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

      // todo: update this:
      std::map<double,double> m_map_data_s_lb, m_map_data_s_ub;
      std::map<double,ibex::Vector> m_map_data_lb, m_map_data_ub;

      std::vector<Contractor*> m_v_ctc;
      double m_volume = 0.;
  };
}

#endif