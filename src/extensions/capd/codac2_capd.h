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
   * \brief Converts a Codac Interval object into an CAPD Interval object
   * 
   * \param x the Codac Interval to be converted
   * \return the converted CAPD Interval
   */
  capd::Interval to_capd(const codac2::Interval& x);

  /**
   * \brief Converts an CAPD Interval object into a Codac Interval object
   * 
   * \param x the CAPD Interval to be converted
   * \return the converted Codac Interval
   */
  codac2::Interval to_codac(const capd::Interval& x);

  /**
   * \brief Converts a Codac IntervalVector object into an CAPD IVector object
   * 
   * \param x the Codac IntervalVector to be converted
   * \return the converted CAPD IVector
   */
  capd::IVector to_capd(const codac2::IntervalVector& x);

  /**
   * \brief Converts an CAPD IVector object into a Codac IntervalVector object
   * 
   * \param x the CAPD IVector to be converted
   * \return the converted Codac IntervalVector
   */
  codac2::IntervalVector to_codac(const capd::IVector& x);

  /**
   * \brief Converts a Codac IntervalMatrix object into an CAPD IMatrix object
   * 
   * \param x the Codac IntervalMatrix to be converted
   * \return the converted CAPD IMatrix
   */
  capd::IMatrix to_capd(const codac2::IntervalMatrix& x);

  /**
   * \brief Converts an CAPD IMatrix object into a Codac IntervalMatrix object
   * 
   * \param x the CAPD IMatrix to be converted
   * \return the converted Codac IntervalMatrix
   */
  codac2::IntervalMatrix to_codac(const capd::IMatrix& x);

}