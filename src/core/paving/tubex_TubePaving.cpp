/** 
 *  TubePaving class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include "tubex_TubePaving.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TubePaving::TubePaving(const IntervalVector& init_box) : Paving(init_box, SetValue::MAYBE)
  {

  }

  void TubePaving::compute(float precision, const TubeVector& x)
  {
    assert(precision > 0.);
    assert(x.size() == size());

    IntervalVector y = box();
    vector<Interval> v_t_inv;
    x.invert(y, v_t_inv);

    bool is_out = v_t_inv.empty();
    bool is_in = false;

    for(size_t i = 0 ; i < v_t_inv.size() && !is_in ; i++)
    {
      const Slice **s = new const Slice*[size()];
      for(int j = 0 ; j < size() ; j++)
        s[j] = x[j].slice(v_t_inv[i].lb());

      while(!is_in && s[0] != NULL && s[0]->tdomain().ub() <= v_t_inv[i].ub())
      {
        bool is_in_i = true;

        for(int j = 0 ; j < size() && is_in_i ; j++)
          is_in_i &= y[j].is_subset(s[j]->codomain());

        is_in |= is_in_i;

        for(int j = 0 ; j < size() ; j++)
          s[j] = s[j]->next_slice();
      }
    }

    if(is_out)
      set_value(SetValue::OUT);

    else if(is_in)
      set_value(SetValue::IN);

    else if(box().max_diam() < precision)
      set_value(SetValue::MAYBE);

    else
    {
      bisect();
      ((TubePaving*)m_first_subpaving)->compute(precision, x);
      ((TubePaving*)m_second_subpaving)->compute(precision, x);
    }
  }
}