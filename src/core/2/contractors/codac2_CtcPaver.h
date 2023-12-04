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

      static bool compare_paving(const std::weak_ptr<PavingNode<N>>& first, const std::weak_ptr<PavingNode<N>>& second)
      {
        return first.lock()->_x.volume() > second.lock()->_x.volume();
      }

      CtcPaver(ContractorOnBox<N>& ctc, size_t max_leaves)
        : _ctc(ctc), _max_leaves(max_leaves)
      {

      }

      void contract(Paving<N>& x)
      {
        auto l_subpav = x.leaves();
        size_t nb_leaves = l_subpav.size();

        while(!l_subpav.empty())
        {
          l_subpav.sort(compare_paving);

          std::shared_ptr<PavingNode<N>> q = l_subpav.front().lock();
          l_subpav.pop_front();
          assert(q.use_count() > 1);

          _ctc.contract(q->_x);

          if(q->_x.is_empty())
            nb_leaves--;

          else if(nb_leaves < _max_leaves)
          {
            q->bisect();
            nb_leaves++;
            l_subpav.push_back(q->left());
            l_subpav.push_back(q->right());
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