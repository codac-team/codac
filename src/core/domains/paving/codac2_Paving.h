/** 
 *  \file codac2_Paving.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>
#include "codac2_PavingNode.h"

namespace codac2
{
  template<typename P>
  class PavingNode;

  template<typename P>
  class Subpaving;

  template<typename P,typename... X>
    requires (std::is_same_v<X,IntervalVector> && ...)
  class Paving : public Domain
  {
    public:

      using Node_ = std::shared_ptr<const PavingNode<P>>;
      using NodeTuple_ = std::tuple<X...>;
      using NodeValue_ = std::function<std::list<IntervalVector>(Node_)>;
      using ConnectedSubset_ = Subpaving<P>;

      Paving(Index n)
        : Paving(IntervalVector(n))
      {
        assert_release(n > 0);
      }

      Paving(const IntervalVector& x)
        : _tree(std::make_shared<PavingNode<P>>(*static_cast<P*>(this), x))
      { }

      Index size() const
      {
        return std::get<0>(_tree->boxes()).size();
      }

      std::shared_ptr<const PavingNode<P>> tree() const
      {
        return _tree;
      }

      std::shared_ptr<PavingNode<P>> tree()
      {
        return std::const_pointer_cast<PavingNode<P>>(const_cast<const Paving<P,X...>*>(this)->tree());
      }

      std::list<IntervalVector> intersecting_boxes(const IntervalVector& x, const NodeValue_& node_value) const
      {
        std::list<IntervalVector> l;

        this->tree()->visit([&]
          (Node_ n)
          {
            for(const auto& bi : node_value(n))
            {
              auto inter = bi & x;
              if(bi.intersects(x))
                l.push_back(bi);
            }

            return n->hull().intersects(x);
          });

        return l;
      }

      std::list<ConnectedSubset_> connected_subsets(const NodeValue_& node_value) const
      {
        return connected_subsets(IntervalVector(size()), node_value);
      }

      std::list<ConnectedSubset_> connected_subsets(const IntervalVector& x0, const NodeValue_& node_value) const
      {
        std::list<IntervalVector> l_boxes = intersecting_boxes(x0, node_value);
        [[maybe_unused]] Index nb_boxes = l_boxes.size();
        std::list<ConnectedSubset_> l_subsets;

        while(!l_boxes.empty())
        {
          auto current_box = l_boxes.front();
          l_subsets.push_back({ current_box });
          l_boxes.pop_front();

          std::list<IntervalVector> l_neighbouring_boxes_to_visit { current_box };

          do
          {
            current_box = l_neighbouring_boxes_to_visit.front();
            l_neighbouring_boxes_to_visit.pop_front();

            for(const auto& ni : intersecting_boxes(current_box, node_value))
            {
              if(std::find(l_boxes.begin(), l_boxes.end(), ni) != l_boxes.end())
              {
                l_boxes.remove(ni);
                l_neighbouring_boxes_to_visit.push_back(ni);
                l_subsets.back().push_back(ni);
              }
            }

          } while(!l_neighbouring_boxes_to_visit.empty());
        }

        assert(l_boxes.empty() && "all the nodes should have been visited");
        assert([&]() -> bool { Index s = 0; for(const auto& si : l_subsets) s += si.size(); return s == nb_boxes; } ()
          && "the total number of boxes should match the sum of number of boxes of each subset");

        return l_subsets;
      }

    protected:

      friend class PavingNode<P>;

      static NodeTuple_ init_tuple(const IntervalVector& x)
      {
        return std::make_tuple(((X)x)...);
      }

      std::shared_ptr<PavingNode<P>> _tree; // must be a shared_ptr to allow enable_shared_from_this
  };


  class PavingOut;
  using PavingOut_Node = PavingNode<PavingOut>;

  class PavingOut : public Paving<PavingOut,IntervalVector>
  {
    public:

      PavingOut(Index n);
      PavingOut(const IntervalVector& x);

      std::list<PavingOut::ConnectedSubset_> connected_subsets(const PavingOut::NodeValue_& node_value = PavingOut::outer) const;
      std::list<PavingOut::ConnectedSubset_> connected_subsets(const IntervalVector& x0, const PavingOut::NodeValue_& node_value = PavingOut::outer) const;

      static const NodeValue_ outer, outer_complem;
  };


  class PavingInOut;
  using PavingInOut_Node = PavingNode<PavingInOut>;

  class PavingInOut : public Paving<PavingInOut,IntervalVector,IntervalVector>
  {
    public:

      PavingInOut(Index n);
      PavingInOut(const IntervalVector& x);

      std::list<PavingInOut::ConnectedSubset_> connected_subsets(const PavingInOut::NodeValue_& node_value = PavingInOut::outer) const;
      std::list<PavingInOut::ConnectedSubset_> connected_subsets(const IntervalVector& x0, const PavingInOut::NodeValue_& node_value = PavingInOut::outer) const;

      static const NodeValue_ outer, outer_complem, inner, bound, all;
  };
}