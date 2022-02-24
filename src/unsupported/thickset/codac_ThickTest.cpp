//============================================================================
//                               P Y I B E X
// File        : ThickTest.cpp
// Author(s)   : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#include <codac_ThickTest.h>

// ThickBoolean isInside()

ThickBoolean ThickDisk::test(const IntervalVector& X) {
    if (X.is_disjoint(this->bbox)){
      return OUT;
    }
    Interval Xm = ibex::max(Interval(0), ibex::sign( (X[0]-mx.ub())*(X[0]-mx.lb()))) * \
            ibex::min(ibex::sqr(X[0]-mx.lb()),ibex::sqr(X[0]-mx.ub()) ) \
          + ibex::max(Interval(0), ibex::sign( (X[1]-my.ub())*(X[1]-my.lb()))) \
            * ibex::min(ibex::sqr(X[1]-my.lb()),ibex::sqr(X[1]-my.ub()) );

    Interval Xp = ibex::max(ibex::sqr(X[0]-mx.lb()),ibex::sqr(X[0]-mx.ub())) + \
                ibex::max(ibex::sqr(X[1]-my.lb()),ibex::sqr(X[1]-my.ub()));

    Interval Xub = Xm | Xp;

    if (Rmax.is_disjoint(Xub)){
			return OUT;
		} else if (Xub.is_subset(Rmin) ){
			return IN;
		} else {
			bool b1 = ( Xm - Rmax.ub() ).is_subset(Interval::NEG_REALS);
      bool b2 = ( Rmax.lb() - Xp).is_subset(Interval::NEG_REALS);
      Interval B1 = ( Xm - Rmin.lb());
      Interval B2 = ( Rmin.ub() - Xp);

      bool incl = false;
      if (B1.ub() < 0 || B2.ub() < 0)
        incl = true;

      if (incl && b1 && b2)
        return MAYBE;
      else if (incl && !(b1 && b2) )
        return MAYBE_OUT;
      else if (!incl && (b1 && b2))
        return MAYBE_IN;
		}
    return UNK;
}


ThickBoolean ThickQInter::test(const IntervalVector& box){

  Interval sum_lb = Interval(0);
  Interval sum_ub = Interval(0);
  for (int i=0; i<list.size(); i++) {
    ThickBoolean b = list[i].test(box);
    switch(b){
      case IN:
        sum_lb += 1.;
        sum_ub += 1.;
        break;
      case MAYBE:
        sum_ub += 1.;
        break;
      case MAYBE_OUT:
        sum_ub += Interval(0, 1);
        break;
      case MAYBE_IN:
        sum_lb += Interval(0, 1);
        sum_ub += 1.;
        break;
      case UNK:
        sum_lb += Interval(0, 1);
        sum_ub += Interval(0, 1);
    }
    //
    //
    // }
    //   if( b == IN){
    //     sum_lb += 1.;
    //     sum_ub += 1.;
    //   } else if ( b == MAYBE){
    //     sum_ub += 1.;
    //   } else if ( b == MAYBE_OUT){
    //     sum_ub += Interval(0, 1)
    //   } else if ( b == )
  }

		Interval b_sup = sum_lb | sum_ub;
		Interval b_sub = sum_lb & sum_ub;
		bool b1 = (this->q - sum_ub).ub() <= 0; // [sum_lb, sum_ub] intersects [q, +oo]
		bool b2 = (sum_lb - this->q).ub() < 0;  // [sum_lb, sum_ub] is not included in [q, +oo]

		if (b_sup.is_subset(Interval(this->q, 1000))){
			return IN;
		} else if ( b_sup.is_subset(Interval(0, this->q-1)) ){
			return OUT;
    } else if ( b1 && !b2 ){
			return MAYBE_IN;
		} else if ( !b1 && b2) {
			return MAYBE_OUT;
		} else if ( b1 && b2){
			return MAYBE;
    }
		return UNK;
}


