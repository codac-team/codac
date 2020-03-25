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

#include <functional>
#include "ibex_Ctc.h"
#include "tubex_Ctc.h"
#include "tubex_AbstractDomain.h"

namespace tubex
{
  enum class ContractorType { COMPONENT, IBEX, TUBEX };

  class AbstractContractor
  {
    public:

      AbstractContractor(const AbstractContractor& ac);
      explicit AbstractContractor();
      explicit AbstractContractor(ibex::Ctc& ctc);
      explicit AbstractContractor(tubex::Ctc& ctc);
      ~AbstractContractor();

      ContractorType type() const;

      bool is_active() const;
      void set_active(bool active);

      std::vector<AbstractDomain*>& domains();

      bool operator==(const AbstractContractor& x) const;

      void contract();


    protected:

      const ContractorType m_type;
      double m_active = true;

      union
      {
        std::reference_wrapper<ibex::Ctc> m_ibex_ctc;
        std::reference_wrapper<tubex::Ctc> m_tubex_ctc;
      };

      std::vector<AbstractDomain*> m_domains;
  };
}

#endif