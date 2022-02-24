//============================================================================
//                               P Y I B E X
// File        : Polar separator with projection on the XY-plan
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Jun 29, 2017
//============================================================================

#ifndef __IBEX_SEP_POLARXY_H__
#define __IBEX_SEP_POLARXY_H__
#include "ibex_Sep.h"
#include "codac_CtcPolar.h"

using namespace std;

using ibex::Interval;
using ibex::IntervalVector;


namespace pyibex {

/**
 * \ingroup iset
 *
 * \brief Separator for point in sector.
 * A sector is defined by its center, a distance and an angle (with uncertainty).
 *
 */
class SepPolarXY : public ibex::Sep {

public:

	SepPolarXY(Interval rho, Interval theta);

  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:

    void contractIn(IntervalVector &x_in);
    void contractOut(IntervalVector &x_out);


    Interval rho, theta;
    Interval rho_m, rho_p;
    Interval theta_m, theta_p;
    Interval cmpl;
    codac::CtcPolar ctc;
};

// class SepPolarXYT : public Sep{
// public:
// 	SepPolarXYT(const Interval& rho, const Interval& phi);
// 	void separate(IntervalVector& x_in, IntervalVector& x_out);
// private:
// 	Interval rho;
// 	Interval phi;
// 	// SepPolarXY& sep;
// };

} // end namespace pyibex

#endif // __IBEX_SEP_POLARXY_H__