ThickBoolean ThickAnd::test(const IntervalVector& box){

  ThickBoolean b = list[0].test(box); //list[0].test(box);
  for (int i=1; i<list.size(); i++) {
    b = opAnd(b, list[i].test(box));
  }
  return b;
}

ThickBoolean ThickInter::test(const IntervalVector& box){

  ThickBoolean b = list[0].test(box); //list[0].test(box);
  for (int i=1; i<list.size(); i++) {
    b = opInter(b, list[i].test(box));
  }
  return b;
}

ThickBoolean ThickOr::test(const IntervalVector& box){
  ThickBoolean b = list[0].test(box); //list[0].test(box);
  for (int i=1; i<list.size(); i++) {
    b = opOr(b, list[i].test(box));
    if (b == IN) break;
  }
  return b;
}



ThickBoolean ThickfIn::test(const IntervalVector& box){
  ThickBox X(f_lb(box), f_ub(box));
  if (X.superset().is_subset(y)){
    return IN;
  } else if (X.superset().is_disjoint(y)){
    return OUT;
  } else {
    bool b1 = X.intersects(y);
    bool b2 = X.is_not_subset(y);
    if ( b1 && b2 ) return MAYBE;
    if (!b1 && b2) return MAYBE_OUT;
    if (!b2 && b1) return MAYBE_IN;
  }
  return UNK;
}


ThickSector::ThickSector(const Interval&x,const Interval&y,const Interval& theta,const Interval& d):
  x(x), y(y), theta(theta), d(d), ThickTest(2),
  tDisk(x, y, Interval(0, d.lb()), Interval(0, d.ub()) ) {}


ThickSector::ThickSector(const Interval&x,const Interval&y,const Interval& theta,const Interval& d1, const Interval& d2):
  x(x), y(y), theta(theta), d(d), ThickTest(2),
  tDisk(x, y, d1, d2 ) {}


ThickBoolean ThickSector::testPlan(const IntervalVector& X, const Interval& th){
  Interval ux = cos( th );
  Interval uy = sin( th );
  Interval mx_m = ( X[0] - x.ub() );
  Interval my_m = ( X[1] - y.ub() );
  Interval mx_p = ( X[0] - x.lb() );
  Interval my_p = ( X[1] - y.lb() );


  Interval vmin = min( ux*my_m, ux*my_p ) -  max( uy*mx_m, uy*mx_p );
  Interval vmax = max( ux*my_m, ux*my_p ) -  min( uy*mx_m, uy*mx_p );

  // Interval vmax = ux * ( X[1] - y.lb() ) - uy * ( X[0] - x.ub() );
  // std::cout <<  vmin << " "  << vmax << " " << ThickInterval(vmin, vmax) << "\n";
  return  ThickInterval(vmin, vmax).is_inside(Interval(0, std::numeric_limits<double>::max()));
}
// Interval limitX(Interval& theta){
//   Interval ix = Interval::EMPTY_SET;
//   Interval zeroPio2 = Interval(0) | Interval::PI/2.;
//   Interval Pio2Pi = Interval(0) | ;
//   Interval zeromPio2 = Interval(0) | Interval::PI/2.;
//   Interval mPio2mPi = Interval(0) | Interval::PI/2.;
//
//   Interval th1 = (theta & );
//   Interval th2 = (theta & (Interval(0) | Interval::PI/2.));
//   Interval th3 = (theta & (Interval(0) | Interval::PI/2.));
//   Interval th4 = (theta & (Interval(0) | Interval::PI/2.));
//
//   if  .intersects(){
//     ix |= Interval::POS_REALS;
//   } else if theta.intersects(Interval(0) | Interval::PI/2.){
// }
ThickBoolean ThickSector::test(const IntervalVector& X){

  ThickBoolean b1 = testPlan(X, theta.lb());
  ThickBoolean b2 = ibex::opNot( testPlan(X, theta.ub()) );
  ThickBoolean b3 = tDisk.test(X);
  return b1 && b2 && b3;

}
