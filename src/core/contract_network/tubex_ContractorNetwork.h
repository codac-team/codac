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
#include "tubex_AbstractDomain.h"
#include "tubex_AbstractContractor.h"

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

      void add(ibex::Ctc& ctc, std::initializer_list<AbstractDomain> list);
      void add(tubex::Ctc& ctc, std::initializer_list<AbstractDomain> list);


    protected:

      AbstractDomain* add_domain(AbstractDomain *ad);
      void add_domain(AbstractDomain *ad, AbstractContractor *ac);
      void add_contractor(AbstractContractor *&ac);

      std::vector<AbstractContractor*> m_v_ctc;
      std::vector<AbstractDomain*> m_v_domains;
      std::deque<AbstractContractor*> m_deque;

      float m_fixedpoint_ratio = 0.0001;
      double m_contraction_duration_max = std::numeric_limits<double>::infinity();
  };
}

#endif