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
#include "codac2_SlicedTube.h"
#include "codac2_SolutionCurveWrapper.h"


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

  /**
   * \brief Converts a CAPD SolutionCurve into a Codac SlicedTube
   * 
   * \param solution_curve the CAPD SolutionCurve to be converted
   * \param tdomain the TDomain defining the time slices of the resulting SlicedTube
   * \return the converted Codac SlicedTube with both the time slices (and eventually gates) of Codac and the gates from CAPD
   */
  codac2::SlicedTube<codac2::IntervalVector> to_codac(const codac2::SolutionCurveWrapper& solution_curve, const std::shared_ptr<TDomain>& tdomain);

}