//============================================================================
//                                  I B E X
// File        : ibex_CtcTransform.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#ifndef __IBEX_CTC_TRANSFORM_H__
#define __IBEX_CTC_TRANSFORM_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"


using ibex::Ctc;
using ibex::Function;
using ibex::IntervalVector;

namespace codac {
/**
* \ingroup set
*
* \brief Transformation of a separator with an inversible transformation
*	T(S)(X) = { ffwd*Sin*fbwd(X), ffwd*Sout*fbwd(X)}
* Using a function and its inverse is less pessimism than using
* a forward / backward propagation (see: sepInverse)
*/

class CtcTransform : public Ctc {
public:
    CtcTransform(Ctc& s, Function& ffwd, Function &fbwd);
    ~CtcTransform();
    virtual void contract(IntervalVector& box);

    /** contractor */
    Ctc& ctc;
    /** Inverse transformation */
    Function& fbwd;
    /** transformation */
    Function& ffwd;

private:
    IntervalVector y;

};

} // end namespace pyibex
#endif // __IBEX_CTC_TRANSFORM_H__
