//============================================================================
//                                 P Y I B E X
// File        : ibex_CtcHull.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Oct 29, 2017
// Last Update : Oct 29, 2017
//============================================================================

#ifndef __IBEX_CTC_HULL_H__
#define __IBEX_CTC_HULL_H__

#include "ibex_Sep.h"
#include "ibex_Ctc.h"
#include "ibex_Function.h"
#include "ibex_Bsc.h"

using ibex::Sep;
using ibex::Ctc;
using ibex::Function;
using ibex::IntervalVector;
using ibex::Bsc;

namespace pyibex {
/**
* \ingroup set
*
* TODO CtcHull Description
*/

class CtcHull : public Ctc {
public:
    CtcHull(Sep& sep, double eps, ibex::Bsc& bsc);
    // ~CtcHull();
    virtual void contract(IntervalVector& x);

    /** Separator */
    Sep& sep;

private:
    /** Bisector */
    Bsc& bsc;
    /** precision */
    double eps;

};

} // end namespace pyibex
#endif // __IBEX_CTC_HULL_H__
