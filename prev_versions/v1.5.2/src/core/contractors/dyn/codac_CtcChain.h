/** 
 *  \file
 *  CtcChain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCCHAIN_H__
#define __CODAC_CTCCHAIN_H__

#include "codac_CtcLinobs.h"
#include "codac2_Tube.h"

namespace codac
{
  /**
   * \class CtcChain
   */
  class CtcChain : public CtcLinobs
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{chain}\f$
       */
      CtcChain();

      void contract(std::vector<Domain*>& v_domains);
      void contract(Tube& x, Tube& v, const Tube& a, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      //void contract(codac2::Tube<IntervalVector>& x, const codac2::Tube<Interval>& a, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      //void contract(codac2::Tube<Interval>& x, codac2::Tube<Interval>& v, const codac2::Tube<Interval>& a, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);


    protected:

      static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
      static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
      friend class ContractorNetwork;
  };
}

#endif