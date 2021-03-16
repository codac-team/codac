/** 
 *  TubePaving class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include "codac_TubePaving.h"

using namespace std;
using namespace ibex;

namespace codac
{
  TubePaving::TubePaving(const IntervalVector& init_box, SetValue value) : Paving(init_box, value)
  {

  }

  void TubePaving::compute(float precision, const TubeVector& x)
  {
    assert(precision > 0.);
    assert(x.size() == size());

    IntervalVector y = box();
    vector<Interval> v_t_inv;
    x.invert(y, v_t_inv, x.tdomain());

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
      set_value(SetValue::UNKNOWN);

    else
    {
      bisect();
      ((TubePaving*)m_first_subpaving)->compute(precision, x);
      ((TubePaving*)m_second_subpaving)->compute(precision, x);
    }
  }

  void TubePaving::compute(float precision, const Tube &x)
  {
      assert(precision > 0.);
      IntervalVector y = box();
      bool is_in = false;


      if ((x.tdomain()&y[0]).is_empty()||(x.codomain()&y[1]).is_empty() || (y[1]&(x(x.tdomain()&y[0]))).is_empty())
      {
          set_value(SetValue::OUT);
          return;
      }

      else if (y[0].is_subset(x.tdomain()) && y[1].is_subset(x(y[0])))
      {
          is_in = true;
          const Slice* s = x.slice(y[0].lb());
          while(is_in  && s != NULL && s->tdomain().lb() <= y[0].ub())
          {
              if (!s->codomain().is_superset(y[1]))
              {
                  is_in = false;
              }
              s = s->next_slice();
          }

      }
      if (is_in)
      {
          set_value(SetValue::IN);
      }
      else if(box().max_diam() < precision)
      {
          set_value(SetValue::UNKNOWN);

      }
      else
      {
          set_value(SetValue::UNKNOWN);
          bisect();
          ((TubePaving*)m_first_subpaving)->compute(precision, x);
          ((TubePaving*)m_second_subpaving)->compute(precision, x);
      }
  }
      /*
          for(size_t i = 0 ; i < v_t_inv.size(); i++)
          {
              bool is_in = true;
              if(!(y[0].is_subset(v_t_inv[i]))){is_in=false;}
              const Slice *s = x.slice(v_t_inv[i].lb());
              while (is_in && s != NULL && s->tdomain().ub() <= v_t_inv[i].ub())
              {
                    is_in &=(y[1].is_subset(s->codomain()));
                    s=s->next_slice();
              }
              if(is_in)
              {
                  set_value(SetValue::IN);
                  return;
              }
          }
          */



}