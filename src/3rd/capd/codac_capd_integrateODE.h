/**
 *  \file
 *  ODE integration tools based on CAPD
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Codac
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CAPDINTEGRATEODE_H__
#define __CODAC_CAPDINTEGRATEODE_H__

#include "codac_TubeVector.h"
#include "codac_TFunction.h"
#include "codac_IntervalVector.h"
#include "codac_Function.h"

namespace codac
{
  // Autonomous

  /**
   * \brief Integrates the autonomous ODE \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$ using CAPD.
   *
   * \param tdomain temporal domain \f$[t_0,t_f]\f$
   * \param f the function \f$\mathbf{f}\f$ (defined with a `Function` object)
   * \param x0 the initial condition \f$\mathbf{x}_0\f$ at \f$t_0\f$
   * \param tube_dt sampling value \f$\delta\f$ for the temporal discretization of the resulting tube
   * \param capd_order (optional) order of the integration method
   * \param capd_dt (optional) custom time step for CAPD integration
   * \return TubeVector enclosing the solution
   */
  TubeVector CAPD_integrateODE(
    const Interval& tdomain, const Function& f, const IntervalVector& x0,
    double tube_dt = 0., int capd_order = 20, double capd_dt = 0.);

  // Non autonomous

  /**
   * \brief Integrates the non-autonomous ODE \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x},t)\f$ using CAPD.
   *
   * \param tdomain temporal domain \f$[t_0,t_f]\f$
   * \param f the temporal function \f$\mathbf{f}\f$ (defined with a `TFunction` object)
   * \param x0 the initial condition \f$\mathbf{x}_0\f$ at \f$t_0\f$
   * \param tube_dt sampling value \f$\delta\f$ for the temporal discretization of the resulting tube
   * \param capd_order (optional) order of the integration method
   * \param capd_dt (optional) custom time step for CAPD integration
   * \return TubeVector enclosing the solution
   */
  TubeVector CAPD_integrateODE(
    const Interval& tdomain, const TFunction& f, const IntervalVector& x0,
    double tube_dt = 0., int capd_order = 20, double capd_dt = 0.);
}

#endif