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
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_Ctc.h"
#include "tubex_AbstractContractor.h"
#include "tubex_AbstractDomain.h"

namespace tubex
{
  class ContractorNetwork
  {
    public:

      ContractorNetwork();
      ~ContractorNetwork();

      int nb_ctc() const;
      int nb_dom() const;

      void contract(float r = 0.0001);

      ibex::Interval& create_var(const ibex::Interval& i_);
      ibex::IntervalVector& create_var(const ibex::IntervalVector& i_);

      void add(ibex::Ctc& ctc, ibex::IntervalVector& iv);
      void add(ibex::Ctc& ctc, ibex::Interval& i1, ibex::Interval& i2, ibex::Interval& i3);
      void add(ibex::Ctc& ctc, ibex::IntervalVector& i1, ibex::IntervalVector& i2, ibex::IntervalVector& i3);
      void add(ibex::Ctc& ctc, ibex::Interval& i1, ibex::Interval& i2, ibex::Interval& i3, ibex::Interval& i4);
      void add(ibex::Ctc& ctc, ibex::Interval& i1, ibex::Interval& i2, ibex::Interval& i3, ibex::Interval& i4, ibex::Interval& i5);
      void add(tubex::Ctc& ctc, ibex::Interval& i1, ibex::Interval& i2, tubex::Tube& i3, tubex::Tube& i4);
      void add(tubex::Ctc& ctc, ibex::Interval& i1, ibex::IntervalVector& i2, tubex::TubeVector& i3, tubex::TubeVector& i4);
      void add(tubex::Ctc& ctc, tubex::Tube& i1, tubex::Tube& i2);
      void add(tubex::Ctc& ctc, tubex::TubeVector& i1, tubex::TubeVector& i2);
      void add(tubex::Ctc& ctc, ibex::Interval& i1, ibex::Interval& i2, tubex::Tube& i3);
      void add(tubex::Ctc& ctc, ibex::Interval& i1, ibex::IntervalVector& i2, tubex::TubeVector& i3);
      void add(tubex::Ctc& ctc, tubex::Slice& i1, tubex::Slice& i2);


    protected:

      AbstractDomain* add_domain(AbstractDomain *ad);
      void add_domain(AbstractDomain *ad, AbstractContractor *ac);
      void add_contractor(AbstractContractor *ac);

      std::vector<AbstractContractor*> m_v_ctc;
      //std::map<AbstractContractor*,double> m_ctc_comput_times;
      std::vector<AbstractDomain*> m_v_domains;

      std::deque<AbstractContractor*> m_deque;
  };
}

#endif