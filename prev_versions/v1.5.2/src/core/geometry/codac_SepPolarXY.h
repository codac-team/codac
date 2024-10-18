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


namespace codac {

/**
 * \ingroup geometry
 *
 * \brief Separator for point in sector.
 * A sector is defined by its center, a distance and an angle (with uncertainty).
 *
 */
class SepPolarXY : public ibex::Sep {

public:
  /**
   * @brief Construct a new SepPolarXY object
   * 
   * @param rho radius of the sector
   * @param theta angle of the sector
   */
	SepPolarXY(Interval rho, Interval theta);

  /**
   * \brief \f$\mathcal{S}\big([\mathbf{x}_{\textrm{in}}],[\mathbf{x}_{\textrm{out}}]\big)\f$
   *
   * \param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
   * \param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
   */
  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:

    /**
     * @brief compute the inner contraction 
     * 
     * @param x_in inner box
     */
    void contractIn(IntervalVector &x_in);

    /**
     * @brief compute the outer contraction
     * 
     * @param x_out  outer box
     */
    void contractOut(IntervalVector &x_out);



    Interval rho, theta;
    // Interval rho_m, rho_p;
    // Interval theta_m, theta_p;
    // Interval cmpl;
    codac::CtcPolar ctc;
};

} // end namespace pyibex

#endif // __IBEX_SEP_POLARXY_H__
