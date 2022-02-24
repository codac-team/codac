//============================================================================
//                                  I B E X
// File        : ibex_CtcTransform.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#include "codac_CtcTransform.h"

namespace pyibex {

CtcTransform::CtcTransform(Ctc& ctc, Function& ffwd, Function& fbwd) :
  Ctc(ctc.nb_var), ctc(ctc), fbwd(fbwd), ffwd(ffwd), y(fbwd.image_dim()) {
}

CtcTransform::~CtcTransform() {

}
void CtcTransform::contract(IntervalVector& box){

  assert(box.size()==fbwd.nb_var() && box.size() == fbwd.nb_var());

  y = ffwd.eval_vector(box);

  ctc.contract(y);

  if( y.is_empty() )
    box.set_empty();
  else
    box &= fbwd.eval_vector(box);

}

} // end namespace pyibex
