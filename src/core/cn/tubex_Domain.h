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
  // todo: assert if structure of referenced domain changes (size, nb slices)

  class Ctc;
  class Contractor;
  class ContractorNetwork;

  class Domain
  {
    public:

      enum class Type { INTERVAL=0, INTERVAL_VECTOR=1, SLICE=2, TUBE=3, TUBE_VECTOR=4 };
      enum class ExternalRef { NONE=0, DOUBLE=1, INTERVAL=2, VECTOR=3, INTERVAL_VECTOR=4, SLICE=5, TUBE=6, TUBE_VECTOR=7 };

      Domain(const Domain& ad);
      Domain(double& d);
      Domain(ibex::Interval& i);
      Domain(ibex::Interval& i, double& extern_d);
      Domain(ibex::Interval& i, ibex::Interval& extern_i);
      Domain(const ibex::Interval& i);
      Domain(ibex::Vector& v);
      Domain(ibex::IntervalVector& iv);
      Domain(const ibex::IntervalVector& iv);
      Domain(tubex::Slice& s);
      Domain(tubex::Tube& t);
      Domain(const tubex::Tube& t);
      Domain(tubex::TubeVector& tv);
      Domain(const tubex::TubeVector& tv);
      ~Domain();

      int id() const;
      Type type() const;

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
      const std::vector<Contractor*>& contractors() const;
      void add_ctc(Contractor *ctc);

      double compute_volume() const;
      double get_saved_volume() const;
      void set_volume(double vol);

      bool is_empty() const;
      
      bool operator==(const Domain& x) const;
      bool operator!=(const Domain& x) const;

      bool is_component_of(const Domain& x) const;
      bool is_component_of(const Domain& x, int& component_id) const;

      bool is_slice_of(const Domain& x) const;
      bool is_slice_of(const Domain& x, int& slice_id) const;
      
      friend std::ostream& operator<<(std::ostream& str, const Domain& x);

      void add_data(double t, const ibex::Interval& y, ContractorNetwork& cn);
      void add_data(double t, const ibex::IntervalVector& y, ContractorNetwork& cn);

      const std::string dom_name(const std::vector<Domain*>& v_domains) const;
      void set_name(const std::string& name);

    protected:

      Domain(Type type, ExternalRef extern_object_type);
      const std::string var_name(const std::vector<Domain*>& v_domains) const;

      // Theoretical type of domain

        const Type m_type;

        union // reference to the values (in memory) this domain is made of
        {
          std::reference_wrapper<ibex::Interval> m_ref_values_i;
          std::reference_wrapper<ibex::IntervalVector> m_ref_values_iv;
          std::reference_wrapper<tubex::Slice> m_ref_values_s;
          std::reference_wrapper<tubex::Tube> m_ref_values_t;
          std::reference_wrapper<tubex::TubeVector> m_ref_values_tv;
        };

        union // if locally stored (such as intermediate variables or doubles to intervals):
        {
          ibex::Interval *m_i_ptr;
          ibex::IntervalVector *m_iv_ptr;
          Tube *m_t_ptr;
          TubeVector *m_tv_ptr;
        };

      // Origin type of the implementation of the domain

        ExternalRef m_extern_object_type;

        union // reference to the unique object (in memory) this domain represents
        {
          std::reference_wrapper<double> m_ref_extern_object_d;
          std::reference_wrapper<ibex::Interval> m_ref_extern_object_i;
          std::reference_wrapper<ibex::Vector> m_ref_extern_object_v;
          std::reference_wrapper<ibex::IntervalVector> m_ref_extern_object_iv;
          std::reference_wrapper<tubex::Slice> m_ref_extern_object_s;
          std::reference_wrapper<tubex::Tube> m_ref_extern_object_t;
          std::reference_wrapper<tubex::TubeVector> m_ref_extern_object_tv;
        };


      // todo: update this:
      std::map<double,double> m_map_data_s_lb, m_map_data_s_ub;
      std::map<double,ibex::Vector> m_map_data_lb, m_map_data_ub;

      std::vector<Contractor*> m_v_ctc;
      double m_volume = 0.;

      std::string m_name;
      int m_dom_id;

      static int dom_counter;

      friend class ContractorNetwork; // todo: remove this
  };
}

#endif