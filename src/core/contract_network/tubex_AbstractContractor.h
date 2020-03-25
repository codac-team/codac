/** 
 *  \file
 *  AbstractContractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_ABSTRACT_CONTRACTOR_H__
#define __TUBEX_ABSTRACT_CONTRACTOR_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_AbstractDomain.h"
#include "tubex_Ctc.h"

namespace tubex
{
  enum class ContractorType { NONE, IBEX, TUBEX };

  class AbstractContractor
  {
    public:

      AbstractContractor(const AbstractContractor& ac);
      explicit AbstractContractor();
      explicit AbstractContractor(ibex::Ctc& ctc);
      explicit AbstractContractor(tubex::Ctc& ctc);
      ~AbstractContractor();

      ContractorType type() const;
      bool operator==(const AbstractContractor& x) const;

      void contract();

      std::vector<AbstractDomain*> m_domains;


    //protected:

      const ContractorType m_type;
      double m_active = true;

      ibex::Ctc& m_ibex_ctc;
      tubex::Ctc& m_tubex_ctc;
  };
}

#endif