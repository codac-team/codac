//============================================================================
//                                  I B E X
// File        : ibex_SepTransform.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#include "codac_SepTransform.h"

namespace codac {

SepTransform::SepTransform(Sep& s, Function& ffwd, Function& fbwd) :
  Sep(s.nb_var), s(s), fbwd(fbwd), ffwd(ffwd), yin(fbwd.image_dim()), yout(fbwd.image_dim()) {
}

SepTransform::~SepTransform() {

}
void SepTransform::separate(IntervalVector& xin, IntervalVector& xout){

  assert(xin.size()==fbwd.nb_var() && xout.size() == fbwd.nb_var());


  xin &= xout;
  yin = ffwd.eval_vector(xin);
  yout = yin;

  s.separate(yin, yout);

  if( yin.is_empty() )
    xin.set_empty();
  else{
    xin &= fbwd.eval_vector(yin);
    ffwd.backward(yin, xin);
  }

  if( yout.is_empty() )
    xout.set_empty();
  else{
    xout &= fbwd.eval_vector(yout);
    ffwd.backward(yout, xout);
  }

}

} // end namespace pyibex
