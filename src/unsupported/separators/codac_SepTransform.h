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

namespace codac {
/**
* \ingroup set
*
* \brief Image of a separator by a function \f$f\f$ where  an analytic expression 
* of \f$\mathbf{f}^{-1}\f$ is avaiable. 
* the computation is less pessimism than using a classical
* a forward / backward propagation (@see: Ibex::SepInverse)
* Transformation of a separator with an inversible function
*	\f$ \mathbf{f}(\mathcal{S}) = \{ \mathbf{f}^{-1}\circ \mathcal{S}_{in} \circ \mathbf{f}, \mathbf{f}^{-1}\circ \mathcal{S}_{out} \circ \mathbf{f} \} \f$
* Using a function and its inverse is less pessimism than using
* a forward / backward propagation (see: sepInverse)
*/

class SepTransform : public Sep {
public:

    /**
     * @brief Construct a new Sep Transform object 
     *  
     * @param s Separator to transform
     * @param ffwd Function \f$\mathbf{f}\f$ used for the forward transformation 
     * @param fbwd Function used for the backward transformation based on an explicite evaluation of \f$\mathbf{f}^{-1}\f$
     */
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
