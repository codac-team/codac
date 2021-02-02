/** 
 *  CtcDelay class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Raphael Voges
 *  \copyright  Copyright 2020 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcDelay.h"
#include "codac_Domain.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcDelay::CtcDelay() : DynCtc(true)
  {

  }

  void CtcDelay::contract(vector<Domain*>& v_domains)
  {
    vector<string> v_str_expected_doms(2);
    v_str_expected_doms[0] = "Interval, Tube, Tube";
    v_str_expected_doms[1] = "Interval, TubeVector, TubeVector";

    if(v_domains.size() != 3 || v_domains[0]->type() != Domain::Type::T_INTERVAL)
      throw DomainsTypeException("CtcDelay", v_domains, v_str_expected_doms);

    // Scalar case:
    if(v_domains[1]->type() == Domain::Type::T_TUBE && v_domains[2]->type() == Domain::Type::T_TUBE)
      contract(v_domains[0]->interval(), v_domains[1]->tube(), v_domains[2]->tube());

    // Vector case:
    else if(v_domains[1]->type() == Domain::Type::T_TUBE_VECTOR && v_domains[2]->type() == Domain::Type::T_TUBE_VECTOR)
      contract(v_domains[0]->interval(), v_domains[1]->tube_vector(), v_domains[2]->tube_vector());

    else
      throw DomainsTypeException("CtcDelay", v_domains, v_str_expected_doms);
  }

  void CtcDelay::contract(Interval& a, Tube& x, Tube& y)
  {
    if(a.is_empty() || x.is_empty() || y.is_empty()){
        a.set_empty();
        x.set_empty();
        y.set_empty();
        return;
    }

    // if the time domains do not intersect for the given [a], we cannot contract
    Interval intv_t = x.tdomain() + a;
    if(!intv_t.intersects(y.tdomain())) return;

    // iterate over the first tube x
    Slice *s_x = x.first_slice();
    while(s_x != NULL)
    {
      const Interval t_x = s_x->tdomain();
      Interval intv_t = t_x + a;

      if(intv_t.is_subset(y.tdomain())){
          const Interval s_y = y(intv_t & y.tdomain());
          // if the evaluation of the tube y, which we would invert inside [intv_t],
          // is already completely inside the codomain of s_x, no contraction for [a] can
          // be achieved and we can avoid the inversion to save computation time
          if(s_y.is_interior_subset(s_x->codomain())){
              s_x->set_envelope(s_x->codomain() & s_y);
          } else {
              const Interval t_y = y.invert(s_x->codomain(), intv_t & y.tdomain());
              a &= t_y - t_x;

              if(a.is_empty()){
                  x.set_empty();
                  y.set_empty();
                  return;
              }
              intv_t = t_x + a;
              s_x->set_envelope(s_x->codomain() & y(intv_t & y.tdomain()));
          }
      }

      intv_t = t_x.lb() + a;
      if(intv_t.is_subset(y.tdomain()))
          s_x->set_input_gate(s_x->input_gate() & y(intv_t & y.tdomain()));

      intv_t = t_x.ub() + a;
      if(intv_t.is_subset(y.tdomain()))
          s_x->set_output_gate(s_x->output_gate() & y(intv_t & y.tdomain()));

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
          const Interval s_x = x(intv_t & x.tdomain());
          // if the evaluation of the tube x, which we would invert inside [intv_t],
          // is already completely inside the codomain of s_y, no contraction for [a] can
          // be achieved and we can avoid the inversion to save computation time
          if(s_x.is_interior_subset(s_y->codomain())){
              s_y->set_envelope(s_y->codomain() & s_x);
          } else {
              const Interval t_x = x.invert(s_y->codomain(), intv_t & x.tdomain());
              a &= t_y - t_x;

              if(a.is_empty()){
                  x.set_empty();
                  y.set_empty();
                  return;
              }
              intv_t = t_y - a;
              s_y->set_envelope(s_y->codomain() & x(intv_t & x.tdomain()));
          }
      }

      intv_t = t_y.lb() - a;
      if(intv_t.is_subset(x.tdomain()))
          s_y->set_input_gate(s_y->input_gate() & x(intv_t & x.tdomain()));

      intv_t = t_y.ub() - a;
      if(intv_t.is_subset(x.tdomain()))
          s_y->set_output_gate(s_y->output_gate() & x(intv_t & x.tdomain()));

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
