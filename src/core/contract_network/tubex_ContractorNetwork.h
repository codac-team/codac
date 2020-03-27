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
#include "tubex_Domain.h"
#include "tubex_Contractor.h"

namespace tubex
{
  class ContractorNetwork
  {
    public:

      ContractorNetwork();
      ~ContractorNetwork();

      int nb_ctc() const;
      int nb_dom() const;
      int nb_ctc_in_stack() const;
      void set_fixedpoint_ratio(float r);

      double contract(bool verbose = false);
      double contract_during(double dt, bool verbose = false);

      ibex::Interval& create_var(const ibex::Interval& i_);
      ibex::IntervalVector& create_var(const ibex::IntervalVector& iv_);

      void add(ibex::Ctc& ctc, const std::vector<Domain>& v_domains);
      void add(tubex::Ctc& ctc, const std::vector<Domain>& v_domains);


    protected:

      Domain* add_domain(Domain *ad);
      void add_domain(Domain *ad, Contractor *ac);
      void add_contractor(Contractor *&ac);

      std::vector<Contractor*> m_v_ctc;
      std::vector<Domain*> m_v_domains;
      std::deque<Contractor*> m_deque;

      float m_fixedpoint_ratio = 0.0001;
      double m_contraction_duration_max = std::numeric_limits<double>::infinity();
  };
}

#endif