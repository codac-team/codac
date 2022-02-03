//============================================================================
//                               P Y I B E X
// File        : Separator for Disk Exists using symbolic itv
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug 23, 2017
// Last Update : Aug 23, 2017
//============================================================================

#ifndef __IBEX_SEP_DISKEXISTS_H__
#define __IBEX_SEP_DISKEXISTS_H__
#include "ibex_Sep.h"
#include "ibex_Interval.h"
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
class SepDiskExists : public ibex::Sep {

public:

	SepDiskExists(Interval x0, Interval y0, Interval rho);

  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

	void contract(IntervalVector &box, bool outer);

protected:

    Interval x0, y0, rho;
};


} // end namespace pyibex

#endif // __IBEX_SEP_DISKEXISTS_H__
