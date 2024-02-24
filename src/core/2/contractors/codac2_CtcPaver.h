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

#ifndef __CODAC2_CTCPAVER_H__
#define __CODAC2_CTCPAVER_H__

#include "codac2_Ctc.h"
#include "codac2_Paving.h"

namespace codac2
{
  template<typename C>
  class CtcPaver : public Ctc
  {
    public:

      //template<int N=Dynamic>
      static bool compare_paving(const std::weak_ptr<PavingNode_<>>& first, const std::weak_ptr<PavingNode_<>>& second)
      {
        return first.lock()->box().volume() > second.lock()->box().volume();
      }

      CtcPaver(C& ctc, size_t *max_leaves)
        : _ctc(ctc), _max_leaves(max_leaves)
      {

      }

      template<int N=Dynamic>
      void contract(Paving_<N>& x)
      {
        std::list<std::weak_ptr<PavingNode_<N>>>& l_subpav = x.leaves();

        auto it_leaves = l_subpav.begin();
        while(it_leaves != l_subpav.end())
        {
          auto shared_leaf = it_leaves->lock();
          _ctc.contract(shared_leaf->box());

          if(shared_leaf->box().is_empty())
            l_subpav.erase(it_leaves++);

          else
            ++it_leaves;
        }

        l_subpav.sort(compare_paving);

        while(l_subpav.size() < *_max_leaves)
        {
          std::shared_ptr<PavingNode_<N>> biggest_leaf = l_subpav.front().lock();
          l_subpav.pop_front();

          if(!biggest_leaf->box().is_bisectable())
            l_subpav.push_back(biggest_leaf);

          else
          {
            biggest_leaf->bisect();
            _ctc.contract(biggest_leaf->left()->box());
            l_subpav.push_back(biggest_leaf->left());
            _ctc.contract(biggest_leaf->right()->box());
            l_subpav.push_back(biggest_leaf->right());
          }
        }
      }

      /*template<int N=Dynamic>
      void contract(Paving_<N>& x)
      {
        auto l_subpav = x.leaves();
        size_t nb_leaves = l_subpav.size();

        while(!l_subpav.empty())
        {
          l_subpav.sort(compare_paving);

          std::shared_ptr<PavingNode_<N>> q = l_subpav.front().lock();
          l_subpav.pop_front();
          assert(q.use_count() > 1);

          _ctc.contract(q->box());

          if(q->box().is_empty())
            nb_leaves--;

          else if(nb_leaves < *_max_leaves && q->box().is_bisectable())
          {
            q->bisect();
            nb_leaves++;
            l_subpav.push_back(q->left());
            l_subpav.push_back(q->right());
          }
        }
      }*/

      template<typename... T>
      std::list<std::shared_ptr<PropagationSpanBase>> get_default_spans(T&... x)
      {
        std::list<std::shared_ptr<PropagationSpanBase>> views;
        (views.push_back(std::make_shared<PropagationSpan<typename std::remove_reference<T>::type>>(&x)), ...);
        for(auto& ctc : _ctc.get_default_spans(x...))
          views.push_back(ctc);
        views.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); });
        return views;
      }

      std::list<std::shared_ptr<PropagationSpanBase>> contract_and_propag(DomainNode<Paving_<>&>& x)
      {
        contract(x.operator Paving&());
        std::list<std::shared_ptr<PropagationSpanBase>> output_views;
        output_views.push_back(x.update_and_propag());
        /* keep this? */ output_views.remove_if([](const std::shared_ptr<PropagationSpanBase>& v) { return !v || v->cannot_propagate(); });
        return output_views;
      }

      make_available_to_cn___call_operator()

    protected:

      C& _ctc;
      size_t *_max_leaves;
  };
}

#endif