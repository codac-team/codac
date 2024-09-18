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
      : Paving<IntervalVector>(n)
    {
      assert_release(n > 0);
    }

    PavingOut::PavingOut(const IntervalVector& x)
      : Paving<IntervalVector>(x)
    { }

    std::list<PavingOut::ConnectedSubset_> PavingOut::connected_subsets(const PavingOut::NodeValue_& node_value) const
    {
      return Paving<IntervalVector>::connected_subsets(node_value);
    }

    std::list<PavingOut::ConnectedSubset_> PavingOut::connected_subsets(const IntervalVector& x0, const PavingOut::NodeValue_& node_value) const
    {
      return Paving<IntervalVector>::connected_subsets(x0, node_value);
    }

    const PavingOut::NodeValue_ PavingOut::outer_approx =
      [](PavingOut::Node_ n)
    {
      list<IntervalVector> l;
      if(n->is_leaf())
        l.push_back(get<0>(n->boxes()));
      return l;
    };

    const PavingOut::NodeValue_ PavingOut::outer_complem_approx =
      [](PavingOut::Node_ n)
    {
      return n->hull().diff(get<0>(n->boxes()));
    };


  // PavingInOut class

    PavingInOut::PavingInOut(size_t n)
      : Paving<IntervalVector,IntervalVector>(n)
    {
      assert_release(n > 0);
    }

    PavingInOut::PavingInOut(const IntervalVector& x)
      : Paving<IntervalVector,IntervalVector>(x)
    { }

    std::list<PavingInOut::ConnectedSubset_> PavingInOut::connected_subsets(const PavingInOut::NodeValue_& node_value) const
    {
      return Paving<IntervalVector,IntervalVector>::connected_subsets(node_value);
    }

    std::list<PavingInOut::ConnectedSubset_> PavingInOut::connected_subsets(const IntervalVector& x0, const PavingInOut::NodeValue_& node_value) const
    {
      return Paving<IntervalVector,IntervalVector>::connected_subsets(x0, node_value);
    }

    const PavingInOut::NodeValue_ PavingInOut::outer_approx =
      [](PavingInOut::Node_ n)
    {
      auto l = n->hull().diff(get<1>(n->boxes()));
      if(n->is_leaf())
        l.push_back(get<0>(n->boxes()) & get<1>(n->boxes()));
      return l;
    };

    const PavingInOut::NodeValue_ PavingInOut::outer_complem_approx =
      [](PavingInOut::Node_ n)
    {
      return n->hull().diff(get<0>(n->boxes()));
    };

    const PavingInOut::NodeValue_ PavingInOut::inner_approx =
      [](PavingInOut::Node_ n)
    {
      return n->hull().diff(get<1>(n->boxes()));
    };

    const PavingInOut::NodeValue_ PavingInOut::bound_approx =
      [](PavingInOut::Node_ n)
    {
      list<IntervalVector> l;
      if(n->is_leaf())
        l.push_back(get<0>(n->boxes()) & get<1>(n->boxes()));
      return l;
    };

    const PavingInOut::NodeValue_ PavingInOut::all_approx =
      [](PavingInOut::Node_ n)
    {
      auto l = n->hull().diff(get<1>(n->boxes()));
      l.splice(l.end(), n->hull().diff(get<0>(n->boxes())));
      if(n->is_leaf())
        l.push_back(get<0>(n->boxes()) & get<1>(n->boxes()));
      return l;
    };
}