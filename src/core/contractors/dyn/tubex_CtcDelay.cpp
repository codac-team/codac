/** 
 *  CtcDelay class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Raphael Voges
 *  \copyright  Copyright 2020 Tubex Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcDelay.h"
#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay() : DynCtc(true)
  {

  }

  void CtcDelay::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 3);
    assert(v_domains[0]->type() == Domain::Type::T_INTERVAL);

    // Scalar case:
    if(v_domains[1]->type() == Domain::Type::T_TUBE && v_domains[2]->type() == Domain::Type::T_TUBE)
      contract(v_domains[0]->interval(), v_domains[1]->tube(), v_domains[2]->tube());

    // Vector case:
    else if(v_domains[1]->type() == Domain::Type::T_TUBE_VECTOR && v_domains[2]->type() == Domain::Type::T_TUBE_VECTOR)
      contract(v_domains[0]->interval(), v_domains[1]->tube_vector(), v_domains[2]->tube_vector());

    else
      assert(false && "vector of domains not consistent with the contractor definition");
  }

  void CtcDelay::contract(Interval& a, Tube& x, Tube& y)
  {
    if(a.is_empty() || x.is_empty() || y.is_empty()){
        a.set_empty();
        x.set_empty();
        y.set_empty();
        return;
    }

    // iterate over the first tube x
    Slice *s_x = x.first_slice();
    while(s_x != NULL)
    {
      const Interval t_x = s_x->tdomain();
      Interval intv_t = t_x + a;
      if(intv_t.is_subset(y.tdomain())){
          const Interval t_y = y.invert(s_x->codomain(),intv_t);
          a &= t_y - t_x;

          if(a.is_empty()){
              x.set_empty();
              y.set_empty();
              return;
          }

          intv_t = t_x + a;
          s_x->set_envelope(s_x->codomain() & y(intv_t));
      }

      intv_t = t_x.lb() + a;
      if(intv_t.is_subset(y.tdomain()))
          s_x->set_input_gate(s_x->input_gate() & y(intv_t));

      intv_t = t_x.ub() + a;
      if(intv_t.is_subset(y.tdomain()))
          s_x->set_output_gate(s_x->output_gate() & y(intv_t));

      if(s_x->is_empty()){
          a.set_empty();
          x.set_empty();
          y.set_empty();
          return;
      }

      s_x = s_x->next_slice();
    }

    // iterate over the second tube y
    Slice *s_y = y.first_slice();
    while(s_y != NULL)
    {
      const Interval t_y = s_y->tdomain();
      Interval intv_t = t_y - a;
      if(intv_t.is_subset(x.tdomain())){
          const Interval t_x = x.invert(s_y->codomain(),intv_t);
          a &= t_y - t_x;

          if(a.is_empty()){
              x.set_empty();
              y.set_empty();
              return;
          }

          intv_t = t_y - a;
          s_y->set_envelope(s_y->codomain() & x(intv_t));
      }

      intv_t = t_y.lb() - a;
      if(intv_t.is_subset(x.tdomain()))
          s_y->set_input_gate(s_y->input_gate() & x(intv_t));

      intv_t = t_y.ub() - a;
      if(intv_t.is_subset(x.tdomain()))
          s_y->set_output_gate(s_y->output_gate() & x(intv_t));

      if(s_y->is_empty()){
          a.set_empty();
          x.set_empty();
          y.set_empty();
          return;
      }

      s_y = s_y->next_slice();
    }

    if(a.is_empty() || x.is_empty() || y.is_empty()){
        a.set_empty();
        x.set_empty();
        y.set_empty();
    }
  }

  void CtcDelay::contract(Interval& a, TubeVector& x, TubeVector& y)
  {
    assert(x.size() == y.size());

    if(a.is_empty() || x.is_empty() || y.is_empty()){
        a.set_empty();
        x.set_empty();
        y.set_empty();
        return;
    }

    for(int i = 0 ; i < x.size() ; i++)
      contract(a, x[i], y[i]);

    if(a.is_empty() || x.is_empty() || y.is_empty()){
        a.set_empty();
        x.set_empty();
        y.set_empty();
    }
  }
}
