/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCCN_H__
#define __CODAC2_CTCCN_H__

#include "codac2_Contractor.h"
#include "codac2_ContractorNetwork.h"

namespace codac2
{
  template<int N>
  class CtcCN : public ContractorOnBox<N>
  {
    public:

      CtcCN(ContractorNetwork& cn, Var<IntervalVector_<N>>& var)
        : _cn(cn), _ref_var(&var)
      {

      }

      void contract(IntervalVector_<N>& x)
      {
        _cn.reset_all_vars();
        _cn.reset_var(_ref_var,x);
        _cn.trigger_all_contractors();
        _cn.contract(false);
        x = *_ref_var;
      }

      make_available_in_cn__templated(CtcCN<N>)

    protected:

      ContractorNetwork& _cn;
      const Var<IntervalVector_<N>>* _ref_var;
  };
}

#endif