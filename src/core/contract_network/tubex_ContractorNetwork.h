/** 
 *  \file
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONTRACTOR_NETWORK_H__
#define __TUBEX_CONTRACTOR_NETWORK_H__

#include <deque>
#include <initializer_list>
#include "ibex_Ctc.h"
#include "tubex_Ctc.h"
#include "tubex_Domain.h"
#include "tubex_Contractor.h"
#include "tubex_CtcDeriv.h"

namespace ibex
{
  class Ctc;
}

namespace tubex
{
  class Domain;
  class DomainSingleton;
  class Contractor;
  class Ctc;
  class CtcDeriv;

  class ContractorNetwork
  {
    public:

      ContractorNetwork();
      ~ContractorNetwork();

      int nb_ctc() const;
      int nb_dom() const;
      int nb_ctc_in_stack() const;
      void set_fixedpoint_ratio(float r);
      void set_all_contractors_active();

      double contract(bool verbose = false);
      double contract_during(double dt, bool verbose = false);

      ibex::Interval& create_var(const ibex::Interval& i);
      ibex::IntervalVector& create_var(const ibex::IntervalVector& iv);
      Tube& create_var(const Tube& t);
      TubeVector& create_var(const TubeVector& tv);

      void add(ibex::Ctc& ctc, const std::vector<Domain>& v_domains);
      void add(tubex::Ctc& ctc, const std::vector<Domain>& v_domains);

      void add_data(Tube& tube, double t, const ibex::Interval& y);
      void add_data(TubeVector& tube, double t, const ibex::IntervalVector& y);

      ibex::IntervalVector& subvector(ibex::IntervalVector& i, int start_index, int end_index);
      TubeVector& subvector(TubeVector& i, int start_index, int end_index);

      void set_name(Domain dom, const std::string& name);
      void set_name(Contractor ctc, const std::string& name);
      void print_dot_graph() const;


    protected:

      Domain* add_domain(Domain *ad);
      void add_domain(Domain *ad, Contractor *ac);
      Contractor* add_contractor(Contractor *&ac);
      void propagate_ctc_from_domain(Domain *dom, Contractor *ctc_to_avoid = NULL);
      void add_to_queue(Contractor *ac, std::deque<Contractor*>& ctc_deque);

      std::vector<Contractor*> m_v_ctc;
      std::vector<Domain*> m_v_domains;
      std::deque<Contractor*> m_deque;

      float m_fixedpoint_ratio = 0.0001;
      double m_contraction_duration_max = std::numeric_limits<double>::infinity();

      CtcDeriv *m_ctc_deriv = NULL;

      friend class Domain;
  };
}

#endif