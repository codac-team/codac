/** 
 *  CtcDelay class
 * ----------------------------------------------------------------------------
 *  \date       2017
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "tubex_CtcDelay.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay() : Ctc()
  {

  }

  void CtcDelay::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 3);

    // todo

    /*// Scalar case:
    if(v_domains[1]->type() == TUBE && v_domains[2]->type() == TUBE)
      contract(v_domains[0]->m_i, v_domains[1]->m_t, v_domains[2]->m_t);

    // Vector case:
    else if(v_domains[1]->type() == TUBE_VECTOR && v_domains[2]->type() == TUBE_VECTOR)
      contract(v_domains[0]->m_i, v_domains[1]->m_tv, v_domains[2]->m_tv);

    else
      assert(false && "unhandled case");*/
  }

  void CtcDelay::contract(const Interval& a, const Tube& x, Tube& y)
  {
    assert(x.domain() == y.domain());
    assert(Tube::same_slicing(x, y));

    const Slice *s_x = x.first_slice();
    Slice *s_y = y.first_slice();

    while(s_x != NULL)
    {
      Interval intv_t = s_y->domain().lb() - a;
      if(intv_t.is_subset(y.domain()))
        s_y->set_input_gate(s_y->input_gate() & x(intv_t));

      intv_t = s_y->domain() - a;
      if(intv_t.is_subset(y.domain()))
        s_y->set_envelope(s_y->codomain() & x(intv_t));

      intv_t = s_y->domain().ub() - a;
      if(intv_t.is_subset(y.domain()))
        s_y->set_output_gate(s_y->output_gate() & x(intv_t));

      s_x = s_x->next_slice();
      s_y = s_y->next_slice();
    }
  }

  void CtcDelay::contract(const Interval& a, const TubeVector& x, TubeVector& y)
  {
    assert(x.size() == y.size());
    assert(x.domain() == y.domain());
    assert(TubeVector::same_slicing(x, y));

    for(int i = 0 ; i < x.size() ; i++)
      contract(a, x[i], y[i]);
  }
}