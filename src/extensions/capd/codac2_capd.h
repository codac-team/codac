/** 
 *  \file codac2_capd.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <capd/intervals/lib.h>
#include <capd/vectalg/lib.h>
#include "codac2_IntervalVector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  /**
   * \brief Casts a Codac Interval object into an CAPD Interval object
   * 
   * \param x const Codac type Interval
   * \return CAPD type Interval
   */
  capd::Interval to_capd(const codac2::Interval& x);

  /**
   * \brief Casts an CAPD Interval object into a Codac Interval object
   * 
   * \param x const CAPD type Interval
   * \return Codac type Interval
   */
  codac2::Interval to_codac(const capd::Interval& x);

  /**
   * \brief Casts a Codac IntervalVector object into an CAPD IntervalVector object
   * 
   * \param x const Codac type IntervalVector
   * \return CAPD type IntervalVector
   */
  capd::IVector to_capd(const codac2::IntervalVector& x);

  /**
   * \brief Casts an CAPD IntervalVector object into a Codac IntervalVector object
   * 
   * \param x const CAPD type IntervalVector
   * \return Codac type IntervalVector
   */
  codac2::IntervalVector to_codac(const capd::IVector& x);

  /**
   * \brief Casts a Codac IntervalMatrix object into an CAPD IntervalMatrix object
   * 
   * \param x const Codac type IntervalMatrix
   * \return CAPD type IntervalMatrix
   */
  capd::IMatrix to_capd(const codac2::IntervalMatrix& x);

  /**
   * \brief Casts an CAPD IntervalMatrix object into a Codac IntervalMatrix object
   * 
   * \param x const CAPD type IntervalMatrix
   * \return Codac type IntervalMatrix
   */
  codac2::IntervalMatrix to_codac(const capd::IMatrix& x);

}