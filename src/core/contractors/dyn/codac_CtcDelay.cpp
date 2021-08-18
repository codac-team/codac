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

    // Build Tree for Tube y since we will evaluate/invert it in the following
    y.enable_synthesis();

    // iterate over the first tube x
    Slice *s_x = x.first_slice();
    while(s_x != NULL)
    {
      const Interval t_x = s_x->tdomain();
      Interval intv_t = t_x + a;

      Interval s_y = y(intv_t);

      // if the evaluation of the tube y, which we would invert inside [intv_t],
      // is already completely inside the codomain of s_x, no contraction for [a] can
      // be achieved and we can avoid the inversion to save computation time

      // TODO: this only makes sense if we assume that
      // (1) most slices won't help to contract the time delay [a]
      // (2) the codomain of s_x is often (more often than (1)) large such that we can
      // omit the inversion (this is especially the case if we want to propagate information
      // from y to x and x is initially unknown or very inaccurate)
      // If we are able to contract [a], the evaluation y(intv_t) is
      // performed twice, thus increasing the computation time

      if(!s_y.is_interior_subset(s_x->codomain())){

          double a_diam_bef = a.diam();

          const Interval t_y = y.invert(s_x->codomain(), intv_t);
          a &= t_y - t_x;

          // Only if a has been contracted, we need to update s_y
          // This can avoid the tube evaluation to save computation time
          // TODO: The tube evaluation could be implemented more intelligently
          // since we already know where to evaluate it (as we did the same evaluation above)
          if(a.diam() < a_diam_bef){

              if(a.is_empty()){
                  x.set_empty();
                  y.set_empty();
                  return;
              }

              intv_t = t_x + a;
              s_y = y(intv_t);
          }
      }

      s_x->set_envelope(s_x->codomain() & s_y);
      s_x->set_input_gate(s_x->input_gate() & y(t_x.lb() + a));
      s_x->set_output_gate(s_x->output_gate() & y(t_x.ub() + a));

      if(s_x->is_empty()){
          a.set_empty();
          x.set_empty();
          y.set_empty();
          return;
      }

      s_x = s_x->next_slice();
    }

    // Build Tree for Tube x since we will evaluate/invert it in the following
    x.enable_synthesis();

    // iterate over the second tube y
    Slice *s_y = y.first_slice();
    while(s_y != NULL)
    {
      const Interval t_y = s_y->tdomain();
      Interval intv_t = t_y - a;

      Interval s_x = x(intv_t);

      // if the evaluation of the tube x, which we would invert inside [intv_t],
      // is already completely inside the codomain of s_y, no contraction for [a] can
      // be achieved and we can avoid the inversion to save computation time

      // TODO: see above

      if(!s_x.is_interior_subset(s_y->codomain())){

          double a_diam_bef = a.diam();

          const Interval t_x = x.invert(s_y->codomain(), intv_t);
          a &= t_y - t_x;

          // Only if a has been contracted, we need to update s_x
          // This can avoid the tube evaluation to save computation time
          // TODO: see above
          if(a.diam() < a_diam_bef){

              if(a.is_empty()){
                  x.set_empty();
                  y.set_empty();
                  return;
              }

              intv_t = t_y - a;
              s_x = x(intv_t);
          }
      }

      s_y->set_envelope(s_y->codomain() & s_x);
      s_y->set_input_gate(s_y->input_gate() & x(t_y.lb() - a));
      s_y->set_output_gate(s_y->output_gate() & x(t_y.ub() - a));

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
