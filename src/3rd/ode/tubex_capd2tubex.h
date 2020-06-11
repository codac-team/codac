/** 
 *  \file
 *  CAPD to Tubex
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CAPD_H__
#define __TUBEX_CAPD_H__

#include <capd/capdlib.h>
#include "tubex_TubeVector.h"
#include "ibex_IntervalVector.h"


namespace tubex
{

    std::string tubexFnc2capdString(const TFunction& f);


  /** \brief  Returns a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD
   *
   * \param domain period of tine on which we would like to perform the integration
   * \param vectorField the vector field associated to the function that we would like to integrate
   * \param x0 The initial condition
   * \param timestep time step desired for the integration. If equal to 0 CAPD will calculate the timestep by itself
   * to increase calculation speed
   * \return guaranteed curve computed by CAPD
   */

    std::vector<ibex::IntervalVector> capd2ibex(const ibex::Interval& domain, capd::IMap& vectorField, const ibex::IntervalVector& x0,
                                                const double& timestep=0);


  /** \brief Convert a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD into a
   * TubeVector
   * \param ibex_curve a vector of ibex::IntervalVectors representing the curve computed by CAPD
   * \return guaranteed curve computed by CAPD in a TubeVector
   */

    TubeVector ibex2tubex(std::vector<ibex::IntervalVector>& ibex_curve);

  /** \brief Combination of capd2ibex and ibex2tubex, to generate a tube from a curve obtained by
   * the guaranteed integration of CAPD
   * \param domain period of tine on which we would like to perform the integration
   * \param f function that we would like to integrate
   * \param x0 The initial condition
   * \param timestep time step desired for the integration. If equal to 0 CAPD will calculate the timestep by itself
   * to increase calculation speed
   * \return tube from a curve obtained by the guaranteed integration of CAPD
   */

    TubeVector capd2tubex(const ibex::Interval& domain, const TFunction& f, const ibex::IntervalVector& x0, const double timestep);

}

#endif