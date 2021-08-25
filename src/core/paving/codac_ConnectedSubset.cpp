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
    for(size_t i = 0 ; i < m_v_subset_items.size() ; i++)
      m_box |= m_v_subset_items[i]->box();
  }

  ConnectedSubset::~ConnectedSubset()
  {

  }

  const IntervalVector ConnectedSubset::box() const
  {
    return m_box;
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
    vector<IntervalVector> v_boxes;
    for(size_t i = 0 ; i < m_v_subset_items.size() ; i++)
      v_boxes.push_back(m_v_subset_items[i]->box());
    return v_boxes;
  }

  vector<IntervalVector> ConnectedSubset::get_boundary(SetValue value_boundary, SetValue value_out) const // items of type k-1
  {
    assert(value_out != value_boundary);
    vector<IntervalVector> v_boundaries;

    for(size_t i = 0 ; i < m_v_subset_items.size() ; i++)
    {
      if(!(m_v_subset_items[i]->value() & value_boundary))
        continue;

      vector<const Paving*> v_neighbours;
      m_v_subset_items[i]->get_neighbours(v_neighbours, value_out, false);

      for(size_t j = 0 ; j < v_neighbours.size() ; j++)
      {
        IntervalVector inter = m_v_subset_items[i]->box() & v_neighbours[j]->box();

        if(!inter[0].is_degenerated() || !inter[1].is_degenerated())
          v_boundaries.push_back(inter);
      }
    }

    return v_boundaries;
  }
  
  const vector<IntervalVector> ConnectedSubset::get_boxed_hulls(const vector<ConnectedSubset> v_subsets)
  {
    vector<IntervalVector> v_boxes(v_subsets.size());
    for(size_t i = 0 ; i < v_subsets.size() ; i++)
      v_boxes[i] = v_subsets[i].box();
    return v_boxes;
  }
}