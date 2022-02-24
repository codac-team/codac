//============================================================================
//                                  I B E X
// File        : ibex_SepTransform.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#ifndef __IBEX_SEP_TRANSFORM_H__
#define __IBEX_SEP_TRANSFORM_H__

#include "ibex_Sep.h"
#include "ibex_Function.h"


using ibex::Sep;
using ibex::Function;
using ibex::IntervalVector;

namespace pyibex {
/**
* \ingroup set
*
* \brief Transformation of a separator with an inversible transformation
*	T(S)(X) = { ffwd*Sin*fbwd(X), ffwd*Sout*fbwd(X)}
* Using a function and its inverse is less pessimism than using
* a forward / backward propagation (see: sepInverse)
*/

class SepTransform : public Sep {
public:
    SepTransform(Sep& s, Function& ffwd, Function &fbwd);
    ~SepTransform();
    virtual void separate(IntervalVector& xin, IntervalVector& xout);

    /** Separator */
    Sep& s;
    /** Inverse transformation */
    Function& fbwd;
    /** transformation */
    Function& ffwd;

private:
    IntervalVector yin, yout;

};

} // end namespace pyibex
#endif // __IBEX_SEP_TRANSFORM_H__
