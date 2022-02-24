//============================================================================
//                                  I B E X
// File        : ibex_SepCtcPairProj.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_CTCPAIR_PROJ_H__
#define __IBEX_SEP_CTCPAIR_PROJ_H__



#include <ibex_IntervalVector.h>
#include <ibex_SepCtcPair.h>
#include <ibex_SepFwdBwd.h>
#include <ibex_CtcForAll.h>
#include <ibex_CtcExist.h>
#include <ibex_SepFwdBwd.h>

#include <ibex_BitSet.h>

using ibex::Ctc;
using ibex::Sep;
using ibex::IntervalVector;
using ibex::CtcExist;
using ibex::CtcForAll;
using ibex::BitSet;
using ibex::SepCtcPair;
using ibex::SepFwdBwd;

namespace pyibex {


class CtcFromSep : public Ctc {
public:
  CtcFromSep(Sep &sep, bool return_ctc_in) : Ctc(sep.nb_var), sep(sep),
  return_ctc_in(return_ctc_in) {}
  void contract(IntervalVector& x){
    IntervalVector x_in(x), x_out(x);
    sep.separate(x_in, x_out);
    x &=  ( (return_ctc_in == true) ? x_in : x_out );
  }

private:
  Sep& sep;
  bool return_ctc_in;
};

class SepCtcPairProj : public Sep {

public:
    SepCtcPairProj(Ctc& ctc_in, Ctc& ctc_out, const IntervalVector& y_init, double prec);
    SepCtcPairProj(SepCtcPair &sep, const IntervalVector& y_init, double prec);
    SepCtcPairProj(Sep &sep, const IntervalVector& y_init, double prec);

    ~SepCtcPairProj();

    void separate(IntervalVector &x_in, IntervalVector &x_out);

protected:
    Ctc& ctcIn;
    Ctc& ctcOut;
    // CtcExist *ctcExist;
    // CtcForAll *ctcForAll;
    // bool delete_ctc;
    IntervalVector y_init;
    BitSet vars;

};

} // namespace pyibex

#endif // __IBEX_SEP_CTCPAIR_PROJ_H__
