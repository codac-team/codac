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
#include "tubex_StructureException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDelay::CtcDelay()
  {

  }

  bool CtcDelay::contract(const Interval& a, const TubeVector& x, TubeVector& y) const
  {
    DomainException::check(x, y);
    DimensionException::check(x, y);
    StructureException::check(x, y);

    Interval intv_t;
    const TubeSlice *slice_x = x.getFirstSlice();
    TubeSlice *slice_y = y.getFirstSlice();

    while(slice_x != NULL)
    {
      intv_t = slice_y->domain().lb() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->setInputGate(slice_y->inputGate() & x[intv_t]);

      intv_t = slice_y->domain() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->setEnvelope(slice_y->codomain() & x[intv_t]);

      intv_t = slice_y->domain().ub() - a;
      if(intv_t.is_subset(y.domain()))
        slice_y->setOutputGate(slice_y->outputGate() & x[intv_t]);

      slice_x = slice_x->nextSlice();
      slice_y = slice_y->nextSlice();
    }

    // todo: return value
  }
}