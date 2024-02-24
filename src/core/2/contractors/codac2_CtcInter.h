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

#ifndef __CODAC2_CTCINTER_H__
#define __CODAC2_CTCINTER_H__

#include "codac2_Ctc.h"
#include "codac2_Paving.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  template<int N=Dynamic>
  class CtcInter : public Ctc
  {
    public:

      CtcInter(Paving_<N>& x)
        : _x(x)
      {

      }

      void contract(IntervalVector_<N>& y)
      {
        _x.contract(y);
      }

      template<typename... T>
      std::list<std::shared_ptr<PropagationSpanBase>> get_default_spans(T&... x)
      {
        std::list<std::shared_ptr<PropagationSpanBase>> views;
        (views.push_back(std::make_shared<PropagationSpan<typename std::remove_reference<T>::type>>(&x)), ...);
        views.push_back(std::make_shared<PropagationSpan<Paving_<N>>>(&_x));
        views.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); });
        return views;
      }

      make_available_to_cn___call_operator()
      make_available_to_cn___contract_and_propag()

    protected:

      Paving_<N>& _x; // todo: const
  };
}

#endif