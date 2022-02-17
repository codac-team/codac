//============================================================================
//                                  I B E X
// File        : ibex_sdSepCtcPairProj.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "codac_SepCtcPairProj.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace pyibex {

SepCtcPairProj::SepCtcPairProj(Ctc& ctc_in, Ctc& ctc_out, const IntervalVector& y_init, double prec) :
    Sep(ctc_in.nb_var), ctcIn(ctc_in), ctcOut(ctc_out),
    vars(ibex::BitSet(ctc_in.nb_var)), y_init(y_init)
{
    for(int i = 0; i < ctc_in.nb_var-y_init.size(); i++){
        vars.add(i);
    }
    for(int i = 0; i < y_init.size(); i++){
        vars.remove(ctc_in.nb_var+i);
    }
    // this->ctc_in = &ctc_in;
    // this->ctc_out = &ctc_in;
    // ctcForAll = new CtcForAll(ctc_in, vars, y_init, prec);
    // ctcExist = new CtcExist(ctc_out, vars, y_init, prec);

}

SepCtcPairProj::SepCtcPairProj(SepCtcPair& sep, const IntervalVector& y_init, double prec) :
    Sep(sep.nb_var), ctcIn(sep.ctc_in), ctcOut(sep.ctc_out),
    vars(ibex::BitSet(sep.nb_var)), y_init(y_init)
{
    for(int i = 0; i < sep.nb_var-y_init.size(); i++){
        vars.add(i);
    }
    for(int i = 0; i < y_init.size(); i++){
        vars.remove(sep.nb_var+i);
    }

    // ctc_in = &sep.ctc_in;
    // ctc_out = &sep_ctc_out;
    // ctcForAll = new CtcForAll(sep.ctc_in, vars, y_init, prec);
    // ctcExist = new CtcExist(sep_ctc_out, vars, y_init, prec);

}

SepCtcPairProj::SepCtcPairProj(Sep& sep, const IntervalVector& y_init, double prec) :
    Sep(sep.nb_var), ctcIn(*new CtcFromSep(sep, true)), ctcOut(*new CtcFromSep(sep, false)),
    vars(ibex::BitSet(sep.nb_var)), y_init(y_init)
{
    for(int i = 0; i < sep.nb_var-y_init.size(); i++){
        vars.add(i);
    }
    for(int i = 0; i < y_init.size(); i++){
        vars.remove(sep.nb_var+i);
    }

    // ctc_in = &sep.ctc_in;
    // ctc_out = &sep_ctc_out;
    // ctcForAll = new CtcForAll(sep.ctc_in, vars, y_init, prec);
    // ctcExist = new CtcExist(sep_ctc_out, vars, y_init, prec);

}


// SepCtcPairProj::SepCtcPairProj(Sep &sep, const IntervalVector& y_init, double prec) :
//     Sep(sep.nb_var), SepCtcPairProj
//     vars(ibex::BitSet::empty(sep.nb_var))
// {
//     for(int i = 0; i < sep.nb_var-y_init.size(); i++){
//         vars.add(false);
//     }
//     for(int i = 0; i < y_init.size(); i++){
//         vars.add(true);
//     }
//     ctcIn = new CtcFromSep(sep, true);
//     ctcOut = new CtcFromSep(sep, false);
//     ctcForAll = new CtcForAll(*ctcIn, vars, y_init, prec);
//     ctcExist = new CtcExist(*ctcOut, vars, y_init, prec);
// }



SepCtcPairProj::~SepCtcPairProj() {
    // delete ctcExist;
    // delete ctcForAll;
    // delete ctcIn;
    // delete ctcOut;
}

void SepCtcPairProj::separate(IntervalVector &x_in, IntervalVector &x_out)
{

    assert(x_in==x_out);
    CtcExist(ctcOut,vars,y_init,x_out.max_diam()/10).contract(x_out);
    CtcForAll(ctcIn,vars,y_init,x_in.max_diam()/10).contract(x_in);
    // ctcExist->contract(x_out);
    // ctcForAll->contract(x_in);

}





} // namespace pyibex
