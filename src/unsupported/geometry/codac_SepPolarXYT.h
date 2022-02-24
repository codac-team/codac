//============================================================================
//                               P Y I B E X
// File        : Polar separator with projection on the XYTheta-plan
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Jun 29, 2017
//============================================================================

#ifndef __IBEX_SEP_POLARXYT_H__
#define __IBEX_SEP_POLARXYT_H__
#include "ibex_Sep.h"
#include "codac_CtcPolar.h"

using namespace std;
using ibex::Sep;


/*
BUG In POLAR --> config
Rho = [0x1.b9adea897635fp+2 , 0x1.c01450efdc9c5p+2]
Theta = [-0x1.95b92733b853ep+2 , -0x1.690af69de9cacp+2]
X[0] = [0x1.be002da3cfcc6p+2 , 0x1.be002da3cfcc6p+2]
X[1] = [-0x1.32b8f5d87284bp-2 , -0x1.32b8f5d87284bp-2]
*/

namespace pyibex {

/**
 * \ingroup iset
 *
 * \brief Separator for point in sector.
 * A sector is defined by its center, a distance and an angle (with uncertainty).
 *
 */
class SepPolarXYT : public Sep {

public:

	SepPolarXYT(Interval rho, Interval theta, double mx, double my);

  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:
	void contract(Interval &x, Interval &y, Interval &theta, Interval &rho, Interval &phi);
    void contractIn(IntervalVector &x_in);
    void contractOut(IntervalVector &x_out);


   	double mx, my;
    Interval rho, theta;
    Interval rho_m, rho_p;
    Interval theta_m, theta_p;
    Interval cmpl;
};

} // end namespace

#endif // __IBEX_SEP_POLARXY_H__
