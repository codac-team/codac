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

#ifndef __CODAC2_CTCCN__
#define __CODAC2_CTCCN__

#include <list>
#include <memory>
#include "codac2_Ctc.h"
#include "codac2_ContractorNetwork.h"
//#include "codac2_PropagationSpan.h"

namespace codac2
{
  //class ContractorNetwork;

  class CtcCN : public Ctc
  {
    public:

      CtcCN(ContractorNetwork& cn, Var<IntervalVector>& var);
      void contract(IntervalVector& x);

      /*CtcCN(ContractorNetwork& cn, Var<IntervalVector>& var)
        : _cn(cn), _ref_var(&var)
      {

      }

      void contract(IntervalVector& x)
      {
        _cn.reset_variables();
        *_ref_var = x;
        _cn.trigger_all_contractors();
        _cn.contract(false,false);
        x = *_ref_var;
      }*/

      template<typename... T>
      std::list<std::shared_ptr<PropagationSpanBase>> get_default_spans(T&... x)
      {
        std::list<std::shared_ptr<PropagationSpanBase>> views;
        (views.push_back(std::make_shared<PropagationSpan<typename std::remove_reference<T>::type>>(&x)), ...);
        _cn.get_views_from_cn(views);
        views.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); });
        return views;
      }

      make_available_to_cn___call_operator()
      make_available_to_cn___contract_and_propag()

    protected:

      ContractorNetwork& _cn;
      Var<IntervalVector>* _ref_var;
  };
}

#endif