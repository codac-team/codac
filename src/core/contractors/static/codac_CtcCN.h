/**
 *  \file
 *  CtcCN class
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Julien DAMERS
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCCN_H__
#define __CODAC_CTCCN_H__

#include "codac_Ctc.h"
#include "codac_ContractorNetwork.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class CtcCN
   * \brief static contractor on a contractor network object
   */
  class CtcCN : public Ctc
  {
    public:
      /**
       * \brief Creates the CtcCN contractor
       * \param cn  the contractor network on which this contractor is based on
       * \param box the IntervalVectorVar symbolising the box to contract in the Contractor Network
       */
      CtcCN(ContractorNetwork& cn, IntervalVectorVar& box);

      /**
       * \brief Contracts
       * \param x the box we want to contract. Its size should be equal to m_box size
       */
      void contract(IntervalVector& x);


    private:

      ContractorNetwork& m_cn;
      IntervalVectorVar& m_box_var;
  };
}

#endif //__CODAC_CTCCN_H__