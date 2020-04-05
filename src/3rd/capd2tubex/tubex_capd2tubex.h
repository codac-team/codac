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

#include "tubex_TubeVector.h"
#include "ibex_IntervalVector.h"
#include "json/json.hpp"

namespace tubex
{
  /** \brief  Returns a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD
   * \param j json file that gives the parameter to CAPD to perform the guaranteed integration
   */
  std::vector<ibex::IntervalVector> capd2ibex(nlohmann::json j);

  /** \brief Convert a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD into a
   * tubex::TubeVector
   * \param ibex_curve a vector of ibex::IntervalVectors representing the curve computed by CAPD
   * */
  tubex::TubeVector ibex2tubex(std::vector<ibex::IntervalVector> ibex_curve);

  /** \brief Combination of capd2ibex and ibex2tubex, to generate a tube from a curve obtained by
   * the guaranteed integration of CAPD
   * \param j json file that gives the parameter to CAPD to perform the guaranteed integration
   */
  tubex::TubeVector capd2tubex (nlohmann::json j);
}

#endif