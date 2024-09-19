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

      Paving(size_t n)
        : Paving(IntervalVector(n))
      {
        assert_release(n > 0);
      }

      Paving(const IntervalVector& x)
        : _tree(std::make_shared<PavingNode<P>>(*static_cast<P*>(this), x))
      { }

      size_t size() const
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

      std::list<Node_> intersecting_nodes(const IntervalVector& x, const NodeValue_& node_value, bool flat_intersections = false) const
      {
        std::list<Node_> l;

        _tree->visit([&]
          (Node_ n)
          {
            for(const auto& bi : node_value(n))
            {
              auto inter = bi & x;
              if(!inter.is_empty() && (flat_intersections || !inter.is_flat()))
              {
                l.push_back(n);
                break;
              }
            }

            return n->hull().intersects(x);
          });

        return l;
      }

      std::list<Node_> neighbours(Node_ x, const NodeValue_& node_value_x, const NodeValue_& node_value_neighb) const
      {
        std::list<Node_> l;
        auto x_boxes = node_value_x(x);

        for(const auto& xi : x_boxes)
        {
          auto xi_neighbours = intersecting_nodes(xi, node_value_neighb, true);
          l.splice(l.end(), xi_neighbours);
        }

        // Removing duplicates
        l.sort(); l.unique();

        // If the input node x contains only one box, it is removed from
        // the neighbours list. Otherwise, "neighbours" may be contained 
        // in x itself.
        if(x_boxes.size() == 1 && x_boxes.front() == x->hull())
         l.remove(x);

        return l;
      }

      std::list<ConnectedSubset_> connected_subsets(const NodeValue_& node_value) const
      {
        return connected_subsets(IntervalVector(size()), node_value);
      }

      std::list<ConnectedSubset_> connected_subsets(const IntervalVector& x0, const NodeValue_& node_value) const
      {
        std::map<Node_,bool> m_visited_nodes;
        std::list<Node_> l_nodes = intersecting_nodes(x0, node_value);

        for(const auto& n : l_nodes)
          m_visited_nodes.insert(std::make_pair(n, false));

        std::list<ConnectedSubset_> l_subsets;

        while(!l_nodes.empty())
        {
          auto current_node = l_nodes.front();
          l_subsets.push_back(ConnectedSubset_({ current_node }, node_value));
          m_visited_nodes[current_node] = true;
          l_nodes.pop_front();

          std::list<Node_> l_neighbouring_nodes_to_visit { current_node };

          do
          {
            current_node = l_neighbouring_nodes_to_visit.front();
            l_neighbouring_nodes_to_visit.pop_front();

            for(const auto& ni : neighbours(current_node, node_value, node_value))
            {
              if(ni->hull().is_flat())
              {
                // At this point, some nodes may be flat due to bisections/contractions.
                // They are removed from the resulting list representing the connected subset.
                l_nodes.remove(ni);
              }

              else if(!m_visited_nodes[ni])
              {
                l_nodes.remove(ni);
                l_neighbouring_nodes_to_visit.push_back(ni);
                l_subsets.back().push_back(ni);
                m_visited_nodes[ni] = true;
              }
            }

          } while(!l_neighbouring_nodes_to_visit.empty());
        }

        assert([&]() -> bool { for(const auto& ni : m_visited_nodes) { if(!ni.second) return false; } return true; } ()
          && "all the nodes should have been visited");
        assert([&]() -> bool { size_t s = 0; for(const auto& si : l_subsets) s += si.size(); return s == m_visited_nodes.size(); } ()
          && "the total number of nodes should match the sum of number of nodes of each subset");

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

      PavingOut(size_t n);
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

      PavingInOut(size_t n);
      PavingInOut(const IntervalVector& x);

      std::list<PavingInOut::ConnectedSubset_> connected_subsets(const PavingInOut::NodeValue_& node_value = PavingInOut::outer) const;
      std::list<PavingInOut::ConnectedSubset_> connected_subsets(const IntervalVector& x0, const PavingInOut::NodeValue_& node_value = PavingInOut::outer) const;

      static const NodeValue_ outer, outer_complem, inner, bound, all;
  };
}