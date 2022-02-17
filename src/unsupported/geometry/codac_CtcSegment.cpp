//============================================================================
//                                P Y I B E X
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Jun 29, 2017
//============================================================================

#include "codac_CtcSegment.h"

using namespace std;

namespace pyibex {

CtcSegment::CtcSegment(double ax, double ay, double bx, double by) : Ctc(2),
    X_with_params(2+4) {

    init();

    X_with_params[2] = Interval(ax);
    X_with_params[3] = Interval(ay);
    X_with_params[4] = Interval(bx);
    X_with_params[5] = Interval(by);
}

CtcSegment::CtcSegment() : Ctc(6), X_with_params(0 /* unused */) {
  init();
}

void CtcSegment::init() {
  ibex::Variable x(2),a(2),b(2);

  ibex::Function *f = new ibex::Function(x,a,b,(b[0]-a[0])*(a[1]-x[1]) - (b[1]-a[1])*(a[0]-x[0]));

  ibex::Function *g = new ibex::Function(x,a,b,ibex::Return(ibex::min(a[0],b[0]) - x[0],
      ibex::min(a[1],b[1]) - x[1],
      x[0] - ibex::max(a[0],b[0]),
      x[1] - ibex::max(a[1],b[1]))
  );

  ncf = new NumConstraint(*f,ibex::EQ,true);
  ncg = new NumConstraint(*g,ibex::LEQ,true);

  ctc_f = new CtcFwdBwd(*ncf);
  ctc_g = new CtcFwdBwd(*ncg);
}

CtcSegment::~CtcSegment() {
  delete ncf; delete ncg;
  delete ctc_f; delete ctc_g;
}


void CtcSegment::contract(IntervalVector &box) {

  if (nb_var==6) {
    ctc_f->contract(box);
    if (box.is_empty()) return;
    ctc_g->contract(box);
  }
  else {
        IntervalVector X(X_with_params);
        X[0] = box[0];
        X[1] = box[1];
        //        X_with_params=cart_prod(box,)

        ctc_f->contract(X);
        if (X.is_empty()) { box.set_empty(); return; }

        ctc_g->contract(X);
        if (X.is_empty()) { box.set_empty(); return; }

    //        box = X_with_params.subvector(0,1);
        box[0] = X[0];
        box[1] = X[1];
  }
}

} // end namespace
