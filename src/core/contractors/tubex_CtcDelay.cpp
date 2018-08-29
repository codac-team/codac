/* ============================================================================
 *  tubex-lib - CtcDelay class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <list>
#include "tubex_CtcDelay.h"
#include "tubex_SlicingException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay() : Ctc()
  {

  }

  void CtcDelay::contract(const Interval& a, const Tube& x, Tube& y) const
  {
    DomainException::check(x, y);
    SlicingException::check(x, y);

    const Slice *slice_x = x.get_first_slice();
    Slice *slice_y = y.get_first_slice();

    while(slice_x != NULL)
    {
      Interval intv_t = slice_y->domain().lb() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->set_input_gate(slice_y->input_gate() & x(intv_t));

      intv_t = slice_y->domain() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->set_envelope(slice_y->codomain() & x(intv_t));

      intv_t = slice_y->domain().ub() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->set_output_gate(slice_y->output_gate() & x(intv_t));

      slice_x = slice_x->next_slice();
      slice_y = slice_y->next_slice();
    }
  }

  void CtcDelay::contract(const Interval& a, const TubeVector& x, TubeVector& y) const
  {
    // todo: check dim x, y

    for(int i = 0 ; i < x.size() ; i++)
      contract(a, x[i], y[i]);
  }
}