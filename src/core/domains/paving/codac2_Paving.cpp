/** 
 *  codac2_Paving.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Paving.h"
#include "codac2_Subpaving.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  // PavingOut class

    PavingOut::PavingOut(size_t n)
      : Paving<PavingOut,IntervalVector>(n)
    {
      assert_release(n > 0);
    }

    PavingOut::PavingOut(const IntervalVector& x)
      : Paving<PavingOut,IntervalVector>(x)
    { }

    std::list<PavingOut::ConnectedSubset_> PavingOut::connected_subsets(const PavingOut::NodeValue_& node_value) const
    {
      return Paving<PavingOut,IntervalVector>::connected_subsets(node_value);
    }

    std::list<PavingOut::ConnectedSubset_> PavingOut::connected_subsets(const IntervalVector& x0, const PavingOut::NodeValue_& node_value) const
    {
      return Paving<PavingOut,IntervalVector>::connected_subsets(x0, node_value);
    }

    const PavingOut::NodeValue_ PavingOut::outer =
      [](PavingOut::Node_ n)
    {
      list<IntervalVector> l;
      if(n->is_leaf())
        l.push_back(get<0>(n->boxes()));
      return l;
    };

    const PavingOut::NodeValue_ PavingOut::outer_complem =
      [](PavingOut::Node_ n)
    {
      if(n->top())
      {
        if(!n->top()->top())
        {
          if(n->top()->left() == n)
            return get<0>(n->top()->boxes()).diff(get<0>(n->boxes()));

          else
            return list<IntervalVector>();
        }

        else
        {
          auto top_subboxes = get<0>(n->top()->boxes()).bisect_largest();
          if(n->top()->left() == n)
            return top_subboxes.first.diff(get<0>(n->boxes()));
          else
            return top_subboxes.second.diff(get<0>(n->boxes()));
        }
      }

      else
        return list<IntervalVector>();
    };


  // PavingInOut class

    PavingInOut::PavingInOut(size_t n)
      : Paving<PavingInOut,IntervalVector,IntervalVector>(n)
    {
      assert_release(n > 0);
    }

    PavingInOut::PavingInOut(const IntervalVector& x)
      : Paving<PavingInOut,IntervalVector,IntervalVector>(x)
    { }

    std::list<PavingInOut::ConnectedSubset_> PavingInOut::connected_subsets(const PavingInOut::NodeValue_& node_value) const
    {
      return Paving<PavingInOut,IntervalVector,IntervalVector>::connected_subsets(node_value);
    }

    std::list<PavingInOut::ConnectedSubset_> PavingInOut::connected_subsets(const IntervalVector& x0, const PavingInOut::NodeValue_& node_value) const
    {
      return Paving<PavingInOut,IntervalVector,IntervalVector>::connected_subsets(x0, node_value);
    }

    const PavingInOut::NodeValue_ PavingInOut::outer =
      [](PavingInOut::Node_ n)
    {
      auto l = n->hull().diff(get<1>(n->boxes()));
      if(n->is_leaf())
        l.push_back(n->unknown());
      return l;
    };

    const PavingInOut::NodeValue_ PavingInOut::outer_complem =
      [](PavingInOut::Node_ n)
    {
      return n->hull().diff(get<0>(n->boxes()));
    };

    const PavingInOut::NodeValue_ PavingInOut::inner =
      [](PavingInOut::Node_ n)
    {
      return n->hull().diff(get<1>(n->boxes()));
    };

    const PavingInOut::NodeValue_ PavingInOut::bound =
      [](PavingInOut::Node_ n)
    {
      list<IntervalVector> l;
      if(n->is_leaf())
        l.push_back(n->unknown());
      return l;
    };

    const PavingInOut::NodeValue_ PavingInOut::all =
      [](PavingInOut::Node_ n)
    {
      auto l = n->hull().diff(get<1>(n->boxes()));
      l.splice(l.end(), n->hull().diff(get<0>(n->boxes())));
      if(n->is_leaf())
        l.push_back(n->unknown());
      return l;
    };
}