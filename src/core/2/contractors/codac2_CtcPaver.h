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

#ifndef __CODAC2_CTCONPAVER_H__
#define __CODAC2_CTCONPAVER_H__

#include "codac2_Contractor.h"

namespace codac2
{
  template<int N>
  class CtcPaver : public Contractor
  {
    public:

      static bool compare_paving(Paving<N>* first, Paving<N>* second)
      {
        return first->box().volume() > second->box().volume();
      }

      CtcPaver(ContractorOnBox<N>& ctc, size_t max_leaves)
        : _ctc(ctc), _max_leaves(max_leaves)
      {

      }

      void contract(Paving<N>& x)
      {
        list<Paving<N>*> l_subpav = x.leaves_list();
        size_t nb_leaves = l_subpav.size();

        while(!l_subpav.empty())
        {
          l_subpav.sort(compare_paving);

          Paving<N>* q = l_subpav.front();
          l_subpav.pop_front();

          _ctc.contract(q->_x);

          if(q->_x.is_empty())
            nb_leaves--;

          else if(nb_leaves < _max_leaves)
          {
            q->bisect();
            nb_leaves++;
            l_subpav.push_back(q->_left.get());
            l_subpav.push_back(q->_right.get());
          }
        }
      }

      make_available_in_cn__templated(CtcPaver<N>)

    protected:

      ContractorOnBox<N>& _ctc;
      size_t _max_leaves;
  };
}

#endif