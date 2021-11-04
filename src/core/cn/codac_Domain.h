/** 
 *  \file
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DOMAIN_H__
#define __CODAC_DOMAIN_H__

#include <functional>
#include "codac_Interval.h"
#include "codac_Variable.h"
#include "codac_IntervalVector.h"
#include "codac_Slice.h"
#include "codac_Tube.h"
#include "codac_TubeVector.h"
#include "codac_Contractor.h"
#include "codac_Hashcode.h"

namespace codac
{
  // todo: assert if structure of referenced domain changes (size, nb slices)

  class DynCtc;
  class Contractor;
  class ContractorNetwork;

  class Domain
  {
    public:

      enum class Type { T_INTERVAL, T_INTERVAL_VECTOR, T_SLICE, T_TUBE, T_TUBE_VECTOR };
      enum class MemoryRef { M_DOUBLE, M_INTERVAL, M_INTERVAL_VAR, M_VECTOR, M_INTERVAL_VECTOR, M_INTERVAL_VECTOR_VAR, M_SLICE, M_TUBE, M_TUBE_VECTOR };

      Domain();
      Domain(const Domain& ad);
      // todo: constructor with non-ref double?
      Domain(double& d);
      Domain(Interval& i);
      Domain(Interval& i, double& extern_d);
      Domain(Interval& i, Interval& extern_i);
      Domain(const Interval& i, bool interm_var = false);
      Domain(IntervalVar& i);
      Domain(Vector& v);
      // todo: ? Domain(const Vector& v);
      Domain(IntervalVector& iv);
      Domain(const IntervalVector& iv, bool interm_var = false);
      Domain(IntervalVectorVar& iv);
      Domain(Slice& s);
      Domain(Tube& t);
      Domain(const Tube& t, bool interm_var = false);
      Domain(TubeVector& tv);
      Domain(const TubeVector& tv, bool interm_var = false);
      ~Domain();

      const Domain& operator=(const Domain& ad);
      void set_ref_values(const Domain& ad);

      int id() const;
      Type type() const;

      Interval& interval();
      const Interval& interval() const;
      IntervalVector& interval_vector();
      const IntervalVector& interval_vector() const;
      Slice& slice();
      const Slice& slice() const;
      Tube& tube();
      const Tube& tube() const;
      TubeVector& tube_vector();
      const TubeVector& tube_vector() const;

      std::vector<Contractor*>& contractors();
      const std::vector<Contractor*>& contractors() const;
      void add_ctc(Contractor *ctc);

      bool is_var() const;
      bool is_var_not_associated() const;

      double compute_volume() const;
      double get_saved_volume() const;
      void set_volume(double vol);

      bool is_interm_var() const;
      void reset_value();

      bool is_empty() const;
      
      bool operator==(const Domain& x) const;
      bool operator!=(const Domain& x) const;

      bool is_component_of(const Domain& x) const;
      bool is_component_of(const Domain& x, int& component_id) const;

      bool is_slice_of(const Domain& x) const;
      bool is_slice_of(const Domain& x, int& slice_id) const;

      void add_data(double t, const Interval& y, ContractorNetwork& cn);
      void add_data(double t, const IntervalVector& y, ContractorNetwork& cn);

      const std::string dom_name(const std::map<DomainHashcode,Domain*> m_domains) const;
      void set_name(const std::string& name);

      static bool all_dyn(const std::vector<Domain>& v_domains);
      static bool all_slices(const std::vector<Domain>& v_domains);
      static bool dyn_same_slicing(const std::vector<Domain>& v_domains);
      static int total_size(const std::vector<Domain>& v_domains);

      static Domain vector_component(Domain& x, int i);

      const std::string dom_type_str() const;
      friend std::ostream& operator<<(std::ostream& str, const Domain& x);

    protected:

      Domain(Type type, MemoryRef memory_type);
      const std::string var_name(const std::map<DomainHashcode,Domain*> m_domains) const;

      // Theoretical type of domain

        Type m_type;

        union // reference to the values (in memory) this domain is made of
        {
          std::reference_wrapper<Interval> m_ref_values_i;
          std::reference_wrapper<IntervalVector> m_ref_values_iv;
          std::reference_wrapper<Slice> m_ref_values_s;
          std::reference_wrapper<Tube> m_ref_values_t;
          std::reference_wrapper<TubeVector> m_ref_values_tv;
        };

        union // if locally stored (such as intermediate variables or doubles to intervals):
        {
          Interval *m_i_ptr;
          IntervalVector *m_iv_ptr;
          Tube *m_t_ptr;
          TubeVector *m_tv_ptr;
        };

      // Memory implementation of the domain

        MemoryRef m_memory_type;

        union // reference to the unique object (in memory) this domain represents
        {
          std::reference_wrapper<double> m_ref_memory_d;
          std::reference_wrapper<Interval> m_ref_memory_i;
          std::reference_wrapper<IntervalVar> m_ref_memory_ivar;
          std::reference_wrapper<Vector> m_ref_memory_v;
          std::reference_wrapper<IntervalVector> m_ref_memory_iv;
          std::reference_wrapper<IntervalVectorVar> m_ref_memory_ivvar;
          std::reference_wrapper<Slice> m_ref_memory_s;
          std::reference_wrapper<Tube> m_ref_memory_t;
          std::reference_wrapper<TubeVector> m_ref_memory_tv;
        };

      // Possibly initial value (for intermediate variables to be reset)

        union // if locally stored (such as intermediate variables or doubles to intervals):
        {
          Interval *m_init_i_ptr;
          IntervalVector *m_init_iv_ptr;
          Tube *m_init_t_ptr;
          TubeVector *m_init_tv_ptr;
        };


      // todo: update this:
      std::map<double,double> m_map_data_s_lb, m_map_data_s_ub;
      std::map<double,Vector> m_map_data_lb, m_map_data_ub;

      Trajectory m_traj_lb, m_traj_ub;

      std::vector<Contractor*> m_v_ctc;
      double m_volume = -1.;

      std::string m_name;
      int m_dom_id;

      static int dom_counter;

      friend class ContractorNetwork; // todo: remove this
      friend class DomainHashcode;
  };
}

#endif