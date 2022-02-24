/** 
 *  ConnectedSubset class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include "codac_ConnectedSubset.h"
#include "codac_Paving.h"

using namespace std;
using namespace ibex;

namespace codac
{
  ConnectedSubset::ConnectedSubset(const vector<const Paving*>& v_subset_items)
    : Set(IntervalVector(2, Interval::EMPTY_SET), SetValue::IN | SetValue::UNKNOWN),
      m_v_subset_items(v_subset_items)
  {
    m_box = IntervalVector(2, Interval::EMPTY_SET);
    for(const auto& subset_item : m_v_subset_items)
      m_box |= subset_item->box();
  }

  ConnectedSubset::~ConnectedSubset()
  {

  }

  bool ConnectedSubset::is_strictly_included_in_paving() const
  {
    return box().is_strict_interior_subset(get_paving()->box());
  }

  const Paving* ConnectedSubset::get_paving() const
  {
    assert(!m_v_subset_items.empty() && "no items in ConnectedSubset (empty items vector)");
    return m_v_subset_items[0]->get_root();
  }

  const vector<const Paving*>& ConnectedSubset::get_items() const // items of type k
  {
    return m_v_subset_items;
  }

  vector<IntervalVector> ConnectedSubset::get_boxes() const // items of type k
  {
    vector<IntervalVector> v_boxes(m_v_subset_items.size());
    size_t i = 0;
    for(const auto& subset_item : m_v_subset_items)
    {
      v_boxes[i] = subset_item->box();
      i++;
    }
    return v_boxes;
  }

  vector<IntervalVector> ConnectedSubset::get_boundary(SetValue value_boundary, SetValue value_out) const // items of type k-1
  {
    assert(value_out != value_boundary);
    vector<IntervalVector> v_boundaries;

    for(const auto& subset_item : m_v_subset_items)
    {
      if(!(subset_item->value() & value_boundary))
        continue;

      vector<const Paving*> v_neighbours;
      subset_item->get_neighbours(v_neighbours, value_out, false);

      for(const auto& neighb : v_neighbours)
      {
        IntervalVector inter = subset_item->box() & neighb->box();

        if(!inter[0].is_degenerated() || !inter[1].is_degenerated())
          v_boundaries.push_back(inter);
      }
    }

    return v_boundaries;
  }
  
  const vector<IntervalVector> ConnectedSubset::get_boxed_hulls(const vector<ConnectedSubset> v_subsets)
  {
    vector<IntervalVector> v_boxes(v_subsets.size());
    size_t i = 0;
    for(const auto& subset : v_subsets)
    {
      v_boxes[i] = subset.box();
      i++;
    }
    return v_boxes;
  }
}